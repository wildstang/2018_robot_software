#include "RobotCore.h"

#include <cstring>

#include "io/Output.h"
#include "io/Input.h"
#include "io/InputManager.h"
#include "io/OutputManager.h"
#include "io/outputs/WsSmartOutput.h"
#include "logger/Probe.h"
#include "subsystems/SubsystemManager.h"
#include "auto/AutoManager.h"
#include "utils/ws_string.h"
#include "utils/ws_utils.h"
#include "config/ConfigConsole.h"
#include "config/ConfigValueRef.h"

#include "logger/WsLogger.h"

RobotCore* RobotCore::sp_instance = NULL;

RobotCore::RobotCore( HardwareHandle* p_hardware, AutoStepFactory* p_factory )
    : WildstangModule( std::string( "robot" ), NULL )
    , WsNetConsole( 1114 )
    , ap_inputManager( NULL )
    , ap_outputManager( NULL )
    , ap_subsystemManager( NULL )
    , ap_autoManager( NULL )
    , ap_configConsole( NULL )
    , ap_hardwareHandle( p_hardware )
    , ap_autoStepFactory( p_factory )
    , a_consoleMutex( PTHREAD_MUTEX_INITIALIZER )
    , a_configFile()
    , a_autoProgramDir()
    , a_loadConfigFile( false )
    , a_loadAutoProgramsFromFile( false )
    , a_rebuildFlag( false )
    , a_probesMuted( false )
    , a_startupTimestamp( 0 )
    , a_badConfigCheck( 0xDEADBEEF )
    , a_activeProbes()
{
    WS_ASSERT( ap_hardwareHandle != NULL );
    WS_ASSERT( ap_autoStepFactory != NULL );
    WS_ASSERT( sp_instance == NULL );
    sp_instance = this;

    WsLogOutput::setInitBegin();
    logging_init( WS_STD_LOG_OUTPUT | WS_NET_LOG_OUTPUT | WS_DEBUG_LOG_OUTPUT );

    pthread_mutex_init( &a_consoleMutex, NULL );

    WS_INFO( "RobotCore constructor" );

    a_startupTimestamp = ws_seconds();

    //
    // The bad config check does not kick out a config completely so to speak
    //
    // Rather if the config check is not 0xF00DFEED, then it will try to load
    // the config from the backup file that you specify. And even then, if it
    // fails to load from the backup file, you'll at least have whatever sort
    // of configuration came from the first file. It's just an extra added
    // layer of defense.
    //
    CONFIG_VAR( "bad_config_check", a_badConfigCheck, int );
}

RobotCore::~RobotCore( void )
{
    a_activeProbes.clear();

    if( ap_autoManager )
    {
        delete ap_autoManager;
        ap_autoManager = NULL;
    }

    if( ap_subsystemManager )
    {
        delete ap_subsystemManager;
        ap_subsystemManager = NULL;
    }

    if( ap_inputManager )
    {
        delete ap_inputManager;
        ap_inputManager = NULL;
    }

    if( ap_outputManager )
    {
        delete ap_outputManager;
        ap_outputManager = NULL;
    }

    if( ap_configConsole )
    {
        delete ap_configConsole;
        ap_configConsole = NULL;
    }

    delete ap_hardwareHandle;
    ap_hardwareHandle = NULL;

    pthread_mutex_destroy( &a_consoleMutex );

    WS_ASSERT( sp_instance == this );
    sp_instance = NULL;
}

int RobotCore::initialize( void )
{
    WS_INFO( "creating input manager" );
    ap_inputManager = new InputManager( this );
    WS_INFO( "creating output manager" );
    ap_outputManager = new OutputManager( this );
    WS_INFO( "creating subsystem manager" );
    ap_subsystemManager = new SubsystemManager();
    WS_INFO( "creating auto manager" );
    ap_autoManager = new AutoManager( ap_autoStepFactory );
    ap_autoStepFactory = NULL; // give control to auto manager

    if( a_loadAutoProgramsFromFile )
    {
        ap_autoManager->setAutoProgramDir( a_autoProgramDir );
    }

    WS_INFO( "creating inputs" );
    this->createInputs();
    WS_INFO( "creating outputs" );
    this->createOutputs();
    WS_INFO( "creating subsystems" );
    this->createSubsystems();

    ap_configConsole = new ConfigConsole( this );
    ap_configConsole->initializeConfigConsole();

    this->startSocket();

    if( a_loadConfigFile )
    {
        WS_INFO( "applying config file %s", a_configFile.c_str() );
        int err = this->loadConfigFile( a_configFile );

        if( a_badConfigCheck != ( int )0xF00DFEED )
        {
            WS_ERROR( "your config check value is 0x%08x, trying the backup file. "
                      "Must be set to 0xF00DFEED (%d). "
                      "Robots get hungry and must be Feeded Foods.",
                      a_badConfigCheck, 0xF00DFEED );
            err = 1;
        }

        if( err )
        {
            WS_INFO( "trying the backup config file..." );

            std::string backup_config;
            const char* config_file_cstr = a_configFile.c_str();
            const char* dot_json = strstr( config_file_cstr, ".json" );
            if( dot_json )
            {
                backup_config = a_configFile.substr( 0, dot_json - config_file_cstr );
                backup_config.append( ".bak" );

                err = this->loadConfigFile( backup_config );
                if( err )
                {
                    WS_ERROR( "was not able to load the backup config %s. "
                              "It is what it is at this point.", backup_config.c_str() );
                }
            }
            else
            {
                WS_ERROR( "Could not deduce the name of the backup config file." );
            }
        }

        if( err )
        {
            WS_ERROR( "errors occurred while applying the config file. "
                      "Some speed controllers may not work." );
        }
    }
    else
    {
        WS_WARNING( "proceeding without config file." );
    }

    WS_INFO( "initializing inputs" );
    ap_inputManager->init();
    WS_INFO( "initializing outputs" );
    ap_outputManager->init();
    WS_INFO( "initializing subsystems" );
    ap_subsystemManager->init();
    WS_INFO( "initializing auto programs" );
    ap_autoManager->init();

    WS_INFO( "initializing hardware" );
    ap_hardwareHandle->initialize();

    WS_INFO( "init complete." );
    WsLogOutput::setInitComplete();

    return 0;
}

void RobotCore::executeUpdate( void )
{
    WS_FUNC_HEAD;

    if( a_rebuildFlag )
    {
        a_rebuildFlag = false;

        WS_INFO( "reinitializing inputs" );
        ap_inputManager->init();

        WS_INFO( "reinitializing outputs" );
        ap_outputManager->init();
    }

    // Read input from hardware
    ap_inputManager->update();

    // Let subsystems react to changes
    ap_autoManager->update();
    ap_subsystemManager->update();

    // Update outputs - send data to devices
    ap_outputManager->update();

    //
    // Display probes
    //

    pthread_mutex_lock( &a_consoleMutex );
    if( ( a_activeProbes.size() > 0 ) && !a_probesMuted )
    {
        std::string probe_str;
        probe_str.append( "probes: " );
        std::set< Probe* >::iterator i = a_activeProbes.begin();
        for(; i != a_activeProbes.end(); ++i )
        {
            Probe* p_probe = *i;
            probe_str.append( ws_std_string_printf( "%s -> %s ## ",
                                                    p_probe->getName().c_str(),
                                                    p_probe->get().c_str() ) );
        }

        net_printf( "%s\n", probe_str.c_str() );
    }
    pthread_mutex_unlock( &a_consoleMutex );

    WS_FUNC_FOOT;
}

RobotCore* RobotCore::getRobotCore( void )
{
    return sp_instance;
}

Input* RobotCore::makeInput( Input* p_input )
{
    WS_ASSERT( p_input != NULL );
    this->getInputManager()->addInput( p_input );
    return p_input;
}

Output* RobotCore::makeOutput( Output* p_output )
{
    WS_ASSERT( p_output != NULL );
    this->getOutputManager()->addOutput( p_output );
    return p_output;
}

Input* RobotCore::makeInput( std::string name, inputTypeT type )
{
    Input* p_input = this->getHardware()->getIOFactory()->createInput( name, type );
    WS_ASSERT( p_input != NULL );
    return this->makeInput( p_input );
}

Output* RobotCore::makeOutput( std::string name, outputTypeT type )
{
    Output* p_output = this->getHardware()->getIOFactory()->createOutput( name, type );
    WS_ASSERT( p_output != NULL );
    return this->makeOutput( p_output );
}

Output* RobotCore::makeSmartOutput( std::string name, std::string in, std::string out )
{
    smartOutputCmdT cmd;

    cmd.control_id = SMART_OUTPUT_CONTROL_MODE_ID_NONE;
    cmd.speed = 0.0;
    cmd.follow_output_id = 0;
    cmd.pos_setpoint = 0.0;
    cmd.vel_setpoint = 0.0;
    cmd.brake_mode = false;
    cmd.reset_sensor = false;

    Output* p_output = new WsSmartOutput( name, in, out, cmd );
    return this->makeOutput( p_output );
}

void RobotCore::setConfigFileName( std::string name )
{
    a_configFile = name;
    a_loadConfigFile = true;
}

void RobotCore::setAutoProgramDir( std::string dir )
{
    a_autoProgramDir = dir;
    a_loadAutoProgramsFromFile = true;
}

int RobotCore::handleLine( const char* line )
{
    std::vector< std::string > tok = ws_strtok_sep( line, " \n\r" );
    if( tok.size() <= 0 ) return 0;

    std::string command = tok[ 0 ];

    int err = 0;
    if( 0 == strncmp( command.c_str(), "pdp_reset", 9 ) )
    {
        this->getHardware()->clearPdpStickyFaults();
    }
    else if( 0 == strncmp( command.c_str(), "pcm_reset", 9 ) )
    {
        this->getHardware()->clearPcmStickyFaults();
    }
    else if( 0 == strncmp( command.c_str(), "rebuild", 7 ) )
    {
        // atomic write, does not need to be synchronized.
        this->setRebuildFlag();
    }
    else if( 0 == strncmp( command.c_str(), "probe", 11 ) )
    {
        err = this->handleProbe( tok );
    }
    else if( 0 == strncmp( command.c_str(), "unprobe", 12 ) )
    {
        err = this->handleUnProbe( tok );
    }
    else if( 0 == strncmp( command.c_str(), "lsprobe", 7 ) )
    {
        err = this->handleLsProbes( tok );
    }
    else if( 0 == strncmp( command.c_str(), "mute", 4 ) )
    {
        a_probesMuted = true;
    }
    else if( 0 == strncmp( command.c_str(), "unmute", 6 ) )
    {
        a_probesMuted = false;
    }
    else if( 0 == strncmp( command.c_str(), "tree", 4 ) )
    {
        err = this->handleTree();
    }
    else if( 0 == strncmp( command.c_str(), "active", 6 ) )
    {
        err = this->handleActive();
    }
    else if( 0 == strncmp( command.c_str(), "clear", 5 ) )
    {
        pthread_mutex_lock( &a_consoleMutex );
        a_activeProbes.clear();
        pthread_mutex_unlock( &a_consoleMutex );
    }
    else if( 0 == strncmp( command.c_str(), "uptime", 6 ) )
    {
        uint64_t uptime = ws_seconds() - a_startupTimestamp;
        net_printf( ">> %s\n", ws_format_seconds( uptime ).c_str() );
    }
    else if( 0 == strncmp( command.c_str(), "help", 4 ) )
    {
        net_printf( ">> help, pdp_reset, pcm_reset, rebuild, probe, unprobe, lsprobe, mute, unmute, tree, active, clear, uptime\n" );
    }
    else
    {
        net_printf( "!! unrecognized command\n" );
        err = 1;
    }

    return err;
}

int RobotCore::handleProbe( const std::vector< std::string >& rc_args )
{
    if( rc_args.size() != 3 )
    {
        net_printf( "!! usage: probe MODULE PROBE\n" );
        return 1;
    }

    std::string module = rc_args[ 1 ];
    std::string probe = rc_args[ 2 ];

    WildstangModule* p_module = this->resolve( module );
    if( p_module == NULL )
    {
        net_printf( "!! module %s not found\n", module.c_str() );
        return 2;
    }

    Probe* p_probe = p_module->getProbe( probe );
    if( p_probe == NULL )
    {
        net_printf( "!! module %s has no probe %s\n", module.c_str(), probe.c_str() );
        return 3;
    }

    pthread_mutex_lock( &a_consoleMutex );
    a_activeProbes.insert( p_probe );
    pthread_mutex_unlock( &a_consoleMutex );

    return 0;
}

int RobotCore::handleUnProbe( const std::vector< std::string >& rc_args )
{
    if( rc_args.size() != 3 )
    {
        net_printf( "!! usage: unprobe MODULE PROBE\n" );
        return 1;
    }

    std::string module = rc_args[ 1 ];
    std::string probe = rc_args[ 2 ];

    WildstangModule* p_module = this->resolve( module );
    if( p_module == NULL )
    {
        net_printf( "!! module %s not found\n", module.c_str() );
        return 2;
    }

    Probe* p_probe = p_module->getProbe( probe );
    if( p_probe == NULL )
    {
        net_printf( "!! module %s has no probe %s\n", module.c_str(), probe.c_str() );
        return 3;
    }

    pthread_mutex_lock( &a_consoleMutex );
    a_activeProbes.erase( p_probe );
    pthread_mutex_unlock( &a_consoleMutex );

    return 0;
}

int RobotCore::handleLsProbes( const std::vector< std::string >& rc_args )
{
    if( rc_args.size() != 2 )
    {
        net_printf( "!! usage: lsprobe MODULE\n" );
        return 1;
    }

    std::string module = rc_args[ 1 ];

    WildstangModule* p_module = this->resolve( module );
    if( p_module == NULL )
    {
        net_printf( "!! module %s not found\n", module.c_str() );
        return 2;
    }

    std::vector< std::string > probes = p_module->listProbes();
    std::vector< std::string >::iterator i = probes.begin();
    for(; i != probes.end(); ++i )
    {
        net_printf( ">> %s\n", ( *i ).c_str() );
    }
    
    return 0;
}

int RobotCore::handleActive( void )
{
    pthread_mutex_lock( &a_consoleMutex );
    std::set< Probe* >::iterator i = a_activeProbes.begin();
    for(; i != a_activeProbes.end(); ++i )
    {
        net_printf( ">> %s\n", ( *i )->getName().c_str() );
    }
    pthread_mutex_unlock( &a_consoleMutex );
    return 0;
}

int RobotCore::handleTree( void )
{
    net_printf( ">> %s\n", this->getModuleName().c_str() );
    return this->makeTree( this, 1 );
}

//
// TODO this code is duplicated in ConfigConsole.
// Maybe think about cleaning that up.
//
#define TAB_BUF_SIZE 256
int RobotCore::makeTree( WildstangModule* p_module, int idx )
{
    static const int tabstop = 2;
    char tab_buf[ TAB_BUF_SIZE ];
    memset( tab_buf, 0x00, sizeof( tab_buf ) );

    int max = TAB_BUF_SIZE - 2;
    int spaces = tabstop * idx;
    max = ( spaces < max ) ? spaces : max;

    for( int space = 0; space < max; ++space )
        tab_buf[ space ] = ' ';

    std::vector< std::string > values = p_module->listProbes();
    std::vector< std::string >::iterator j = values.begin();
    for(; j != values.end(); ++j )
    {
        net_printf( ">> %s&%s\n", tab_buf, ( *j ).c_str() );
    }

    int err = 0;

    std::vector< WildstangModule* > children = p_module->getChildren();
    std::vector< WildstangModule* >::iterator i = children.begin();
    for(; i != children.end(); ++i )
    {
        net_printf( ">> %s%s:\n", tab_buf, ( *i )->getModuleName().c_str() );
        err += this->makeTree( *i, idx + 1 );
    }

    return err;
}

