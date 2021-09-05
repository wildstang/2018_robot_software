#ifndef __WSROBOT_H__
#define __WSROBOT_H__

#include "core/RobotCore.h"
#include "utils/ws_debug.h"
#include "utils/WsTimer.h"

class Monitor;
class Drive;
class Arm;
class Lift;
class Claw;
class Manipulator;
class LED;
class AutoChooser;

class WsRobot : public RobotCore
{
    public:
        WsRobot( void );
        virtual ~WsRobot( void );

        virtual void wsRobotInit( void ) override;
        virtual void wsDisabledInit( void ) override;
        virtual void wsAutonomousInit( void ) override;
        virtual void wsTeleopInit( void ) override;
        virtual void wsTestInit( void ) override;

        virtual void wsRobotPeriodic( void ) override;
        virtual void wsDisabledPeriodic( void ) override;
        virtual void wsAutonomousPeriodic( void ) override;
        virtual void wsTeleopPeriodic( void ) override;
        virtual void wsTestPeriodic( void ) override;

        inline Monitor* getMonitor( void )
            { WS_ASSERT( ap_monitor != NULL ); return ap_monitor; }
        inline Drive* getDrive( void )
            { WS_ASSERT( ap_drive != NULL ); return ap_drive; }
        inline Arm* getArm( void )
            { WS_ASSERT( ap_arm != NULL ); return ap_arm; }
        inline Lift* getLift( void )
            { WS_ASSERT( ap_lift != NULL ); return ap_lift; }
        inline Claw* getClaw( void )
            { WS_ASSERT( ap_claw != NULL ); return ap_claw; }
        inline Manipulator* getManipulator( void )
            { WS_ASSERT( ap_manipulator != NULL ); return ap_manipulator; }
        inline LED* getLED( void )
            { WS_ASSERT( ap_led != NULL ); return ap_led; }
        inline AutoChooser* getAutoChooser( void )
            { WS_ASSERT( ap_autoChooser != NULL ); return ap_autoChooser; }

        static WsRobot* getInstance( void );

    protected:
        virtual void createOutputs( void );
        virtual void createInputs( void );
        virtual void createSubsystems( void );

        int startAutonomousProgram( void );

    private:

        /**
         * This should be called to reset any robot state between runs, without
         * having to restart robot code.
         *
         */
        void resetRobotState( void );

        //
        // variables
        //

        static WsRobot* sp_instance;

        long a_lastCycleTime;

        bool a_firstDisabled;
        bool a_firstRun;

        std::string a_autoProgramHardcodeName;
        bool a_useHardcodedProgram;

        bool a_autoProgramStarted;

        Monitor* ap_monitor;
        Drive* ap_drive;
        Arm* ap_arm;
        Lift* ap_lift;
        Claw* ap_claw;
        Manipulator* ap_manipulator;
        LED* ap_led;
        AutoChooser* ap_autoChooser;

        WsTimer a_autoDelayTimer;
};

#endif
