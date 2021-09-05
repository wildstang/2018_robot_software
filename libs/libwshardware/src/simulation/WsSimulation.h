#ifndef __WSSIMULATION_H__
#define __WSSIMULATION_H__

#include <map>
#include <vector>
#include <string>
#include <deque>

#include <pthread.h>

class WsSimulatedInput;
class WsSimulatedOutput;
class RobotCore;

typedef enum
{
    SIMULATION_STATE_NONE = 0,

    SIMULATION_STATE_DISABLED,
    SIMULATION_STATE_TELEOP,
    SIMULATION_STATE_AUTONOMOUS,
    SIMULATION_STATE_TEST,

    SIMULATION_STATE_NUM
} simulationStateT;

typedef enum
{
    SIMULATION_COMMAND_TYPE_NONE = 0,

    SIMULATION_COMMAND_TYPE_INPUT_VALUE,
    SIMULATION_COMMAND_TYPE_LS,
    SIMULATION_COMMAND_TYPE_MODE,
    SIMULATION_COMMAND_TYPE_MUTE,
    SIMULATION_COMMAND_TYPE_AUTO_PROGRAM,

    SIMULATION_COMMAND_TYPE_NUM
} simulationCommandTypeT;

typedef struct
{
    simulationCommandTypeT type;
    std::vector< std::string > args;
} simulationCommandT;

simulationStateT parseSimulationState( std::string str );
std::string toString( simulationStateT state );

class WsSimulation
{
    public:
        WsSimulation( RobotCore* p_bot );
        virtual ~WsSimulation( void );

        static WsSimulation* getInstance( void ); // bad hack

        void addSimulatedInput( WsSimulatedInput* p_input );
        void addSimulatedOutput( WsSimulatedOutput* p_output );

        std::string getAutoProgramName( void );

        int run( void );
        void quit( void );
        void disablePrints( void );

    protected:
        bool a_quit;
        bool a_running;
        std::map< std::string, WsSimulatedInput* > a_inputs;
        std::map< std::string, WsSimulatedOutput* > a_outputs;
        std::string a_autoProgram;
        RobotCore* ap_bot;
        bool a_printsDisabled;

        pthread_t a_thread;
        pthread_mutex_t a_mutex;
        unsigned int a_sleepInterval;

        std::deque< simulationCommandT > a_commands;
        simulationStateT a_state;
        simulationStateT a_statePrev;

        WsSimulatedInput* getInput( std::string name );
        WsSimulatedOutput* getOutput( std::string name );

        int popCommands( void );
        int runCommand( simulationCommandT* p_command );

        // following are in robot thread.
        int handleInput( const std::vector< std::string >& rc_args );
        int handleLs( const std::vector< std::string >& rc_args );
        int handleMode( const std::vector< std::string >& rc_args );
        int handleMute( const std::vector< std::string >& rc_args );

    private:
        WsSimulation( const WsSimulation& rc_rhs );

        static WsSimulation* sp_instance;

        static void* consoleThreadEntry( void* arg );
        void consoleThreadRun( void );

        int handleLine( char* line ); // in console thread
};

#endif

