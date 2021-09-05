#ifndef __CLAW_H__
#define __CLAW_H__

#include "subsystems/Subsystem.h"
#include "io/Output.h"
#include "logger/Probe.h"
#include "utils/WsTimer.h"

typedef enum
{
    INTAKE_DIR_NEU = 0,

    INTAKE_DIR_FWD,
    INTAKE_DIR_REV,
    INTAKE_DIR_REV_SLOWER,
    INTAKE_DIR_REV_SLOWEST,
    INTAKE_DIR_REV_BALLS_TO_WALL,

    INTAKE_DIR_NUM
} intakeDirT;

typedef enum
{
    CLAW_STATE_NONE = 0,

    CLAW_STATE_RAGDOLL,
    CLAW_STATE_OPEN,
    CLAW_STATE_CLOSED,

    CLAW_STATE_NUM
} clawStateT;

class Claw : public Subsystem
{
    public:
        Claw( void );
        virtual ~Claw( void );

        virtual void init( void );
        virtual void resetState( void );
        virtual void update( void );
        virtual void selfTest( void );

        void setClawPositionOpen( void );
        void setClawPositionClosed( void );
        void setClawPositionRagdoll( void );

        void setIntakeDirectionForward( void );
        void setIntakeDirectionReverse( void );
        void setIntakeDirectionReverseSlower( void );
        void setIntakeDirectionReverseSlowest( void );
        void setIntakeDirectionNeutral( void );
        void setIntakeDirectionReverseBallsToWall( void );

        bool clawHasCube( void );

        clawStateT getState( void );

    private:
        int inputIntakeButton( bool value );
        int inputOuttakeButton( bool value );
        int inputClawOpen( bool value );
        int inputClawUnOpen( bool value );
        int inputClawDrop( bool value );
        int inputClawSpit( bool value );
        int inputClawCloseLock( bool value );
        int inputIntakePosition( bool value );
        int inputLeftSwitch( bool value );
        int inputRightSwitch( bool value );

        //
        // Config
        //
        double a_intakeSpeedReverse;
        double a_intakeSpeedReverseSlower;
        double a_intakeSpeedReverseSlowest;
        double a_intakeSpeedReverseBallsToWall;
        double a_intakeSpeedForwardLeft;
        double a_intakeSpeedForwardRight;
        bool a_reverseIntakeDirection;

        //
        // Outputs
        //
        double a_intakeLeft;
        double a_intakeRight;
        bool a_solenoidIn;
        bool a_solenoidOut;

        //
        // Inputs
        //
        bool a_leftSwitch;
        bool a_rightSwitch;
        bool a_intakeForwardButton;
        bool a_intakeReverseButton;
        bool a_intakePositionButton;

        bool a_intakeOpenLock;
        bool a_dropMomentary;
        bool a_spitMomentary;
        bool a_closeLock;

        bool a_outtaking;
        bool a_outtakingPrev;

        intakeDirT a_intakeDir;
        clawStateT a_clawState;

        ProbeValue< int > a_clawStateProbe;
        ProbeValue< int > a_intakeDirProbe;

        double a_oscillationPeriod;
        double a_oscillationFactor;
        bool a_oscillationSide;

        WsTimer a_oscillationTimer;

        void updateClawState( void );
};

#endif
