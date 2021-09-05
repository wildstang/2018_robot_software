#include "Lift.h"

#include "utils/ws_string.h"
#include "io/Output.h"
#include "io/routing/OutputSource.h"
#include "io/routing/InputSlot.h"
#include "config/ConfigValueRef.h"

#include "robot/WsRobot.h"
#include "subsystems/Arm.h"
#include "subsystems/Manipulator.h"

#include <cmath>

Lift::Lift( void )
    : Subsystem( std::string( "lift" ) )
    , a_brakeDeployed( false )
    , a_softResetSensorPosition( 0.0 )
    , a_state( LIFT_STATE_NONE )
    , a_statePrev( LIFT_STATE_NONE )
    , ap_arm( NULL )
    , a_desiredPosition( 0.0 )
    , a_desiredSpeed( 0.0 )
    , a_climbMode( false )
    , a_setPointIntake( 0.0 )
    , a_setPointCarry( 0.0 )
    , a_setPointCarryPlusOne( 0.0 )
    , a_setPointSwitch( 0.0 )
    , a_setPointSwitchPlusOne( 0.0 )
    , a_setPointScale( 0.0 )
    , a_setPointScalePlusOne( 0.0 )
    , a_setPointScaleReverse( 0.0 )
	, a_setPointScaleReversePlusOne( 0.0 )
    , a_setPointShootScale( 0.0 )
    , a_setPointShootScaleReverse( 0.0 )
    , a_maximumHeight( 0.0 )
    , a_safetyMargin( 0.0 )
    , a_minimumSafeArmHeight( 0.0 )
    , a_maxSpeedPercent( 1.0 )
    , a_brakeSpeedThresh( 0.05 )
    , a_inRangeEpsilon( 0.0 )
    , a_reverseManualSafety( false )
    , a_reverseSensorSafety( false )
    , a_reverseBrakeLogic( false )
    , a_pidStabilized( false )
    , a_bottomSafetySwitch( false )
    , a_currentPosition( 0.0 )
    , a_currentVelocity( 0.0 )
    , a_winchTalonStruct( { SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE,
                            0.0, 0, 0.0, 0.0, false, false } )
    , a_winchFollowerStruct( { SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE,
                               0.0, 0, 0.0, 0.0, false, false } )
    , a_brakeDeployedOutput( 1 )
    , a_stateProbe( "state", std::string( "" ) )
    , a_currentPositionProbe( "current_pos", 0.0 )
    , a_desiredPositionProbe( "desired_pos", 0.0 )
    , a_desiredSpeedProbe( "desired_pos", 0.0 )
    , a_isPidProbe( "is_pid", false )
    , a_isManualProbe( "is_manual", false )
    , a_isBrakeProbe( "is_brake", false )
    , a_brakeDeployedProbe( "brake_deployed", false )
{
    CONFIG_VAR( "set_point_intake", a_setPointIntake, double );
    CONFIG_VAR( "set_point_intake_plus_one", a_setPointIntakePlusOne, double );
    CONFIG_VAR( "set_point_carry", a_setPointCarry, double );
    CONFIG_VAR( "set_point_carry_plus_one", a_setPointCarryPlusOne, double );
    CONFIG_VAR( "set_point_switch", a_setPointSwitch, double );
    CONFIG_VAR( "set_point_switch_plus_one", a_setPointSwitchPlusOne, double );
    CONFIG_VAR( "set_point_scale", a_setPointScale, double );
    CONFIG_VAR( "set_point_scale_plus_one", a_setPointScalePlusOne, double );
    CONFIG_VAR( "set_point_scale_reverse", a_setPointScaleReverse, double );
	CONFIG_VAR( "set_point_scale_reverse_plus_one", a_setPointScaleReversePlusOne, double );
    CONFIG_VAR( "set_point_shoot_scale", a_setPointShootScale, double );
    CONFIG_VAR( "set_point_shoot_scale_reverse", a_setPointShootScaleReverse, double );

    CONFIG_VAR( "maximum_height", a_maximumHeight, double );
    CONFIG_VAR( "safety_margin", a_safetyMargin, double );
    CONFIG_VAR( "arm_safety_thresh", a_minimumSafeArmHeight, double );
    CONFIG_VAR( "in_range_epsilon", a_inRangeEpsilon, double );

    CONFIG_VAR( "max_manual_speed", a_maxSpeedPercent, double );
    CONFIG_VAR( "brake_speed_thresh", a_brakeSpeedThresh, double );

    CONFIG_VAR( "reverse_manual_safety", a_reverseManualSafety, bool );
    CONFIG_VAR( "reverse_sensor_safety", a_reverseSensorSafety, bool );
    CONFIG_VAR( "reverse_brake_logic", a_reverseBrakeLogic, bool );

    this->addProbe( &a_stateProbe );
    this->addProbe( &a_currentPositionProbe );
    this->addProbe( &a_desiredPositionProbe );
    this->addProbe( &a_desiredSpeedProbe );
    this->addProbe( &a_isPidProbe );
    this->addProbe( &a_isManualProbe );
    this->addProbe( &a_isBrakeProbe );
    this->addProbe( &a_brakeDeployedProbe );
}

Lift::~Lift( void )
{
}

void Lift::init( void )
{
    ap_arm = WsRobot::getInstance()->getArm();

    CONNECT_INPUT( "lift_joy", inputManualSpeed, Lift, double );
    CONNECT_INPUT( "lift_winch_sensor", inputCurrentPosition, Lift, double );
    CONNECT_INPUT( "lift_winch_velocity", inputCurrentVelocity, Lift, double );
    //CONNECT_INPUT( "lift_bottom_switch", inputBottomSwitch, Lift, bool );
    //CONNECT_INPUT( "lift_winch_is_stabilized", inputPidStabilized, Lift, bool );
    CONNECT_INPUT( "lift_winch_smart_is_stabilized", inputPidStabilized, Lift, bool );

    CONNECT_INPUT( "climb_button", inputClimbButton, Lift, bool );

    //CONNECT_OUTPUT( "lift_winch", a_winchTalonStruct, smartOutputCmdT );
    CONNECT_OUTPUT( "lift_winch_smart", a_winchTalonStruct, smartOutputCmdT );
    CONNECT_OUTPUT( "lift_follower", a_winchFollowerStruct, smartOutputCmdT );
    CONNECT_OUTPUT( "lift_brake", a_brakeDeployedOutput, int );

    a_winchFollowerStruct.control_id = SMART_OUTPUT_CONTROL_MODE_ID_FOLLOW;
    a_winchFollowerStruct.follow_output_id = 15;
}

void Lift::resetState( void )
{
    a_brakeDeployed = true;
    a_state = LIFT_STATE_BRAKE_APPLIED;
    a_statePrev = LIFT_STATE_BRAKE_APPLIED;

    a_desiredPosition = a_setPointIntake;
    a_desiredSpeed = 0.0;

    a_winchTalonStruct.control_id =
        SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
    a_winchTalonStruct.speed = 0.0;
}

void Lift::update( void )
{
    if( !a_climbMode )
    {
        double m_sign = a_reverseManualSafety ? -1.0 : 1.0;
        double s_sign = a_reverseSensorSafety ? -1.0 : 1.0;

        if( a_bottomSafetySwitch )
        {
            a_softResetSensorPosition = a_currentPosition;
        }

        if( !isBrake( a_state ) )
        {
            a_brakeDeployed = false;
        }

        liftStateT next_state = a_state;

        switch( a_state )
        {
            case LIFT_STATE_MANUAL_NOMINAL:
                a_winchTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                a_winchTalonStruct.speed = a_desiredSpeed;

                if( a_bottomSafetySwitch &&
                        ( ( a_desiredSpeed * m_sign ) < 0.0 ) )
                {
                    a_winchTalonStruct.speed = 0.0;

                    next_state = LIFT_STATE_MANUAL_BOTTOM_SW_BLOCK;
                }
                else if( ( ( a_currentPosition * s_sign ) <
                            ( a_minimumSafeArmHeight * s_sign ) ) &&
                        !ap_arm->inLiftSafeZone() )
                {
                    a_winchTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                    a_winchTalonStruct.pos_setpoint =
                        a_minimumSafeArmHeight + a_safetyMargin;

                    next_state = LIFT_STATE_MANUAL_ARM_BLOCK;
                }
                else if( ( ( a_currentPosition * s_sign ) >
                            ( a_maximumHeight * s_sign ) ) &&
                        ( ( a_desiredSpeed * m_sign ) > 0.0 ) )
                {
                    a_winchTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                    a_winchTalonStruct.pos_setpoint = a_maximumHeight;

                    next_state = LIFT_STATE_MANUAL_MAX_HEIGHT_BLOCK;
                }
                break;

            case LIFT_STATE_MANUAL_ARM_BLOCK:
                a_winchTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                a_winchTalonStruct.pos_setpoint =
                    a_minimumSafeArmHeight + a_safetyMargin;

                if( ( a_desiredSpeed * m_sign ) > 0.0 )
                {
                    a_winchTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                    a_winchTalonStruct.speed = a_desiredSpeed;

                    next_state = LIFT_STATE_MANUAL_NOMINAL;
                }
                else if( ap_arm->inLiftSafeZone() )
                {
                    a_winchTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                    a_winchTalonStruct.speed = a_desiredSpeed;

                    next_state = LIFT_STATE_MANUAL_NOMINAL;
                }
                break;

            case LIFT_STATE_MANUAL_BOTTOM_SW_BLOCK:
                a_winchTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                a_winchTalonStruct.speed = 0.0;

                if( ( a_desiredSpeed * m_sign ) > 0.0 )
                {
                    a_winchTalonStruct.speed = a_desiredSpeed;

                    next_state = LIFT_STATE_MANUAL_NOMINAL;
                }
                else if( !a_bottomSafetySwitch )
                {
                    a_winchTalonStruct.speed = a_desiredSpeed;

                    next_state = LIFT_STATE_MANUAL_NOMINAL;
                }
                break;

            case LIFT_STATE_MANUAL_MAX_HEIGHT_BLOCK:
                a_winchTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                a_winchTalonStruct.pos_setpoint = a_maximumHeight;

                if( ( a_desiredSpeed * m_sign ) < 0.0 )
                {
                    a_winchTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                    a_winchTalonStruct.speed = a_desiredSpeed;

                    next_state = LIFT_STATE_MANUAL_NOMINAL;
                }
                else if( ( a_currentPosition * s_sign ) <
                        ( a_maximumHeight * s_sign ) )
                {
                    a_winchTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                    a_winchTalonStruct.speed = a_desiredSpeed;

                    next_state = LIFT_STATE_MANUAL_NOMINAL;
                }
                break;

            case LIFT_STATE_PID_NOMINAL:
                a_winchTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                a_winchTalonStruct.pos_setpoint = a_desiredPosition;

                if( a_bottomSafetySwitch &&
                        ( ( a_currentPosition * s_sign ) >
                          ( a_desiredPosition * s_sign ) ) )
                {
                    a_winchTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                    a_winchTalonStruct.speed = 0.0;

                    next_state = LIFT_STATE_PID_BOTTOM_SW_BLOCK;
                }
                else if( ( ( a_desiredPosition * s_sign ) <
                            ( a_minimumSafeArmHeight * s_sign ) ) &&
                        !ap_arm->inLiftSafeZone() )
                {
                    a_winchTalonStruct.pos_setpoint =
                        a_minimumSafeArmHeight + a_safetyMargin;

                    next_state = LIFT_STATE_PID_ARM_BLOCK;
                }
                else if( ( a_desiredPosition * s_sign ) >
                        ( a_maximumHeight * s_sign ) )
                {
                    a_winchTalonStruct.pos_setpoint = a_maximumHeight;

                    next_state = LIFT_STATE_PID_MAX_HEIGHT_BLOCK;
                }
                break;

            case LIFT_STATE_PID_ARM_BLOCK:
                a_winchTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                a_winchTalonStruct.pos_setpoint =
                    a_minimumSafeArmHeight + a_safetyMargin;

                if( ( a_desiredPosition * s_sign ) >
                        ( a_minimumSafeArmHeight * s_sign ) )
                {
                    a_winchTalonStruct.pos_setpoint = a_desiredPosition;

                    next_state = LIFT_STATE_PID_NOMINAL;
                }
                else if( ap_arm->inLiftSafeZone() )
                {
                    a_winchTalonStruct.pos_setpoint = a_desiredPosition;

                    next_state = LIFT_STATE_PID_NOMINAL;
                }
                break;

            case LIFT_STATE_PID_BOTTOM_SW_BLOCK:
                a_winchTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                a_winchTalonStruct.speed = 0.0;

                if( ( a_desiredPosition * s_sign ) >
                        ( a_currentPosition * s_sign ) )
                {
                    a_winchTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                    a_winchTalonStruct.pos_setpoint = a_desiredPosition;

                    next_state = LIFT_STATE_PID_NOMINAL;
                }
                else if( !a_bottomSafetySwitch )
                {
                    a_winchTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                    a_winchTalonStruct.pos_setpoint = a_desiredPosition;

                    next_state = LIFT_STATE_PID_NOMINAL;
                }
                break;

            case LIFT_STATE_PID_MAX_HEIGHT_BLOCK:
                a_winchTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                a_winchTalonStruct.speed = 0.0;

                if( ( a_desiredPosition * s_sign ) <
                        ( a_currentPosition * s_sign ) )
                {
                    a_winchTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                    a_winchTalonStruct.pos_setpoint = a_desiredPosition;

                    next_state = LIFT_STATE_PID_NOMINAL;
                }
                else if( ( a_currentPosition * s_sign ) <
                        ( a_maximumHeight * s_sign ) )
                {
                    a_winchTalonStruct.control_id =
                        SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID;
                    a_winchTalonStruct.pos_setpoint = a_desiredPosition;

                    next_state = LIFT_STATE_PID_NOMINAL;
                }
                break;

            case LIFT_STATE_PID_BRAKE_ARM_BLOCK:
                a_winchTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                a_winchTalonStruct.speed = 0.0;
                a_brakeDeployed = true;

                if( fabs( a_currentVelocity ) >
                        fabs( a_brakeSpeedThresh ) )
                {
                    WS_WARNING( "brake applied, but lift was still moving." );
                    a_brakeDeployed = false;
                }

                if( ( a_desiredPosition * s_sign ) >
                        ( a_minimumSafeArmHeight * s_sign ) )
                {
                    a_winchTalonStruct.pos_setpoint = a_desiredPosition;

                    next_state = LIFT_STATE_PID_NOMINAL;
                }
                else if( ap_arm->inLiftSafeZone() )
                {
                    a_winchTalonStruct.pos_setpoint = a_desiredPosition;

                    next_state = LIFT_STATE_PID_NOMINAL;
                }
                break;

            case LIFT_STATE_BRAKE_APPLIED:
                a_winchTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                a_winchTalonStruct.speed = 0.0;
                a_brakeDeployed = true;

                if( fabs( a_currentVelocity ) >
                        fabs( a_brakeSpeedThresh ) )
                {
                    WS_WARNING( "brake applied, but lift was still moving." );
                    //next_state = LIFT_STATE_PID_NOMINAL;
                    //a_deiredPosition = a_currentPosition;
                    a_brakeDeployed = false;
                }
                break;

            case LIFT_STATE_NONE:
            case LIFT_STATE_NUM:
            default:
                a_winchTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                a_winchTalonStruct.speed = 0.0;
                break;
        }

        if( a_winchTalonStruct.control_id ==
                SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID )
        {
            bool brake = a_pidStabilized;

            if( fabs( a_currentVelocity ) <
                    fabs( a_brakeSpeedThresh ) )
            {
                a_brakeDeployed = brake;
            }
            else
            {
                if( brake )
                    WS_WARNING( "pid says stabilized but lift is moving too fast." );
            }

            if( brake )
            {
                a_winchTalonStruct.control_id =
                    SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
                a_winchTalonStruct.speed = 0.0;

                if( a_state != LIFT_STATE_PID_ARM_BLOCK )
                {
                    next_state = LIFT_STATE_BRAKE_APPLIED;
                }
                else
                {
                    next_state = LIFT_STATE_PID_BRAKE_ARM_BLOCK;
                }
            }
        }

        a_statePrev = a_state;
        a_state = next_state;
    }
    else
    {
        a_winchTalonStruct.control_id =
            SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
        a_winchTalonStruct.speed = a_joystickValue;
        a_brakeDeployed = false;
    }

    a_brakeDeployedOutput = a_reverseBrakeLogic ?
        ( a_brakeDeployed ? 3 : 2 ) : ( a_brakeDeployed ? 2 : 3 );

    a_stateProbe.update( toString( a_state ) );
    a_currentPositionProbe.update( a_currentPosition );
    a_desiredPositionProbe.update( a_desiredPosition );
    a_desiredSpeedProbe.update( a_desiredSpeed );
    a_isPidProbe.update( isPid( a_state ) );
    a_isManualProbe.update( isManual( a_state ) );
    a_isBrakeProbe.update( isBrake( a_state ) );
    a_brakeDeployedProbe.update( a_brakeDeployed );
}

void Lift::selfTest( void )
{
}

int Lift::inputPidStabilized( bool value )
{
    a_pidStabilized = value;
    return 0;
}

int Lift::inputCurrentVelocity( double value )
{
    a_currentVelocity = value;
    return 0;
}

void Lift::setIntakePosition( void )
{
    if( !isPid( a_state ) )
    {
        a_state = LIFT_STATE_PID_NOMINAL;
    }

    a_desiredPosition = a_setPointIntake;
}

void Lift::setIntakePlusOnePosition( void )
{
    if( !isPid( a_state ) )
    {
        a_state = LIFT_STATE_PID_NOMINAL;
    }

    a_desiredPosition = a_setPointIntakePlusOne;
}

void Lift::setCarryPosition( void )
{
    if( !isPid( a_state ) )
    {
        a_state = LIFT_STATE_PID_NOMINAL;
    }

    a_desiredPosition = a_setPointCarry;
}

void Lift::setCarryPlusOnePosition( void )
{
    if( !isPid( a_state ) )
    {
        a_state = LIFT_STATE_PID_NOMINAL;
    }

    a_desiredPosition = a_setPointCarryPlusOne;
}

void Lift::setSwitchPosition( void )
{
    if( !isPid( a_state ) )
    {
        a_state = LIFT_STATE_PID_NOMINAL;
    }

    a_desiredPosition = a_setPointSwitch;
}

void Lift::setSwitchPlusOnePosition( void )
{
    if( !isPid( a_state ) )
    {
        a_state = LIFT_STATE_PID_NOMINAL;
    }

    a_desiredPosition = a_setPointSwitchPlusOne;
}

void Lift::setScalePosition( void )
{
    if( !isPid( a_state ) )
    {
        a_state = LIFT_STATE_PID_NOMINAL;
    }

    a_desiredPosition = a_setPointScale;
}

void Lift::setScalePlusOnePosition( void )
{
    if( !isPid( a_state ) )
    {
        a_state = LIFT_STATE_PID_NOMINAL;
    }

    a_desiredPosition = a_setPointScalePlusOne;
}

void Lift::setScaleReversePosition( void )
{
    if( !isPid( a_state ) )
    {
        a_state = LIFT_STATE_PID_NOMINAL;
    }

    a_desiredPosition = a_setPointScaleReverse;
}

void Lift::setScaleReversePlusOnePosition( void )
{
	if( !isPid( a_state ) )
	{
		a_state = LIFT_STATE_PID_NOMINAL;
	}

	a_desiredPosition = a_setPointScaleReversePlusOne;
}

void Lift::setShootScalePosition( void )
{
    if( !isPid( a_state ) )
    {
        a_state = LIFT_STATE_PID_NOMINAL;
    }

    a_desiredPosition = a_setPointShootScale;
}

void Lift::setShootScaleReversePosition( void )
{
    if( !isPid( a_state ) )
    {
        a_state = LIFT_STATE_PID_NOMINAL;
    }

    a_desiredPosition = a_setPointShootScaleReverse;
}

bool Lift::inArmSafeZone( void )
{
    double sign = a_reverseSensorSafety ? -1.0 : 1.0;

    bool safe = ( ( a_currentPosition * sign ) >
                  ( a_minimumSafeArmHeight * sign ) );

    return safe;
}

int Lift::inputManualSpeed( double value )
{
    a_joystickValue = value;

    if( fabs( value ) >= .1 )
    {
        if ( value >= a_maxSpeedPercent )
        {
            value = a_maxSpeedPercent;
        }
        else if ( value <= ( -1.0 * a_maxSpeedPercent ) )
        {
            value = ( -1.0 * a_maxSpeedPercent );
        }

        a_desiredSpeed = value;

        if( !isManual( a_state ) )
        {
            a_state = LIFT_STATE_MANUAL_NOMINAL;
			WsRobot::getInstance()->getManipulator()->setManualMode();
        }
    }
    else
    {
        if( !isPid( a_state ) )
        {
            a_state = LIFT_STATE_PID_NOMINAL;
            a_desiredPosition = a_currentPosition;
        }
    }

    return 0;
}

int Lift::inputCurrentPosition( double value )
{
    a_currentPosition = value;
    return 0;
}

int Lift::inputBottomSwitch( bool value )
{
    a_bottomSafetySwitch = value;
    return 0;
}

int Lift::inputClimbButton( bool value )
{
    if( value )
    {
        a_climbMode = true;
    }

    return 0;
}

bool Lift::isStabilized( void )
{
    return ( a_state == LIFT_STATE_BRAKE_APPLIED );
}

bool Lift::isInRange( void )
{
    return ( ( isPid( a_state ) &&
             ( fabs( a_currentPosition - a_desiredPosition ) <
               fabs( a_inRangeEpsilon ) ) ) || isBrake( a_state ) );
}

std::string toString( liftStateT state )
{
    std::string str = std::string( "unknown" );

    switch( state )
    {
        case LIFT_STATE_MANUAL_NOMINAL:
            str = std::string( "manual_nominal" );
            break;
        case LIFT_STATE_MANUAL_ARM_BLOCK:
            str = std::string( "manual_arm_block" );
            break;
        case LIFT_STATE_MANUAL_BOTTOM_SW_BLOCK:
            str = std::string( "manual_bottom_sw_block" );
            break;
        case LIFT_STATE_MANUAL_MAX_HEIGHT_BLOCK:
            str = std::string( "manual_max_height_block" );
            break;
        case LIFT_STATE_PID_NOMINAL:
            str = std::string( "pid_nominal" );
            break;
        case LIFT_STATE_PID_ARM_BLOCK:
            str = std::string( "pid_arm_block" );
            break;
        case LIFT_STATE_PID_BRAKE_ARM_BLOCK:
            str = std::string( "pid_brake_arm_block" );
            break;
        case LIFT_STATE_PID_BOTTOM_SW_BLOCK:
            str = std::string( "pid_bottom_sw_block" );
            break;
        case LIFT_STATE_PID_MAX_HEIGHT_BLOCK:
            str = std::string( "pid_max_height_block" );
            break;
        case LIFT_STATE_BRAKE_APPLIED:
            str = std::string( "brake_applied" );
            break;
        case LIFT_STATE_NONE:
        case LIFT_STATE_NUM:
        default:
            break;
    }

    return str;
}

bool isManual( liftStateT state )
{
    return ( ( state == LIFT_STATE_MANUAL_NOMINAL ) ||
             ( state == LIFT_STATE_MANUAL_ARM_BLOCK ) ||
             ( state == LIFT_STATE_MANUAL_BOTTOM_SW_BLOCK ) ||
             ( state == LIFT_STATE_MANUAL_MAX_HEIGHT_BLOCK ) );
}

bool isPid( liftStateT state )
{
    return ( ( state == LIFT_STATE_PID_NOMINAL ) ||
             ( state == LIFT_STATE_PID_ARM_BLOCK ) ||
             ( state == LIFT_STATE_PID_BRAKE_ARM_BLOCK ) ||
             ( state == LIFT_STATE_PID_BOTTOM_SW_BLOCK ) ||
             ( state == LIFT_STATE_PID_MAX_HEIGHT_BLOCK ) );
}

bool isBrake( liftStateT state )
{
    return ( ( state == LIFT_STATE_BRAKE_APPLIED ) ||
             ( state == LIFT_STATE_PID_BRAKE_ARM_BLOCK ) );
}
