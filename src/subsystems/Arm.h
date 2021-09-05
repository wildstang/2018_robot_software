#ifndef __ARM_H__
#define __ARM_H__

#include "subsystems/Subsystem.h"
#include "io/Output.h"

class Lift;

typedef enum
{
    ARM_STATE_NONE = 0,

    ARM_STATE_MANUAL_NOMINAL,
    ARM_STATE_MANUAL_LIFT_BLOCK,
    ARM_STATE_MANUAL_BOTTOM_SW_BLOCK,
    ARM_STATE_MANUAL_TOP_SW_BLOCK,
    ARM_STATE_PID_NOMINAL,
    ARM_STATE_PID_LIFT_BLOCK,
    ARM_STATE_PID_BOTTOM_SW_BLOCK,
    ARM_STATE_PID_TOP_SW_BLOCK,

    ARM_STATE_NUM
} armStateT;

std::string toString( armStateT state );
bool isManual( armStateT state );
bool isPid( armStateT state );

class Arm : public Subsystem
{
    public:
        Arm( void );
        virtual ~Arm( void );

        virtual void init( void );
        virtual void resetState( void );
        virtual void selfTest( void );
        virtual void update( void );

        void setIntakePosition( void );
        void setIntakePlusOnePosition( void );
        void setIntakeCompletePosition( void );
        void setScoreSwitchPosition( void );
        void setScoreSwitchPlusOnePosition( void );
        void setScoreScalePosition( void );
        void setScoreScalePlusOnePosition( void );
        void setScoreReversePosition( void );
        void setCarryPosition( void );
        void setShootScalePosition( void );
        void setShootScaleReversePosition( void );

        bool inLiftSafeZone( void );
        bool isStabilized( void );

    private:
        armStateT a_state;
        armStateT a_statePrev;

        Lift* ap_lift;

        double a_desiredPosition;
        double a_desiredSpeed;
        bool a_climbMode;

        //
        // Configuration
        //
        double a_setPointIntake;
        double a_setPointIntakePlusOne;
        double a_setPointIntakeComplete;
        double a_setPointScoreScale;
        double a_setPointScoreScalePlusOne;
        double a_setPointScoreSwitch;
        double a_setPointScoreSwitchPlusOne;
        double a_setPointScoreReverse;
        double a_setPointCarry;
        double a_setPointShootScale;
        double a_setPointShootScaleReverse;

        double a_maxSpeedPercent;
        double a_maxLiftSafeAngle;
        double a_safetyMargin;
        double a_stabilizedEpsilon;
        bool a_reverseManualSafety;
        bool a_reverseSensorSafety;

        //
        // Input cache values
        //
        bool a_bottomSafetySwitch;
        bool a_topSafetySwitch;
        double a_currentPosition;

        //
        // Outputs
        //
        smartOutputCmdT a_armTalonStruct;
        smartOutputCmdT a_armFollowerStruct;

        //
        // Inputs
        //
        int inputBottomSwitch( bool value );
        int inputTopSwitch( bool value );
        int inputManualSpeed( double value );
        int inputCurrentPosition( double value );
        int inputClimbButton( bool value );
};

#endif
