#include "ArmStep.h"

#include "subsystems/Arm.h"
#include "robot/WsRobot.h"
#include "logger/WsLogger.h"

#include "auto/script/AutoStepFactoryArgs.h"

ArmStep::ArmStep( autoArmT position )
    : AutoStep()
    , ap_arm( NULL )
    , a_position( position )
{
}

ArmStep::ArmStep( AutoStepFactoryArgs* p_args )
    : AutoStep()
    , ap_arm( NULL )
    , a_position( AUTO_ARM_NONE )
{
    if( p_args )
    {
        int pos = 0;
        p_args->getPosition( pos );
        this->a_position = ( autoArmT )pos;
    }
    else
    {
        WS_ERROR( "not enough arguments provided for %s",
                  __PRETTY_FUNCTION__ );
    }
}

ArmStep::~ArmStep( void )
{
}

void ArmStep::initialize( void )
{
    ap_arm = WsRobot::getInstance()->getArm();
}

void ArmStep::update( void )
{
    switch( a_position )
    {
        case AUTO_ARM_POS_INTAKE:
            ap_arm->setIntakePosition();
            break;
        case AUTO_ARM_POS_INTAKE_PLUS_ONE:
            ap_arm->setIntakePlusOnePosition();
            break;
        case AUTO_ARM_POS_INTAKE_COMPLETE:
            ap_arm->setIntakeCompletePosition();
            break;
        case AUTO_ARM_POS_SCORE_SWITCH:
            ap_arm->setScoreSwitchPosition();
            break;
        case AUTO_ARM_POS_SCORE_SWITCH_PLUS_ONE:
            ap_arm->setScoreSwitchPlusOnePosition();
            break;
        case AUTO_ARM_POS_SCORE_SCALE:
            ap_arm->setScoreScalePosition();
            break;
        case AUTO_ARM_POS_SCORE_SCALE_PLUS_ONE:
            ap_arm->setScoreScalePlusOnePosition();
            break;
        case AUTO_ARM_POS_SCORE_REVERSE:
            ap_arm->setScoreReversePosition();
            break;
        case AUTO_ARM_POS_CARRY:
            ap_arm->setCarryPosition();
            break;
        case AUTO_ARM_POS_SHOOT_SCALE:
            ap_arm->setShootScalePosition();
            break;
        case AUTO_ARM_POS_SHOOT_SCALE_REVERSE:
            ap_arm->setShootScaleReversePosition();
            break;
        case AUTO_ARM_NONE:
        case AUTO_ARM_NUM:
        default:
            break;
    }

    setFinished( true );
}

std::string ArmStep::getClassName( void )
{
    return std::string( "ArmStep" );
}

AutoStepFactoryArgs ArmStep::getArguments( void )
{
    AutoStepFactoryArgs a;

    int pos = ( int )a_position;
    a.setPosition( pos );

    return a;
}
