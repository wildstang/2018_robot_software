#include "Manipulator.h"

#include "robot/WsRobot.h"
#include "subsystems/Lift.h"
#include "subsystems/Arm.h"
#include "io/routing/InputSlot.h"

Manipulator::Manipulator( void )
    : Subsystem( std::string( "manipulator" ) )
    , ap_arm( NULL )
    , ap_lift( NULL )
    , a_state( MANIPULATOR_STATE_NONE )
    , a_statePrev( MANIPULATOR_STATE_NONE )
    , a_deployedArm( true )
    , a_clawLeftSwitch( false )
    , a_clawRightSwitch( false )
{
}

Manipulator::~Manipulator( void )
{
}

void Manipulator::init( void )
{
    ap_arm = WsRobot::getInstance()->getArm();
    ap_lift = WsRobot::getInstance()->getLift();

    CONNECT_INPUT( "scale_normal_position_button", inputScalePositionButton, Manipulator, bool );
    CONNECT_INPUT( "scale_plus_one_position_button", inputScalePlusOnePositionButton, Manipulator, bool );
    CONNECT_INPUT( "scale_reverse_normal_position_button", inputScaleBackwardsPositionButton, Manipulator, bool );
    CONNECT_INPUT( "scale_reverse_plus_one_position_button", inputScaleBackwardsPlusOnePositionButton, Manipulator, bool );
    CONNECT_INPUT( "switch_normal_position_button", inputSwitchPositionButton, Manipulator, bool );
    CONNECT_INPUT( "switch_plus_one_position_button", inputSwitchPlusOnePositionButton, Manipulator, bool );
    CONNECT_INPUT( "carry_normal_position_button", inputCarryPositionButton, Manipulator, bool );
    CONNECT_INPUT( "carry_plus_one_position_button", inputCarryPlusOnePositionButton, Manipulator, bool );
    CONNECT_INPUT( "intake_normal_position_button", inputIntakePositionButton, Manipulator, bool );
    CONNECT_INPUT( "intake_plus_one_position_button", inputIntakePositionPlusOneButton, Manipulator, bool );

    CONNECT_INPUT( "claw_left_switch", inputClawLeftSwitch, Manipulator, bool );
    CONNECT_INPUT( "claw_right_switch", inputClawRightSwitch, Manipulator, bool );
}

void Manipulator::resetState( void )
{
    a_state = MANIPULATOR_STATE_NONE;
    a_deployedArm = true;
}

void Manipulator::update( void )
{
    switch( a_state )
    {
        case MANIPULATOR_STATE_SCALE_POSITION:
            if( a_state != a_statePrev )
            {
                ap_lift->setScalePosition();
                if( isScale() )
                {
                    ap_arm->setScoreScalePosition();
                    a_deployedArm = true;
                }
                else
                {
                    ap_arm->setCarryPosition();
                    a_deployedArm = false;
                }
            }

            if( !a_deployedArm )
            {
                a_deployedArm = ap_lift->isInRange();
                if( a_deployedArm )
                {
                    ap_arm->setScoreScalePosition();
                }
            }
            break;

        case MANIPULATOR_STATE_SCALE_PLUS_ONE_POSITION:
            if( a_state != a_statePrev )
            {
                ap_lift->setScalePlusOnePosition();
                if( isScale() )
                {
                    ap_arm->setScoreScalePlusOnePosition();
                    a_deployedArm = true;
                }
                else
                {
                    ap_arm->setCarryPosition();
                    a_deployedArm = false;
                }
                a_deployedArm = false;
            }

            if( !a_deployedArm )
            {
                a_deployedArm = ap_lift->isInRange();
                if( a_deployedArm )
                {
                    ap_arm->setScoreScalePlusOnePosition();
                }
            }
            break;

        case MANIPULATOR_STATE_SCALE_BACK_POSITION:
            if( a_state != a_statePrev )
            {
                ap_lift->setScaleReversePosition();
                if( isScale() )
                {
                    ap_arm->setScoreReversePosition();
                    a_deployedArm = true;
                }
                else
                {
                    ap_arm->setCarryPosition();
                    a_deployedArm = false;
                }
                a_deployedArm = false;
            }

            if( !a_deployedArm )
            {
                a_deployedArm = ap_lift->isInRange();
                if( a_deployedArm )
                {
                    ap_arm->setScoreReversePosition();
                }
            }
            break;

        case MANIPULATOR_STATE_SCALE_BACK_PLUS_ONE_POSITION:
            if( a_state != a_statePrev )
            {
                ap_lift->setScaleReversePlusOnePosition();
                if( isScale() )
                {
                    ap_arm->setScoreReversePosition();
                    a_deployedArm = true;
                }
                else
                {
                    ap_arm->setCarryPosition();
                    a_deployedArm = false;
                }
                a_deployedArm = false;
            }

            if( !a_deployedArm )
            {
                a_deployedArm = ap_lift->isInRange();
                if( a_deployedArm )
                {
                    ap_arm->setScoreReversePosition();
                }
            }
            break;

        case MANIPULATOR_STATE_SWITCH_POSITION:
            if( a_state != a_statePrev )
            {
                ap_lift->setSwitchPosition();
                if( !isScale() )
                {
                    ap_arm->setScoreSwitchPosition();
                    a_deployedArm = true;
                }
                else
                {
                    ap_arm->setCarryPosition();
                    a_deployedArm = false;
                }
                a_deployedArm = false;
            }

            if( !a_deployedArm )
            {
                a_deployedArm = ap_lift->isInRange();
                if( a_deployedArm )
                {
                    ap_arm->setScoreSwitchPosition();
                }
            }
            break;

        case MANIPULATOR_STATE_SWITCH_PLUS_ONE_POSITION:
            if( a_state != a_statePrev )
            {
                ap_lift->setSwitchPlusOnePosition();
                if( !isScale() )
                {
                    ap_arm->setScoreSwitchPlusOnePosition();
                    a_deployedArm = true;
                }
                else
                {
                    ap_arm->setCarryPosition();
                    a_deployedArm = false;
                }
                a_deployedArm = false;
            }

            if( !a_deployedArm )
            {
                a_deployedArm = ap_lift->isInRange();
                if( a_deployedArm )
                {
                    ap_arm->setScoreSwitchPlusOnePosition();
                }
            }
            break;

        case MANIPULATOR_STATE_CARRY_POSITION:
            if( a_state != a_statePrev )
            {
                ap_lift->setCarryPosition();
                ap_arm->setCarryPosition();
            }
            break;

        case MANIPULATOR_STATE_CARRY_PLUS_ONE_POSITION:
            if( a_state != a_statePrev )
            {
                ap_lift->setCarryPlusOnePosition();
                ap_arm->setCarryPosition();
            }
            break;

        case MANIPULATOR_STATE_INTAKE_POSITION:
            if( a_state != a_statePrev )
            {
                ap_lift->setIntakePosition();
                if( !isScale() )
                {
                    ap_arm->setIntakePosition();
                    a_deployedArm = true;
                }
                else
                {
                    ap_arm->setCarryPosition();
                    a_deployedArm = false;
                }
                a_deployedArm = false;
            }

            if( !a_deployedArm )
            {
                a_deployedArm = ap_lift->isInRange();
                if( a_deployedArm )
                {
                    ap_arm->setIntakePosition();
                }
            }
            break;

        case MANIPULATOR_STATE_INTAKE_PLUS_ONE_POSITION:
            if( a_state != a_statePrev )
            {
                ap_lift->setIntakePlusOnePosition();
                if( !isScale() )
                {
                    ap_arm->setIntakePlusOnePosition();
                    a_deployedArm = true;
                }
                else
                {
                    ap_arm->setCarryPosition();
                    a_deployedArm = false;
                }
                a_deployedArm = false;
            }

            if( !a_deployedArm )
            {
                a_deployedArm = ap_lift->isInRange();
                if( a_deployedArm )
                {
                    ap_arm->setIntakePlusOnePosition();
                }
            }
            break;

        case MANIPULATOR_STATE_INTAKE_COMPLETE_POSITION:
            if( a_state != a_statePrev )
            {
                ap_lift->setIntakePosition();
                if( !isScale() )
                {
                    ap_arm->setIntakeCompletePosition();
                    a_deployedArm = true;
                }
                else
                {
                    ap_arm->setCarryPosition();
                    a_deployedArm = false;
                }
                a_deployedArm = false;
            }

            if( !a_deployedArm )
            {
                a_deployedArm = ap_lift->isInRange();
                if( a_deployedArm )
                {
                    ap_arm->setIntakeCompletePosition();
                }
            }
            break;

        case MANIPULATOR_STATE_NONE:
        case MANIPULATOR_STATE_NUM:
        default:
            break;
    }

    a_statePrev = a_state;
}

void Manipulator::selfTest( void )
{
}

int Manipulator::inputScalePositionButton( bool value )
{
    if( value )
    {
        a_state = MANIPULATOR_STATE_SCALE_POSITION;
        a_deployedArm = false;
    }

    return 0;
}

int Manipulator::inputScalePlusOnePositionButton( bool value )
{
    if( value )
    {
        a_state = MANIPULATOR_STATE_SCALE_PLUS_ONE_POSITION;
        a_deployedArm = false;
    }

    return 0;
}

int Manipulator::inputScaleBackwardsPositionButton( bool value )
{
    if( value )
    {
        a_state = MANIPULATOR_STATE_SCALE_BACK_POSITION;
        a_deployedArm = false;
    }

    return 0;
}

int Manipulator::inputScaleBackwardsPlusOnePositionButton( bool value )
{
    if( value )
    {
        a_state = MANIPULATOR_STATE_SCALE_BACK_PLUS_ONE_POSITION;
        a_deployedArm = false;
    }

    return 0;
}

int Manipulator::inputSwitchPositionButton( bool value )
{
    if( value )
    {
        a_state = MANIPULATOR_STATE_SWITCH_POSITION;
        a_deployedArm = false;
    }

    return 0;
}

int Manipulator::inputSwitchPlusOnePositionButton( bool value )
{
    if( value )
    {
        a_state = MANIPULATOR_STATE_SWITCH_PLUS_ONE_POSITION;
        a_deployedArm = false;
    }

    return 0;
}

int Manipulator::inputCarryPositionButton( bool value )
{
    if( value )
    {
        a_state = MANIPULATOR_STATE_CARRY_POSITION;
        a_deployedArm = false;
    }

    return 0;
}

int Manipulator::inputCarryPlusOnePositionButton( bool value )
{
    if( value )
    {
        a_state = MANIPULATOR_STATE_CARRY_PLUS_ONE_POSITION;
        a_deployedArm = false;
    }

    return 0;
}

int Manipulator::inputIntakePositionButton( bool value )
{
    if( value )
    {
        if( a_clawRightSwitch && a_clawLeftSwitch )
        {
            a_state = MANIPULATOR_STATE_INTAKE_COMPLETE_POSITION;
            a_deployedArm = false;
        }
        else
        {
            a_state = MANIPULATOR_STATE_INTAKE_POSITION;
            a_deployedArm = false;
        }
    }

    return 0;
}

int Manipulator::inputIntakePositionPlusOneButton( bool value )
{
    if( value )
    {
        a_state = MANIPULATOR_STATE_INTAKE_PLUS_ONE_POSITION;
        a_deployedArm = false;
    }

    return 0;
}

int Manipulator::inputClawLeftSwitch( bool value )
{
    a_clawLeftSwitch = value;

    if( a_clawRightSwitch && a_clawLeftSwitch &&
        ( a_state == MANIPULATOR_STATE_INTAKE_POSITION ) )
    {
        a_state = MANIPULATOR_STATE_INTAKE_COMPLETE_POSITION;
        a_deployedArm = false;
    }

    return 0;
}

int Manipulator::inputClawRightSwitch( bool value )
{
    a_clawRightSwitch = value;

    if( a_clawRightSwitch && a_clawLeftSwitch &&
        ( a_state == MANIPULATOR_STATE_INTAKE_POSITION ) )
    {
        a_state = MANIPULATOR_STATE_INTAKE_COMPLETE_POSITION;
        a_deployedArm = false;
    }

    return 0;
}

void Manipulator::setManualMode( void )
{
    a_state = MANIPULATOR_STATE_NONE;
}

std::string toString( manipulatorStateT state )
{
    std::string str = std::string( "unknown" );

    switch( state )
    {
        case MANIPULATOR_STATE_SCALE_POSITION:
            str = std::string( "scale_position" );
            break;
        case MANIPULATOR_STATE_SCALE_PLUS_ONE_POSITION:
            str = std::string( "scale_plus_one_position" );
            break;
        case MANIPULATOR_STATE_SCALE_BACK_POSITION:
            str = std::string( "scale_back_position" );
            break;
        case MANIPULATOR_STATE_SCALE_BACK_PLUS_ONE_POSITION:
            str = std::string( "scale_back_plus_one_position" );
            break;
        case MANIPULATOR_STATE_SWITCH_POSITION:
            str = std::string( "switch_position" );
            break;
        case MANIPULATOR_STATE_SWITCH_PLUS_ONE_POSITION:
            str = std::string( "switch_plus_one_position" );
            break;
        case MANIPULATOR_STATE_CARRY_POSITION:
            str = std::string( "carry_position" );
            break;
        case MANIPULATOR_STATE_CARRY_PLUS_ONE_POSITION:
            str = std::string( "carry_plus_one_position" );
            break;
        case MANIPULATOR_STATE_INTAKE_POSITION:
            str = std::string( "intake_position" );
            break;
        case MANIPULATOR_STATE_INTAKE_PLUS_ONE_POSITION:
            str = std::string( "intake_plus_one_position" );
            break;
        case MANIPULATOR_STATE_INTAKE_COMPLETE_POSITION:
            str = std::string( "intake_complete_position" );
            break;
        case MANIPULATOR_STATE_NONE:
        case MANIPULATOR_STATE_NUM:
        default:
            str = std::string( "unknown" );
            break;
    }

    return str;
}

bool Manipulator::isScale( void )
{
    return ( a_statePrev == MANIPULATOR_STATE_SCALE_POSITION ||
             a_statePrev == MANIPULATOR_STATE_SCALE_BACK_POSITION ||
             a_statePrev == MANIPULATOR_STATE_SCALE_PLUS_ONE_POSITION ||
             a_statePrev == MANIPULATOR_STATE_SCALE_BACK_PLUS_ONE_POSITION );
}
