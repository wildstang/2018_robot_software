#include "ManipulatorStep.h"

#include "subsystems/Lift.h"
#include "subsystems/Arm.h"
#include "robot/WsRobot.h"
#include "logger/WsLogger.h"

#include "auto/script/AutoStepFactoryArgs.h"

ManipulatorStep::ManipulatorStep( autoManipulatorT position )
    : AutoStep()
    , ap_lift( NULL )
    , ap_arm( NULL )
    , a_position( position )
{
}

ManipulatorStep::ManipulatorStep( AutoStepFactoryArgs* p_args )
    : AutoStep()
    , ap_lift( NULL )
    , ap_arm( NULL )
    , a_position( AUTO_NONE )
{
    if( p_args )
    {
        int pos = 0;
        p_args->getPosition( pos );
        this->a_position = ( autoManipulatorT )pos;
    }
    else
    {
        WS_ERROR( "not enough arguments provided for %s",
                  __PRETTY_FUNCTION__ );
    }
}

ManipulatorStep::~ManipulatorStep( void )
{
}

void ManipulatorStep::initialize( void )
{
    ap_lift = WsRobot::getInstance()->getLift();
    ap_arm = WsRobot::getInstance()->getArm();
}

void ManipulatorStep::update( void )
{
    switch( a_position )
    {
        case AUTO_INTAKE:
            ap_lift->setIntakePosition();
            ap_arm->setIntakePosition();
            break;
        case AUTO_INTAKE_PLUS_ONE:
            ap_lift->setIntakePlusOnePosition();
            ap_arm->setIntakePlusOnePosition();
            break;
        case AUTO_CARRY:
            ap_lift->setCarryPosition();
            ap_arm->setCarryPosition();
            break;
        case AUTO_CARRY_PLUS_ONE:
            ap_lift->setCarryPlusOnePosition();
            ap_arm->setCarryPosition();
            break;
        case AUTO_SWITCH:
            ap_lift->setSwitchPosition();
            ap_arm->setScoreSwitchPosition();
            break;
        case AUTO_SWITCH_PLUS_ONE:
            ap_lift->setSwitchPlusOnePosition();
            ap_arm->setScoreSwitchPlusOnePosition();
            break;
        case AUTO_SCALE:
            ap_lift->setScalePosition();
            ap_arm->setScoreScalePosition();
            break;
        case AUTO_SCALE_PLUS_ONE:
            ap_lift->setScalePlusOnePosition();
            ap_arm->setScoreScalePlusOnePosition();
            break;
        case AUTO_SCALE_REVERSE:
            ap_lift->setScaleReversePosition();
            ap_arm->setScoreReversePosition();
            break;
        case AUTO_SCALE_REVERSE_PLUS_ONE:
            ap_lift->setScaleReversePlusOnePosition();
            ap_arm->setScoreReversePosition();
            break;
        case AUTO_SHOOT_SCALE:
            ap_lift->setShootScalePosition();
            ap_arm->setShootScalePosition();
            break;
        case AUTO_SHOOT_SCALE_REVERSE:
            ap_lift->setShootScaleReversePosition();
            ap_arm->setShootScaleReversePosition();
            break;
        case AUTO_NONE:
        case AUTO_NUM:
        default:
            break;
    }

    setFinished( true );
}

std::string ManipulatorStep::getClassName( void )
{
    return std::string( "ManipulatorStep" );
}

AutoStepFactoryArgs ManipulatorStep::getArguments( void )
{
    AutoStepFactoryArgs a;

    int pos = ( int )a_position;
    a.setPosition( pos );

    return a;
}

