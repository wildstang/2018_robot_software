#ifndef __LIFT_H__
#define __LIFT_H__

#include "subsystems/Subsystem.h"
#include "io/Output.h"

#include "logger/Probe.h"

class Arm;

typedef enum
{
    LIFT_STATE_NONE = 0,

    LIFT_STATE_MANUAL_NOMINAL,
    LIFT_STATE_MANUAL_ARM_BLOCK,
    LIFT_STATE_MANUAL_BOTTOM_SW_BLOCK,
    LIFT_STATE_MANUAL_MAX_HEIGHT_BLOCK,
    LIFT_STATE_PID_NOMINAL,
    LIFT_STATE_PID_ARM_BLOCK,
    LIFT_STATE_PID_BRAKE_ARM_BLOCK,
    LIFT_STATE_PID_BOTTOM_SW_BLOCK,
    LIFT_STATE_PID_MAX_HEIGHT_BLOCK,
    LIFT_STATE_BRAKE_APPLIED,

    LIFT_STATE_NUM
} liftStateT;

std::string toString( liftStateT state );
bool isManual( liftStateT state );
bool isPid( liftStateT state );
bool isBrake( liftStateT state );

class Lift : public Subsystem
{
    public:
        Lift( void );
        virtual ~Lift( void );

        virtual void init( void );
        virtual void resetState( void );
        virtual void update( void );
        virtual void selfTest( void );

        void setIntakePosition( void );
        void setIntakePlusOnePosition( void );
        void setCarryPosition( void );
        void setCarryPlusOnePosition( void );
        void setSwitchPosition( void );
        void setSwitchPlusOnePosition( void );
        void setScalePosition( void );
        void setScalePlusOnePosition( void );
        void setScaleReversePosition( void );
        void setScaleReversePlusOnePosition( void );
        void setShootScalePosition( void );
        void setShootScaleReversePosition( void );

        bool inArmSafeZone( void );

        bool isStabilized( void );
        bool isInRange( void );

    private:
        bool a_brakeDeployed;
        double a_softResetSensorPosition;

        liftStateT a_state;
        liftStateT a_statePrev;

        Arm* ap_arm;

        double a_desiredPosition;
        double a_desiredSpeed;
        bool a_climbMode;

        //
        // Configuration
        //
        double a_setPointIntake;
        double a_setPointIntakePlusOne;
        double a_setPointCarry;
        double a_setPointCarryPlusOne;
        double a_setPointSwitch;
        double a_setPointSwitchPlusOne;
        double a_setPointScale;
        double a_setPointScalePlusOne;
        double a_setPointScaleReverse;
        double a_setPointScaleReversePlusOne;
        double a_setPointShootScale;
        double a_setPointShootScaleReverse;
        double a_maximumHeight;
        double a_safetyMargin;
        double a_minimumSafeArmHeight;
        double a_maxSpeedPercent;
        double a_brakeSpeedThresh;
        double a_inRangeEpsilon;

        bool a_reverseManualSafety;
        bool a_reverseSensorSafety;
        bool a_reverseBrakeLogic;

        //
        // Cached input values
        //
        bool a_pidStabilized;
        bool a_bottomSafetySwitch;
        double a_currentPosition;
        double a_currentVelocity;
        double a_joystickValue;

        //
        // Outputs:
        //
        smartOutputCmdT a_winchTalonStruct;
        smartOutputCmdT a_winchFollowerStruct;

        int a_brakeDeployedOutput;

        ProbeValue< std::string > a_stateProbe;
        ProbeValue< double > a_currentPositionProbe;
        ProbeValue< double > a_desiredPositionProbe;
        ProbeValue< double > a_desiredSpeedProbe;
        ProbeValue< bool > a_isPidProbe;
        ProbeValue< bool > a_isManualProbe;
        ProbeValue< bool > a_isBrakeProbe;
        ProbeValue< bool > a_brakeDeployedProbe;

        //
        // Inputs:
        //
        int inputManualSpeed( double value );
        int inputPidStabilized( bool value );
        int inputBottomSwitch( bool value );
        int inputCurrentPosition( double value );
        int inputCurrentVelocity( double velocity );
        int inputClimbButton( bool value );
};

#endif
