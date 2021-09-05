#include "LiftStep.h"

#include "subsystems/Lift.h"
#include "robot/WsRobot.h"
#include "logger/WsLogger.h"

#include "auto/script/AutoStepFactoryArgs.h"

LiftStep::LiftStep( autoLiftT position )
    : AutoStep()
    , ap_lift( NULL )
    , a_position( position )
{
}

LiftStep::LiftStep( AutoStepFactoryArgs* p_args )
    : AutoStep()
    , ap_lift( NULL )
    , a_position( AUTO_LIFT_NONE )
{
    if( p_args )
    {
        int pos = 0;
        p_args->getPosition( pos );
        this->a_position = ( autoLiftT )pos;
    }
    else
    {
        WS_ERROR( "not enough arguments provided for %s",
                  __PRETTY_FUNCTION__ );
    }
}

LiftStep::~LiftStep( void )
{
}

void LiftStep::initialize( void )
{
    ap_lift = WsRobot::getInstance()->getLift();
}

void LiftStep::update( void )
{
    switch( a_position )
    {
        case AUTO_LIFT_POS_INTAKE:
            ap_lift->setIntakePosition();
            break;
        case AUTO_LIFT_POS_INTAKE_PLUS_ONE:
            ap_lift->setIntakePlusOnePosition();
            break;
        case AUTO_LIFT_POS_CARRY:
            ap_lift->setCarryPosition();
            break;
        case AUTO_LIFT_POS_CARRY_PLUS_ONE:
            ap_lift->setCarryPlusOnePosition();
            break;
        case AUTO_LIFT_POS_SWITCH:
            ap_lift->setSwitchPosition();
            break;
        case AUTO_LIFT_POS_SWITCH_PLUS_ONE:
            ap_lift->setSwitchPlusOnePosition();
            break;
        case AUTO_LIFT_POS_SCALE:
            ap_lift->setScalePosition();
            break;
        case AUTO_LIFT_POS_SCALE_PLUS_ONE:
            ap_lift->setScalePlusOnePosition();
            break;
        case AUTO_LIFT_POS_SCALE_REVERSE:
            ap_lift->setScaleReversePosition();
            break;
        case AUTO_LIFT_POS_SCALE_REVERSE_PLUS_ONE:
            ap_lift->setScaleReversePlusOnePosition();
            break;
        case AUTO_LIFT_POS_SHOOT_SCALE:
            ap_lift->setShootScalePosition();
            break;
        case AUTO_LIFT_POS_SHOOT_SCALE_REVERSE:
            ap_lift->setShootScaleReversePosition();
            break;
        case AUTO_LIFT_NONE:
        case AUTO_LIFT_NUM:
        default:
            break;
    }

    setFinished( true );
}

std::string LiftStep::getClassName( void )
{
    return std::string( "LiftStep" );
}

AutoStepFactoryArgs LiftStep::getArguments( void )
{
    AutoStepFactoryArgs a;

    int pos = ( int )a_position;
    a.setPosition( pos );

    return a;
}
