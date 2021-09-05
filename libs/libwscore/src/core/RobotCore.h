#ifndef __ROBOTCORE_H__
#define __ROBOTCORE_H__

#include <list>
#include <pthread.h>

#include "core/WildstangModule.h"
#include "core/HardwareHandle.h"
#include "io/InputOutputFactory.h"
#include "net/WsNetConsole.h"
#include "utils/ws_debug.h"

class Outputs;
class Inputs;
class Subsystems;
class WsSmartDashboard;
class InputManager;
class OutputManager;
class SubsystemManager;
class AutoManager;
class AutoStepFactory;
class Probe;
class ConfigConsole;

class RobotCore : public WildstangModule, public WsNetConsole
{
    public:
        RobotCore( HardwareHandle* p_hardware, AutoStepFactory* p_factory );
        ~RobotCore( void );

        static RobotCore* getRobotCore( void );

        int initialize( void );
        void executeUpdate( void );

		virtual void wsRobotInit( void ) = 0;
		virtual void wsDisabledInit( void ) = 0;
		virtual void wsAutonomousInit( void ) = 0;
		virtual void wsTeleopInit( void ) = 0;
		virtual void wsTestInit( void ) = 0;

		virtual void wsRobotPeriodic( void ) = 0;
		virtual void wsDisabledPeriodic( void ) = 0;
		virtual void wsAutonomousPeriodic( void ) = 0;
		virtual void wsTeleopPeriodic( void ) = 0;
		virtual void wsTestPeriodic( void ) = 0;

        inline InputManager* getInputManager( void ) 
            { WS_ASSERT( ap_inputManager != NULL ); return ap_inputManager; }
        inline OutputManager* getOutputManager( void ) 
            { WS_ASSERT( ap_outputManager != NULL ); return ap_outputManager; }
        inline SubsystemManager* getSubsystemManager( void ) 
            { WS_ASSERT( ap_subsystemManager != NULL ); return ap_subsystemManager; }
        inline AutoManager* getAutoManager( void )
            { WS_ASSERT( ap_autoManager != NULL ); return ap_autoManager; }
        inline WsSmartDashboard* getSmartDashboard( void ) 
            { WS_ASSERT( ap_hardwareHandle != NULL );
              return ap_hardwareHandle->getSmartDashboard(); }
        inline HardwareHandle* getHardware( void ) 
            { WS_ASSERT( ap_hardwareHandle != NULL ); return ap_hardwareHandle; }

        Input* makeInput( Input* p_input );
        Output* makeOutput( Output* p_output );
        Input* makeInput( std::string name, inputTypeT type );
        Output* makeOutput( std::string name, outputTypeT type );
        inline Input* makeInput( const char* name, inputTypeT type )
            { return this->makeInput( std::string( name ), type ); }
        inline Output* makeOutput( const char* name, outputTypeT type )
            { return this->makeOutput( std::string( name ), type ); }
        Output* makeSmartOutput( std::string name, std::string in, std::string out );
        inline Output* makeSmartOutput( const char* name, 
                                        const char* in, 
                                        const char* out )
            { return this->makeSmartOutput( std::string( name ), 
                                            std::string( in ),
                                            std::string( out ) ); }

    protected:
        virtual void createOutputs( void ) = 0;
        virtual void createInputs( void ) = 0;
        virtual void createSubsystems( void ) = 0;

        virtual int handleLine( const char* line ) override;

        int handleProbe( const std::vector< std::string >& rc_args );
        int handleUnProbe( const std::vector< std::string >& rc_args );
        int handleLsProbes( const std::vector< std::string >& rc_args );
        int handleActive( void );
        int handleTree( void );

        void setConfigFileName( std::string name );
        void setAutoProgramDir( std::string dir );

    private:
        InputManager* ap_inputManager;
        OutputManager* ap_outputManager;
        SubsystemManager* ap_subsystemManager;
        AutoManager* ap_autoManager;
        ConfigConsole* ap_configConsole;
        HardwareHandle* ap_hardwareHandle;
        AutoStepFactory* ap_autoStepFactory;

        pthread_mutex_t a_consoleMutex;

        std::string a_configFile;
        std::string a_autoProgramDir;
        bool a_loadConfigFile;
        bool a_loadAutoProgramsFromFile;
        bool a_rebuildFlag;
        bool a_probesMuted;

        uint64_t a_startupTimestamp;

        int a_badConfigCheck;

        std::set< Probe* > a_activeProbes;

        inline void setRebuildFlag( void ) { a_rebuildFlag = true; }

        static RobotCore* sp_instance;

        int makeTree( WildstangModule* p_module, int idx );
};

#endif

