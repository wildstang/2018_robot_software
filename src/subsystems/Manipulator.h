#ifndef __MANIPULATOR_H__
#define __MANIPULATOR_H__

#include "subsystems/Subsystem.h"
#include "io/Output.h"

class Arm;
class Lift;

typedef enum
{
    MANIPULATOR_STATE_NONE = 0,

    MANIPULATOR_STATE_SCALE_POSITION,
    MANIPULATOR_STATE_SCALE_PLUS_ONE_POSITION,
    MANIPULATOR_STATE_SCALE_BACK_POSITION,
    MANIPULATOR_STATE_SCALE_BACK_PLUS_ONE_POSITION,
    MANIPULATOR_STATE_SWITCH_POSITION,
    MANIPULATOR_STATE_SWITCH_PLUS_ONE_POSITION,
    MANIPULATOR_STATE_CARRY_POSITION,
    MANIPULATOR_STATE_CARRY_PLUS_ONE_POSITION,
    MANIPULATOR_STATE_INTAKE_POSITION,
    MANIPULATOR_STATE_INTAKE_PLUS_ONE_POSITION,
    MANIPULATOR_STATE_INTAKE_COMPLETE_POSITION,

    MANIPULATOR_STATE_NUM
} manipulatorStateT;

std::string toString( manipulatorStateT state );

class Manipulator : public Subsystem
{
    public:
        Manipulator( void );
        virtual ~Manipulator( void );

        virtual void init( void );
        virtual void resetState( void );
        virtual void update( void );
        virtual void selfTest( void );

        int inputScalePositionButton( bool value );
        int inputScalePlusOnePositionButton( bool value );
        int inputScaleBackwardsPositionButton( bool value );
        int inputScaleBackwardsPlusOnePositionButton( bool value );
        int inputSwitchPositionButton( bool value );
        int inputSwitchPlusOnePositionButton( bool value );
        int inputCarryPositionButton( bool value );
        int inputCarryPlusOnePositionButton( bool value );
        int inputIntakePositionButton( bool value );
        int inputIntakePositionPlusOneButton( bool value );
        int inputClawLeftSwitch( bool value );
        int inputClawRightSwitch( bool value );

        void setManualMode( void );

    private:
        Arm* ap_arm;
        Lift* ap_lift;

        manipulatorStateT a_state;
        manipulatorStateT a_statePrev;

        bool a_deployedArm;

        bool a_clawLeftSwitch;
        bool a_clawRightSwitch;

        bool isScale( void );
};

#endif
