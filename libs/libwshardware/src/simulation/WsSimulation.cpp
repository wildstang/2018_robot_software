#include "WsSimulation.h"

#include <cstring>
#include <unistd.h>

#include "utils/ws_string.h"
#include "utils/ws_debug.h"

#include "inputs/WsSimulatedInput.h"
#include "outputs/WsSimulatedOutput.h"
#include "io/InputManager.h"
#include "io/OutputManager.h"

#define SIMULATION_BUFF_LENGTH 2048

WsSimulation* WsSimulation::sp_instance = NULL;

WsSimulation::WsSimulation( RobotCore* p_bot )
    : a_quit( false )
    , a_running( false )
    , a_inputs()
    , a_outputs()
    , a_autoProgram()
    , ap_bot( p_bot )
    , a_printsDisabled( false )
    , a_thread( 0 )
    , a_mutex( PTHREAD_MUTEX_INITIALIZER )
    , a_sleepInterval( 16666 )
    , a_commands()
    , a_state( SIMULATION_STATE_NONE )
    , a_statePrev( SIMULATION_STATE_NONE )
{
    WS_ASSERT( sp_instance == NULL );
    sp_instance = this;

    // TODO: slow things down for now
    a_sleepInterval = 1000000;

    pthread_mutex_init( &a_mutex, NULL );
}

WsSimulation::WsSimulation( const WsSimulation& rc_rhs )
{
}

WsSimulation::~WsSimulation( void )
{
    this->quit();

    pthread_join( a_thread, NULL );
    pthread_mutex_destroy( &a_mutex );

    delete ap_bot;
    ap_bot = NULL;

    WS_ASSERT( sp_instance == this );
    sp_instance = NULL;
}

int WsSimulation::run( void )
{
    pthread_create( &a_thread, NULL, consoleThreadEntry, this );
    a_running = true;

    ap_bot->initialize();
    ap_bot->wsRobotInit();

    while( !a_quit ) // atomic read
    {
        this->popCommands();

        simulationStateT next = a_state;
        switch( a_state )
        {
            case SIMULATION_STATE_DISABLED:
                if( a_state != a_statePrev )
                {
                    ap_bot->wsDisabledInit();
                }
                ap_bot->wsDisabledPeriodic();
                break;
            case SIMULATION_STATE_TELEOP:
                if( a_state != a_statePrev )
                {
                    ap_bot->wsTeleopInit();
                }
                ap_bot->wsTeleopPeriodic();
                break;
            case SIMULATION_STATE_AUTONOMOUS:
                if( a_state != a_statePrev )
                {
                    ap_bot->wsAutonomousInit();
                }
                ap_bot->wsAutonomousPeriodic();
                break;
            case SIMULATION_STATE_TEST:
                if( a_state != a_statePrev )
                {
                    ap_bot->wsTestInit();
                }
                ap_bot->wsTestPeriodic();
                break;
            case SIMULATION_STATE_NUM:
            case SIMULATION_STATE_NONE:
            default:
                WS_ERROR( "robot in a bad state, switching to disabled" );
                next = SIMULATION_STATE_DISABLED;
                break;
        }

        ap_bot->wsRobotPeriodic();
        ap_bot->executeUpdate();

        a_statePrev = a_state;
        a_state = next;

        usleep( a_sleepInterval );
    }

    return 0;
}

int WsSimulation::handleLine( char* line )
{
    int err = 0;

    simulationCommandT cmd;
    cmd.type = SIMULATION_COMMAND_TYPE_NONE;
    cmd.args = ws_strtok_sep( line, " \n" );
    if( cmd.args.size() <= 0 ) return 0;

    const char* command = cmd.args[ 0 ].c_str();

    if( 0 == strncmp( command, "input", 5 ) )
    {
        // change an input value
        cmd.type = SIMULATION_COMMAND_TYPE_INPUT_VALUE;
    }
    else if( 0 == strncmp( command, "ls", 2 ) )
    {
        // list the inputs/outputs
        cmd.type = SIMULATION_COMMAND_TYPE_LS;
    }
    else if( 0 == strncmp( command, "mode", 4 ) )
    {
        // select disable, teleoperated, or autonomous
        cmd.type = SIMULATION_COMMAND_TYPE_MODE;
    }
    else if( 0 == strncmp( command, "mute", 4 ) )
    {
        // select which outputs are displayed
        cmd.type = SIMULATION_COMMAND_TYPE_MUTE;
    }
    else if( 0 == strncmp( command, "program", 7 ) )
    {
        // set the auto program name
        cmd.type = SIMULATION_COMMAND_TYPE_AUTO_PROGRAM;
    }
    else if( 0 == strncmp( command, "quit", 4 ) )
    {
        this->quit();
        // quit the program
    }
    else if( 0 == strncmp( command, "help", 4 ) )
    {
        // list the commands
        printf( ">> input, ls, mode, mute, quit, program, help\n" );
        printf( "** ok\n" );
    }
    else
    {
        // d'oh
        printf( "!! unrecognized command\n" );
        printf( "** fail\n" );
        err = 1;
    }

    if( cmd.type != SIMULATION_COMMAND_TYPE_NONE )
    {
        pthread_mutex_lock( &a_mutex );
        a_commands.push_back( cmd );
        pthread_mutex_unlock( &a_mutex );
    }

    return err;
}

int WsSimulation::handleInput( const std::vector< std::string >& rc_args )
{
    int err = 0;

    if( rc_args.size() != 3 )
    {
        printf( "!! expected: input INPUT VALUE\n" );
        return 1;
    }

    std::string input = rc_args[ 1 ];
    std::string value = rc_args[ 2 ];

    WsSimulatedInput* p_input = this->getInput( input );
    if( p_input == NULL )
    {
        printf( "!! no input %s\n", input.c_str() );
        return 1;
    }

    err += p_input->setSimulatedValueFromString( value );

   return err;
}

int WsSimulation::handleLs( const std::vector< std::string >& rc_args )
{
    int err = 0;
    std::vector< std::string > v;
    std::vector< std::string >::iterator i;

    printf( ">># INPUTS\n" );
    v = RobotCore::getRobotCore()->getInputManager()->listInputs();
    for( i = v.begin(); i != v.end(); ++i )
    {
        printf( ">> %s\n", ( *i ).c_str() );
    }

    printf( ">># OUTPUTS\n" );
    v = RobotCore::getRobotCore()->getOutputManager()->listOutputs();
    for( i = v.begin(); i != v.end(); ++i )
    {
        printf( ">> %s\n", ( *i ).c_str() );
    }

    return err;
}

int WsSimulation::handleMode( const std::vector< std::string >& rc_args )
{
    int err = 0;

    if( rc_args.size() != 2 )
    {
        printf( "!! expected: mode [disabled/teleop/autonomous/test]\n" );
        return 1;
    }

    a_state = parseSimulationState( rc_args[ 1 ] );

    return err;
}

int WsSimulation::handleMute( const std::vector< std::string >& rc_args )
{
    int err = 0;

    if( rc_args.size() != 3 )
    {
        printf( "!! expected: mute OUTPUT 1/0\n" );
        return 1;
    }

    std::string output = rc_args[ 1 ];
    int enabled = atoi( rc_args[ 2 ].c_str() );

    WsSimulatedOutput* p_output = this->getOutput( output );
    if( p_output == NULL )
    {
        printf( "!! no output %s\n", output.c_str() );
        return 1;
    }

    p_output->setEnabled( enabled != 0 );

    return err;
}

WsSimulation* WsSimulation::getInstance( void )
{
    WS_ASSERT( sp_instance != NULL );
    return sp_instance;
}

void WsSimulation::addSimulatedInput( WsSimulatedInput* p_input )
{
    if( p_input == NULL ) return;

    WS_TRACE( "adding simulated input %s", 
              p_input->getSimulatedName().c_str() );

    std::map< std::string, WsSimulatedInput* >::iterator i = 
        a_inputs.find( p_input->getSimulatedName() );
    WS_ASSERT( i == a_inputs.end() );

    a_inputs[ p_input->getSimulatedName() ] = p_input;
} 

WsSimulatedInput* WsSimulation::getInput( std::string name )
{
    std::map< std::string, WsSimulatedInput* >::iterator i =
        a_inputs.find( name );

    if( i != a_inputs.end() )
    {
        return i->second;
    }

    return NULL;
}

void WsSimulation::addSimulatedOutput( WsSimulatedOutput* p_output )
{
    if( p_output == NULL ) return;

    if( a_printsDisabled ) p_output->setEnabled( false );

    WS_TRACE( "adding simulated output %s",
              p_output->getSimulatedName().c_str() );

    std::map< std::string, WsSimulatedOutput* >::iterator i = 
        a_outputs.find( p_output->getSimulatedName() );
    WS_ASSERT( i == a_outputs.end() );

    a_outputs[ p_output->getSimulatedName() ] = p_output;
} 

WsSimulatedOutput* WsSimulation::getOutput( std::string name )
{
    std::map< std::string, WsSimulatedOutput* >::iterator i =
        a_outputs.find( name );

    if( i != a_outputs.end() )
    {
        return i->second;
    }

    return NULL;
}

std::string WsSimulation::getAutoProgramName( void )
{
    return a_autoProgram;
}

void* WsSimulation::consoleThreadEntry( void* arg )
{
    if( arg == NULL ) return NULL;
    WsSimulation* p_sim = ( WsSimulation* )arg;
    p_sim->consoleThreadRun();
    return NULL;
}

void WsSimulation::consoleThreadRun( void )
{
    char buf[ SIMULATION_BUFF_LENGTH ];
    memset( buf, 0x00, sizeof( buf ) );

    while( NULL != fgets( buf, SIMULATION_BUFF_LENGTH - 1, stdin ) )
    {
        this->handleLine( buf );

        if( a_quit ) break;
    }
}

void WsSimulation::quit( void )
{
    a_quit = true; // atomic write
}

int WsSimulation::popCommands( void )
{
    int err = 0;

    pthread_mutex_lock( &a_mutex );
    while( !( a_commands.empty() ) )
    {
        simulationCommandT cmd = a_commands.front();
        a_commands.pop_front();

        err += this->runCommand( &cmd );

        if( err )
        {
            WS_ERROR( "bad things happened with the command %s",
                      toString( cmd.type ).c_str() );
            // do not break out, finish all commands.
        }
    }
    pthread_mutex_unlock( &a_mutex );

    return err;
}

int WsSimulation::runCommand( simulationCommandT* p_command )
{
    if( p_command == NULL ) 
    {
        WS_ERROR( "command was null" );
        return 1;
    }

    int err = 0;

    simulationCommandTypeT cmd = p_command->type;
    if( cmd == SIMULATION_COMMAND_TYPE_INPUT_VALUE )
    {
        // change an input value
        err = this->handleInput( p_command->args );
    }
    else if( cmd == SIMULATION_COMMAND_TYPE_LS )
    {
        // list the inputs/outputs
        err = this->handleLs( p_command->args );
    }
    else if( cmd == SIMULATION_COMMAND_TYPE_MODE )
    {
        // select disable, teleoperated, or autonomous
        err = this->handleMode( p_command->args );
    }
    else if( cmd == SIMULATION_COMMAND_TYPE_MUTE )
    {
        // select which outputs are displayed
        err = this->handleMute( p_command->args );
    }
    else if( cmd == SIMULATION_COMMAND_TYPE_AUTO_PROGRAM )
    {
        if( p_command->args.size() != 2 )
        {
            printf( "!! expected auto program name\n" );
            err = 1;
        }
        else
        {
            a_autoProgram = p_command->args[ 1 ];
        }
    }
    else
    {
        // should never happen.
        WS_ASSERT( false );
    }

    if( !err )
    {
        printf( "** ok\n" );
    }
    else
    {
        printf( "** fail\n" );
    }

    return err;
}

void WsSimulation::disablePrints( void )
{
    a_printsDisabled = true;
}

simulationStateT parseSimulationState( std::string str )
{
    simulationStateT state;
    ws_to_lower( str );

    if( 0 == strncmp( "dis", str.c_str(), 3 ) )
    {
        state = SIMULATION_STATE_DISABLED;
    }
    else if( 0 == strncmp( "tel", str.c_str(), 3 ) )
    {
        state = SIMULATION_STATE_TELEOP;
    }
    else if( 0 == strncmp( "aut", str.c_str(), 3 ) )
    {
        state = SIMULATION_STATE_AUTONOMOUS;
    }
    else if( 0 == strncmp( "tes", str.c_str(), 3 ) )
    {
        state = SIMULATION_STATE_TEST;
    }
    else
    {
        state = SIMULATION_STATE_NONE;
    }

    return state;
}

std::string toString( simulationStateT state )
{
    std::string str;

    switch( state )
    {
        case SIMULATION_STATE_DISABLED:
            str = std::string( "disabled" );
            break;
        case SIMULATION_STATE_TELEOP:
            str = std::string( "teleop" );
            break;
        case SIMULATION_STATE_AUTONOMOUS:
            str = std::string( "autonomous" );
            break;
        case SIMULATION_STATE_TEST:
            str = std::string( "test" );
            break;
        case SIMULATION_STATE_NONE:
        case SIMULATION_STATE_NUM:
        default:
            str = std::string( "undefined" );
            break;
    }

    return str;
}

