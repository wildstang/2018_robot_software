#include "AutoManager.h"

#include "utils/ws_string.h"
#include "utils/ws_file.h"
#include "auto/AutoProgram.h"
#include "auto/program/SleeperProgram.h"
#include "logger/WsLogger.h"
#include "logger/WsSmartDashboard.h"
#include "auto/script/AutoStepFactory.h"
#include "config/ConfigValueRef.h"
#include "core/RobotCore.h"

#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

AutoManager::AutoManager( AutoStepFactory* p_factory )
    : WildstangModule( std::string( "auto" ), RobotCore::getRobotCore() )
    , WsNetConsole( 1113 )
    , a_programs()
    , ap_runningProgram( NULL )
    , ap_sleeperProgram( NULL )
    , ap_queuedProgram( NULL )
    , a_autoProgramsDirectorySet( false )
    , a_directoryName()
    , a_receivingProgram()
    , a_receivingCode()
    , a_currentlyReceivingProgram( false )
    , a_reversed( false )
    , a_forceStopAtStep( 0 )
    , ap_factory( p_factory )
{
    CONFIG_VAR( "force_stop_at_step", a_forceStopAtStep, int );

    ap_sleeperProgram = new SleeperProgram();

    this->clear();
}

AutoManager::~AutoManager( void )
{
    ap_runningProgram = NULL;
    ap_queuedProgram = NULL;

    std::map< std::string, AutoProgram* >::iterator i = a_programs.begin();
    for(; i != a_programs.end(); ++i )
    {
        AutoProgram* p_program = i->second;
        delete p_program;
    }

    a_programs.clear();

    delete ap_sleeperProgram;
    ap_sleeperProgram = NULL;

    delete ap_factory;
    ap_factory = NULL;
}

void AutoManager::init( void )
{
    ap_factory->createStepFactories();
    this->definePrograms();

    this->startSocket();
}

void AutoManager::definePrograms( void )
{
    //
    // Define hardcoded programs below here
    //

    //
    // Define hardcoded programs above here
    //

    if( a_autoProgramsDirectorySet )
    {
        this->loadProgramsFromDirectory( a_directoryName );
    }
}

void AutoManager::update( void )
{
    if( ap_queuedProgram != NULL )
    {
        this->startProgram( ap_queuedProgram );
        ap_queuedProgram = NULL;
    }

    if( ap_runningProgram != NULL )
    {
        ap_runningProgram->update();

        if( ap_runningProgram->isFinished() )
        {
            ap_runningProgram->cleanup();
            ap_runningProgram = NULL;
        }
    }
}

void AutoManager::startProgram( AutoProgram* p_program )
{
    if( ap_runningProgram != NULL )
        ap_runningProgram->cleanup();

    if( p_program == NULL )
    {
        p_program = ap_sleeperProgram;
        WS_WARNING( "tried to start a null program, running sleeper" );
    }

    ap_runningProgram = p_program;

    WS_INFO( "Running Autonomous Program %s",
        ap_runningProgram->getProgramName().c_str() );
    ap_runningProgram->initialize();

    SD_PUT( "Running Autonomous Program", ap_runningProgram->getProgramName() );
}


void AutoManager::startSelectedProgram( void )
{
    AutoProgram* p_program = NULL;
    if( RobotCore::getRobotCore()->getHardware()->isAutoProgramLockedIn() )
    {
        p_program =
            RobotCore::getRobotCore()->getHardware()->getSelectedAutoProgram();

        if( p_program == NULL )
        {
            WS_WARNING( "hardware said that a null program was selected. Running sleeper." );
            p_program = ap_sleeperProgram;
        }
    }
    else
    {
        p_program = ap_sleeperProgram;
        WS_WARNING( "started autonomous without a program locked in. Running sleeper" );
    }

    this->startProgram( p_program );
}

void AutoManager::startProgramByName( std::string program_name )
{
    WS_INFO( "starting auto program %s", program_name.c_str() );

    std::map< std::string, AutoProgram* >::iterator i = a_programs.find( program_name );
    if( i == a_programs.end() )
    {
        this->startProgram( ap_sleeperProgram );
    }
    else
    {
        this->startProgram( i->second );
    }
}

void AutoManager::clear( void )
{
    if( ap_runningProgram != NULL )
    {
        ap_runningProgram->cleanup();
        ap_runningProgram = NULL;
    }

    SD_PUT( "Running Autonomous Program", "No Program Running" );
}

AutoProgram* AutoManager::getRunningProgram( void )
{
    return ap_runningProgram;
}

std::string AutoManager::getRunningProgramName( void )
{
    if( !ap_runningProgram ) return std::string( "" );
    return ap_runningProgram->getProgramName();
}

void AutoManager::addProgram( AutoProgram* p_program )
{
    if( p_program == NULL ) return;

    std::map< std::string, AutoProgram* >::iterator i =
        a_programs.find( p_program->getProgramName() );
    if( i == a_programs.end() )
    {
        a_programs[ p_program->getProgramName() ] = p_program;

        RobotCore::getRobotCore()->getHardware()->addAutoProgram( p_program );

        WS_INFO( "adding auto program %s", p_program->getProgramName().c_str() );
    }
    else
    {
        WS_ERROR( "program %s already exists", p_program->getProgramName().c_str() );
        delete p_program;
    }
}

int AutoManager::handleLine( const char* line )
{
    int err = 0;

    if( !a_currentlyReceivingProgram )
    {
        std::vector< std::string > tok = ws_strtok_sep( line, " \n\r" );
        if( tok.size() <= 0 ) return 0;

        std::string command = tok[ 0 ];

        if( 0 == strncmp( "receive", command.c_str(), 7 ) )
        {
            err = this->handleReceive( tok );
        }
        else if( 0 == strncmp( "list", command.c_str(), 4 ) )
        {
            err = this->handleList();
        }
        else if( 0 == strncmp( "send", command.c_str(), 4 ) )
        {
            err = this->handleSend( tok );
        }
        else if( 0 == strncmp( "generate", command.c_str(), 8 ) )
        {
            err = this->handleGenerate( tok );
        }
        else if( 0 == strncmp( "delete", command.c_str(), 6 ) )
        {
            err = this->handleDelete( tok );
        }
        else if( 0 == strncmp( "steps", command.c_str(), 5 ) )
        {
            err = this->handleListSteps();
        }
        else if( 0 == strncmp( "dict", command.c_str(), 4 ) )
        {
            err = this->handleDict();
        }
        else if( 0 == strncmp( "usage", command.c_str(), 5 ) )
        {
            err = this->handleUsage( tok );
        }
        else if( 0 == strncmp( "api", command.c_str(), 3 ) )
        {
            err = this->handleApi();
        }
        else if( 0 == strncmp( "commit", command.c_str(), 3 ) )
        {
            err = this->handleCommit( tok );
        }
        else if( 0 == strncmp( "help", command.c_str(), 4 ) )
        {
            net_printf( ">> receive, list, send, generate, delete, steps, usage, api, dict, commit, help\n" );
        }
        else
        {
            net_printf( "!! unrecognized command\n" );
            err = 1;
        }
    }
    else
    {
        if( 0 == strncmp( "!STOP", line, 5 ) )
        {
            a_currentlyReceivingProgram = false;
            AutoProgram* p_program = new AutoProgram( a_receivingProgram,
                                                      a_receivingCode );
            a_receivingCode.clear();

            if( p_program->checkSyntax() )
            {
                this->addProgram( p_program );
            }
            else
            {
                net_printf( "!! syntax error in program %s\n", a_receivingProgram.c_str() );
                delete p_program;
                p_program = NULL;
            }
        }
        else
        {
            a_receivingCode.append( line );
            a_receivingCode.append( "\n" );
        }
    }

    return err;
}

int AutoManager::loadProgramsFromDirectory( std::string dir_name )
{
    int err = 0;

    DIR* p_directory = opendir( dir_name.c_str() );
    if( p_directory == NULL )
    {
        WS_ERROR( "auto program directory not found %s", dir_name.c_str() );
        return 1;
    }

    struct dirent* p_entry;
    while( ( p_entry = readdir( p_directory ) ) != NULL )
    {
        std::string filename = dir_name + "/" + p_entry->d_name;
        struct stat stbuf;

        if( stat( filename.c_str(), &stbuf ) == -1 )
        {
            WS_ERROR( "cannot stat %s", filename.c_str() );
            continue;
        }

        if( ( stbuf.st_mode & S_IFMT ) == S_IFDIR )
        {
            // is a directory
            continue;
        }

        std::string auto_program_code = ws_get_file_contents_string( filename );

        const char* p_filename = filename.c_str();
        const char* p_program_name = strrchr( p_filename, '/' );
        if( p_program_name == NULL ) p_program_name = p_filename;
        else p_program_name++;

        const char* dot = strchr( p_program_name, '.' );
        if( dot )
        {
            if( 0 == strncmp( "auto", dot + 1, 4 ) )
            {
                std::string name = std::string( p_program_name )
                    .substr( 0, dot - p_program_name );

                this->addProgram( new AutoProgram( name, auto_program_code ) );
            }
        }
    }

    closedir( p_directory );

    return err;
}

int AutoManager::handleReceive( const std::vector< std::string >& rc_args )
{
    if( rc_args.size() != 2 )
    {
        net_printf( "!! expected: receive PROG_NAME\n" );
        return 1;
    }

    a_receivingProgram = rc_args[ 1 ];
    a_currentlyReceivingProgram = true;

    return 0;
}

int AutoManager::handleList( void )
{
    std::map< std::string, AutoProgram* >::iterator i = a_programs.begin();
    for(; i != a_programs.end(); ++i )
    {
        net_printf( ">> %s\n", i->first.c_str() );
    }

    return 0;
}

int AutoManager::handleSend( const std::vector< std::string >& rc_args )
{
    if( rc_args.size() != 2 )
    {
        net_printf( "!! expected: send PROG_NAME\n" );
        return 1;
    }

    std::string name = rc_args[ 1 ];

    std::map< std::string, AutoProgram* >::iterator i = a_programs.find( name );
    if( i == a_programs.end() )
    {
        net_printf( "!! program %s does not exist\n", name.c_str() );
        return 1;
    }

    std::string code = i->second->getCode();
    std::vector< std::string > lines = ws_strtok_sep( code.c_str(), "\n" );
    std::vector< std::string >::iterator j = lines.begin();
    for(; j != lines.end(); ++j )
    {
        std::string line = *j;
        net_printf( ">> %s\n", line.c_str() );
    }

    return 0;
}

int AutoManager::handleGenerate( const std::vector< std::string >& rc_args )
{
    if( rc_args.size() != 2 )
    {
        net_printf( "!! expected: generate PROG_NAME\n" );
        return 1;
    }

    std::string name = rc_args[ 1 ];

    std::map< std::string, AutoProgram* >::iterator i = a_programs.find( name );
    if( i == a_programs.end() )
    {
        net_printf( "!! program %s does not exist\n", name.c_str() );
        return 1;
    }

    std::string code = i->second->generateCode();
    std::vector< std::string > lines = ws_strtok_sep( code.c_str(), "\n" );
    std::vector< std::string >::iterator j = lines.begin();
    for(; j != lines.end(); ++j )
    {
        std::string line = *j;
        net_printf( ">> %s\n", line.c_str() );
    }

    return 0;
}

int AutoManager::handleDelete( const std::vector< std::string >& rc_args )
{
    if( rc_args.size() != 2 )
    {
        net_printf( "!! expected: delete PROG_NAME\n" );
        return 1;
    }

    std::map< std::string, AutoProgram* >::iterator i =
        a_programs.find( rc_args[ 1 ] );
    if( i != a_programs.end() )
    {
        a_programs.erase( i );
    }

    return 0;
}

int AutoManager::handleListSteps( void )
{
    std::vector< std::string > list = ap_factory->getInstalledSteps();
    std::vector< std::string >::iterator i = list.begin();
    for(; i != list.end(); ++i )
    {
        net_printf( ">> %s\n", ( *i ).c_str() );
    }

    return 0;
}

int AutoManager::handleDict( void )
{
    std::vector< std::string > v = ap_factory->listDictionaryKeys();
    std::vector< std::string >::iterator i = v.begin();
    for(; i != v.end(); ++i )
    {
        net_printf( ">> %s\n", ( *i ).c_str() );
    }

    return 0;
}

int AutoManager::handleUsage( const std::vector< std::string >& rc_args )
{
    if( rc_args.size() != 2 )
    {
        net_printf( "!! expected: usage STEP_NAME\n" );
        return 1;
    }

    std::string step = rc_args[ 1 ];
    std::string usage = ap_factory->getStepUsage( step );
    net_printf( ">> usage: %s\n", usage.c_str() );

    return 0;
}

int AutoManager::handleApi( void )
{
    std::vector< std::string > steps = ap_factory->getInstalledSteps();
    std::vector< std::string >::iterator i = steps.begin();
    for(; i != steps.end(); ++i )
    {
        std::string usage = ap_factory->getStepUsage( *i );
        net_printf( ">> %s\n", usage.c_str() );
    }

    return 0;
}

int AutoManager::handleCommit( const std::vector< std::string >& rc_args )
{
    if( rc_args.size() != 2 )
    {
        net_printf( "!! usage: commit PROGRAM_NAME\n" );
        return 1;
    }

    std::string name = rc_args[ 1 ];

    std::map< std::string, AutoProgram* >::iterator i = a_programs.find( name );
    if( i == a_programs.end() )
    {
        net_printf( "!! program %s does not exist\n", name.c_str() );
        return 1;
    }

    std::string code = i->second->getCode();
    std::string file = a_directoryName + "/" + name + ".auto";

    FILE* p_file = fopen( file.c_str(), "w" );
    if( p_file == NULL )
    {
        net_printf( "!! not able to open file %s for write\n", file.c_str() );
        return 1;
    }

    int err = 0;
    size_t bytes = fwrite( code.c_str(), 1, code.length(), p_file );
    if( bytes != code.length() )
    {
        net_printf( "!! was not able to write all %s to file\n", name.c_str() );
        err = 1;
    }

    fclose( p_file );

    sync();

    return err;
}

void AutoManager::setAutoProgramDir( std::string dir_name )
{
    a_autoProgramsDirectorySet = true;
    a_directoryName = dir_name;
}

std::vector< std::string > AutoManager::getListOfPrograms( void )
{
    std::vector< std::string > v;

    std::map< std::string, AutoProgram* >::iterator i = a_programs.begin();
    for(; i != a_programs.end(); ++i )
    {
        v.push_back( i->first );
    }

    return v;
}

void AutoManager::changeProgram( std::string program_name, bool reverse )
{
    std::map< std::string, AutoProgram* >::iterator i = a_programs.find( program_name );
    if( i != a_programs.end() )
    {
        this->changeProgram( i->second, reverse );
    }
    else
    {
        WS_ERROR( "couldn't find the program %s", program_name.c_str() );
    }
}

void AutoManager::changeProgram( AutoProgram* p_program, bool reverse )
{
    if( p_program == NULL ) return;
    a_reversed = reverse;
    ap_queuedProgram = p_program;
}

