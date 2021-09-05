#include "Arm.h"

#include <cstdio>
#include <cmath>

#include "io/Input.h"
#include "io/Output.h"
#include "io/InputManager.h"
#include "io/OutputManager.h"
#include "io/routing/InputSlot.h"
#include "io/routing/OutputSource.h"
#include "logger/WsLogger.h"
#include "utils/ws_string.h"
#include "config/ConfigValueRef.h"

#include "robot/WsRobot.h"
#include "subsystems/Lift.h"
#include "subsystems/Manipulator.h"

#include <cmath>

Arm::Arm( void )
    : Subsystem( std::string( "arm" ) )
    , a_state( ARM_STATE_NONE )
    , a_statePrev( ARM_STATE_NONE )
    , ap_lift( NULL )
    , a_desiredPosition( 0.0 )
    , a_desiredSpeed( 0.0 )
    , a_climbMode( false )
    , a_setPointIntake( 0.0 )
    , a_setPointIntakeComplete( 0.0 )
    , a_setPointScoreScale( 0.0 )
    , a_setPointScoreScalePlusOne( 0.0 )
    , a_setPointScoreSwitch( 0.0 )
    , a_setPointScoreSwitchPlusOne( 0.0 )
    , a_setPointScoreReverse( 0.0 )
    , a_setPointCarry( 0.0 )
    , a_setPointShootScale( 0.0 )
    , a_setPointShootScaleReverse( 0.0 )
    , a_maxSpeedPercent( 0.0 )
    , a_maxLiftSafeAngle( 0.0 )
    , a_safetyMargin( 0.0 )
    , a_stabilizedEpsilon( 0.0 )
    , a_reverseManualSafety( false )
    , a_reverseSensorSafety( false )
    , a_bottomSafetySwitch( false )
    , a_topSafetySwitch( false )
    , a_currentPosition( 0.0 )
    , a_armTalonStruct( { SMART_OUTPUT_CONTROL_MODE_ID_NONE,
                          0.0, 0, 0.0, 0.0, false, false } )
    , a_armFollowerStruct( { SMART_OUTPUT_CONTROL_MODE_ID_NONE,
                             0.0, 0, 0.0, 0.0, false, false } )
{
    CONFIG_VAR( "set_point_intake", a_setPointIntake, double );
    CONFIG_VAR( "set_point_intake_plus_one", a_setPointIntakePlusOne, double );
    CONFIG_VAR( "set_point_intake_complete", a_setPointIntakeComplete, double );
    CONFIG_VAR( "set_point_score_scale", a_setPointScoreScale, double );
    CONFIG_VAR( "set_point_score_scale_plus_one", a_setPointScoreScalePlusOne, double );
    CONFIG_VAR( "set_point_score_switch", a_setPointScoreSwitch, double );
    CONFIG_VAR( "set_point_score_switch_plus_one", a_setPointScoreSwitchPlusOne, double );
    CONFIG_VAR( "set_point_score_reverse", a_setPointScoreReverse, double );
    CONFIG_VAR( "set_point_carry", a_setPointCarry, double );
    CONFIG_VAR( "set_point_shoot_scale", a_setPointShootScale, double );
    CONFIG_VAR( "set_point_shoot_scale_reverse", a_setPointShootScaleReverse, double );

    CONFIG_VAR( "max_speed", a_maxSpeedPercent, double );

    CONFIG_VAR( "lift_safety_thresh", a_maxLiftSafeAngle, double );
    CONFIG_VAR( "safety_margin", a_safetyMargin, double );

    CONFIG_VAR( "stabilized_epsilon", a_stabilizedEpsilon, double );

    CONFIG_VAR( "reverse_manual_safety", a_reverseManualSafety, bool );
    CONFIG_VAR( "reverse_sensor_safety", a_reverseSensorSafety, bool );
}

Arm::~Arm( void )
{
}

void Arm::init( void )
{
    ap_lift = WsRobot::getInstance()->getLift();

    CONNECT_INPUT( "arm_bottom_switch", inputBottomSwitch, Arm, bool );
    CONNECT_INPUT( "arm_top_switch", inputTopSwitch, Arm, bool );

    CONNECT_INPUT( "arm_joy", inputManualSpeed, Arm, double );
    CONNECT_INPUT( "arm_pot", inputCurrentPosition, Arm, double );

    CONNECT_INPUT( "climb_button", inputClimbButton, Arm, bool );

    CONNECT_OUTPUT( "arm_smart", a_armTalonStruct, smartOutputCmdT );
    CONNECT_OUTPUT( "arm_follower", a_armFollowerStruct, smartOutputCmdT );

    a_armFollowerStruct.control_id = SMART_OUTPUT_CONTROL_MODE_ID_FOLLOW;
    a_armFollowerStruct.follow_output_id = 8;
}

void Arm::resetState( void )
{
    a_state = ARM_STATE_NONE;
    a_statePrev = ARM_STATE_NONE;

    a_desiredPosition = a_setPointIntake;
    a_desiredSpeed = 0.0;

    a_armTalonStruct.control_id =
        SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
    a_armTalonStruct.speed = 0.0;
}

void Arm::selfTest( void )
{
}

void Arm::update( void )
{
    if( !a_climbMode )
    {
        double m_sign = a_reverseManualSafety ? -1.0 : 1.0;
        double s_sign = a_reverseSensorSafety ? -1.0 : 1.0;

        armStateT next_state = a_state;

        switch( a_state )
        {
            case ARM_STATE_MANUAL_NOMINAL:
                a_armTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                a_armTalonStruct.speed = a_desiredSpeed;

                if( a_bottomSafetySwitch &&
                        ( ( a_desiredSpeed * m_sign ) < 0.0 ) )
                {
                    a_armTalonStruct.speed = 0.0;

                    next_state = ARM_STATE_MANUAL_BOTTOM_SW_BLOCK;
                }
                else if( ( ( a_currentPosition * s_sign ) >
                            ( a_maxLiftSafeAngle * s_sign ) ) &&
                        !ap_lift->inArmSafeZone() )
                {
                    a_armTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                    a_armTalonStruct.pos_setpoint =
                        a_maxLiftSafeAngle + a_safetyMargin;

                    next_state = ARM_STATE_MANUAL_LIFT_BLOCK;
                }
                else if( a_topSafetySwitch &&
                        ( ( a_desiredSpeed * m_sign ) > 0.0 ) )
                {
                    a_armTalonStruct.speed = 0.0;

                    next_state = ARM_STATE_MANUAL_TOP_SW_BLOCK;
                }
                break;

            case ARM_STATE_MANUAL_LIFT_BLOCK:
                a_armTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                a_armTalonStruct.pos_setpoint =
                    a_maxLiftSafeAngle + a_safetyMargin;

                if( ( a_desiredSpeed * m_sign ) < 0.0 )
                {
                    a_armTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                    a_armTalonStruct.speed = a_desiredSpeed;

                    next_state = ARM_STATE_MANUAL_NOMINAL;
                }
                else if( ap_lift->inArmSafeZone() )
                {
                    a_armTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                    a_armTalonStruct.speed = a_desiredSpeed;

                    next_state = ARM_STATE_MANUAL_NOMINAL;
                }
                break;

            case ARM_STATE_MANUAL_BOTTOM_SW_BLOCK:
                a_armTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                a_armTalonStruct.speed = 0.0;

                if( ( a_desiredSpeed * m_sign ) > 0.0 )
                {
                    a_armTalonStruct.speed = a_desiredSpeed;

                    next_state = ARM_STATE_MANUAL_NOMINAL;
                }
                else if( !a_bottomSafetySwitch )
                {
                    a_armTalonStruct.speed = a_desiredSpeed;

                    next_state = ARM_STATE_MANUAL_NOMINAL;
                }
                break;

            case ARM_STATE_MANUAL_TOP_SW_BLOCK:
                a_armTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                a_armTalonStruct.speed = 0.0;

                if( ( a_desiredSpeed * m_sign ) < 0.0 )
                {
                    a_armTalonStruct.speed = a_desiredSpeed;

                    next_state = ARM_STATE_MANUAL_NOMINAL;
                }
                else if( !a_topSafetySwitch )
                {
                    a_armTalonStruct.speed = a_desiredSpeed;

                    next_state = ARM_STATE_MANUAL_NOMINAL;
                }
                break;

            case ARM_STATE_PID_NOMINAL:
                a_armTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                a_armTalonStruct.pos_setpoint = a_desiredPosition;

                if( a_bottomSafetySwitch &&
                        ( ( a_currentPosition * s_sign ) >
                          ( a_desiredPosition * s_sign ) ) )
                {
                    a_armTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                    a_armTalonStruct.speed = 0.0;

                    next_state = ARM_STATE_PID_BOTTOM_SW_BLOCK;
                }
                else if( ( ( a_desiredPosition * s_sign ) >
                            ( a_maxLiftSafeAngle * s_sign ) ) &&
                        !ap_lift->inArmSafeZone() )
                {
                    a_armTalonStruct.pos_setpoint =
                        a_maxLiftSafeAngle + a_safetyMargin;

                    next_state = ARM_STATE_PID_LIFT_BLOCK;
                }
                else if( a_topSafetySwitch &&
                        ( ( a_currentPosition * s_sign ) <
                          ( a_desiredPosition * s_sign ) ) )
                {
                    a_armTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                    a_armTalonStruct.speed = 0.0;

                    next_state = ARM_STATE_PID_TOP_SW_BLOCK;
                }
                break;

            case ARM_STATE_PID_LIFT_BLOCK:
                a_armTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                a_armTalonStruct.pos_setpoint =
                    a_maxLiftSafeAngle + a_safetyMargin;

                if( ( a_desiredPosition * s_sign ) <
                        ( a_maxLiftSafeAngle * s_sign ) )
                {
                    a_armTalonStruct.pos_setpoint = a_desiredPosition;

                    next_state = ARM_STATE_PID_NOMINAL;
                }
                else if( ap_lift->inArmSafeZone() )
                {
                    a_armTalonStruct.pos_setpoint = a_desiredPosition;

                    next_state = ARM_STATE_PID_NOMINAL;
                }
                break;

            case ARM_STATE_PID_BOTTOM_SW_BLOCK:
                a_armTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                a_armTalonStruct.speed = 0.0;

                if( ( a_desiredPosition * s_sign ) >
                        ( a_currentPosition * s_sign ) )
                {
                    a_armTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                    a_armTalonStruct.pos_setpoint = a_desiredPosition;

                    next_state = ARM_STATE_PID_NOMINAL;
                }
                else if( !a_bottomSafetySwitch )
                {
                    a_armTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                    a_armTalonStruct.pos_setpoint = a_desiredPosition;

                    next_state = ARM_STATE_PID_NOMINAL;
                }
                break;

            case ARM_STATE_PID_TOP_SW_BLOCK:
                a_armTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                a_armTalonStruct.speed = 0.0;

                if( ( a_desiredPosition * s_sign ) <
                        ( a_currentPosition * s_sign ) )
                {
                    a_armTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                    a_armTalonStruct.pos_setpoint = a_desiredPosition;

                    next_state = ARM_STATE_PID_NOMINAL;
                }
                else if( !a_topSafetySwitch )
                {
                    a_armTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                    a_armTalonStruct.pos_setpoint = a_desiredPosition;

                    next_state = ARM_STATE_PID_NOMINAL;
                }
                break;

            case ARM_STATE_NONE:
            case ARM_STATE_NUM:
            default:
                a_armTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                a_armTalonStruct.speed = 0.0;
                break;
        }

        a_statePrev = a_state;
        a_state = next_state;
    }
    else
    {
        a_armTalonStruct.control_id =
            SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
        a_armTalonStruct.speed = 0.0;
    }
}

int Arm::inputBottomSwitch( bool value )
{
    a_bottomSafetySwitch = value;
    return 0;
}

int Arm::inputTopSwitch( bool value )
{
    a_topSafetySwitch = value;
    return 0;
}

void Arm::setScoreSwitchPosition( void )
{
    a_state = ARM_STATE_PID_NOMINAL;
    a_desiredPosition = a_setPointScoreSwitch;
}

void Arm::setScoreSwitchPlusOnePosition( void )
{
    a_state = ARM_STATE_PID_NOMINAL;
    a_desiredPosition = a_setPointScoreSwitchPlusOne;
}

void Arm::setScoreScalePosition( void )
{
    a_state = ARM_STATE_PID_NOMINAL;
    a_desiredPosition = a_setPointScoreScale;
}

void Arm::setScoreScalePlusOnePosition( void )
{
    a_state = ARM_STATE_PID_NOMINAL;
    a_desiredPosition = a_setPointScoreScalePlusOne;
}

void Arm::setScoreReversePosition( void )
{
    a_state = ARM_STATE_PID_NOMINAL;
    a_desiredPosition = a_setPointScoreReverse;
}

void Arm::setIntakePosition( void )
{
    a_state = ARM_STATE_PID_NOMINAL;
    a_desiredPosition = a_setPointIntake;
}

void Arm::setIntakePlusOnePosition( void )
{
    a_state = ARM_STATE_PID_NOMINAL;
    a_desiredPosition = a_setPointIntakePlusOne;
}

void Arm::setIntakeCompletePosition( void )
{
    a_state = ARM_STATE_PID_NOMINAL;
    a_desiredPosition = a_setPointIntakeComplete;
}

void Arm::setCarryPosition( void )
{
    a_state = ARM_STATE_PID_NOMINAL;
    a_desiredPosition = a_setPointCarry;
}

void Arm::setShootScalePosition( void )
{
    a_state = ARM_STATE_PID_NOMINAL;
    a_desiredPosition = a_setPointShootScale;
}

void Arm::setShootScaleReversePosition( void )
{
    a_state = ARM_STATE_PID_NOMINAL;
    a_desiredPosition = a_setPointShootScaleReverse;
}

int Arm::inputManualSpeed( double value )
{
    if( fabs( value ) >= .1 )
    {
        if( value >= a_maxSpeedPercent )
        {
            value = a_maxSpeedPercent;
        }
        else if( value <= ( -1.0 * a_maxSpeedPercent ) )
        {
            value = ( -1.0 * a_maxSpeedPercent );
        }

        if( !isManual( a_state ) )
        {
            a_state = ARM_STATE_MANUAL_NOMINAL;
			WsRobot::getInstance()->getManipulator()->setManualMode();
        }

        a_desiredSpeed = value;
    }
    else
    {
        if( !isPid( a_state ) )
        {
            a_state = ARM_STATE_PID_NOMINAL;
            a_desiredPosition = a_currentPosition;
        }
    }

    return 0;
}

int Arm::inputCurrentPosition( double value )
{
    a_currentPosition = value;
    return 0;
}

bool Arm::inLiftSafeZone( void )
{
    double sign = a_reverseSensorSafety ? -1.0 : 1.0;

    bool safe = ( ( a_currentPosition * sign ) <
                  ( a_maxLiftSafeAngle * sign ) );

    //printf( "arm safe: %d\n", safe ? 1 : 0 );

    return safe;
}

int Arm::inputClimbButton( bool value )
{
    if( value )
    {
        a_climbMode = true;
    }

    return 0;
}

bool Arm::isStabilized( void )
{
    return ( fabs( a_currentPosition - a_armTalonStruct.pos_setpoint )
           < fabs( a_stabilizedEpsilon ) );
}

std::string toString( armStateT state )
{
    std::string str = std::string( "unknown" );

    switch( state )
    {
        case ARM_STATE_MANUAL_NOMINAL:
            str = std::string( "manual_nominal" );
            break;
        case ARM_STATE_MANUAL_LIFT_BLOCK:
            str = std::string( "manual_lift_block" );
            break;
        case ARM_STATE_MANUAL_BOTTOM_SW_BLOCK:
            str = std::string( "manual_bottom_sw_block" );
            break;
        case ARM_STATE_MANUAL_TOP_SW_BLOCK:
            str = std::string( "manual_top_sw_block" );
            break;
        case ARM_STATE_PID_NOMINAL:
            str = std::string( "pid_nominal" );
            break;
        case ARM_STATE_PID_LIFT_BLOCK:
            str = std::string( "pid_lift_block" );
            break;
        case ARM_STATE_PID_BOTTOM_SW_BLOCK:
            str = std::string( "pid_bottom_sw_block" );
            break;
        case ARM_STATE_PID_TOP_SW_BLOCK:
            str = std::string( "pid_top_sw_block" );
            break;
        case ARM_STATE_NONE:
        case ARM_STATE_NUM:
        default:
            break;
    }

    return str;
}

bool isManual( armStateT state )
{
    return ( ( state == ARM_STATE_MANUAL_NOMINAL ) ||
             ( state == ARM_STATE_MANUAL_LIFT_BLOCK ) ||
             ( state == ARM_STATE_MANUAL_BOTTOM_SW_BLOCK ) ||
             ( state == ARM_STATE_MANUAL_TOP_SW_BLOCK ) );
}

bool isPid( armStateT state )
{
    return ( ( state == ARM_STATE_PID_NOMINAL ) ||
             ( state == ARM_STATE_PID_LIFT_BLOCK ) ||
             ( state == ARM_STATE_PID_BOTTOM_SW_BLOCK ) ||
             ( state == ARM_STATE_PID_TOP_SW_BLOCK ) );
}
