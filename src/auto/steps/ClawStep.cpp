#include "ClawStep.h"

#include "subsystems/Claw.h"
#include "robot/WsRobot.h"
#include "logger/WsLogger.h"

#include "auto/script/AutoStepFactoryArgs.h"

ClawStep::ClawStep( clawAutoPositionT position, clawAutoDirectionT direction )
    : AutoStep()
    , ap_claw( NULL )
    , a_position( position )
    , a_direction( direction )
{
}

ClawStep::ClawStep( AutoStepFactoryArgs* p_args )
    : AutoStep()
    , ap_claw( NULL )
    , a_position( AUTO_CLAW_POS_NONE )
    , a_direction( AUTO_CLAW_DIRECTION_NONE )
{
    if( p_args )
    {
        int pos = 0;
        p_args->getPosition( pos );
        this->a_position = ( clawAutoPositionT )pos;

        int direction = 0;
        p_args->getDirection( direction );
        this->a_direction = ( clawAutoDirectionT )direction;
    }
    else
    {
        WS_ERROR( "not enough arguments provided for %s",
                  __PRETTY_FUNCTION__ );
    }
}

ClawStep::~ClawStep( void )
{
}

void ClawStep::initialize( void )
{
    ap_claw = WsRobot::getInstance()->getClaw();
}

void ClawStep::update( void )
{
    switch( a_position )
    {
        case AUTO_CLAW_POS_OPEN:
            ap_claw->setClawPositionOpen();
            break;
        case AUTO_CLAW_POS_CLOSED:
            ap_claw->setClawPositionClosed();
            break;
        case AUTO_CLAW_POS_RAGDOLL:
            ap_claw->setClawPositionRagdoll();
            break;
        case AUTO_CLAW_POS_NONE:
        case AUTO_CLAW_POS_NUM:
        default:
            break;
    }

    switch( a_direction )
    {
        case AUTO_CLAW_DIRECTION_FWD:
            ap_claw->setIntakeDirectionForward();
            break;
        case AUTO_CLAW_DIRECTION_REV:
            ap_claw->setIntakeDirectionReverse();
            break;
        case AUTO_CLAW_DIRECTION_REV_SLOWER:
            ap_claw->setIntakeDirectionReverseSlower();
            break;
        case AUTO_CLAW_DIRECTION_REV_SLOWEST:
            ap_claw->setIntakeDirectionReverseSlowest();
            break;
        case AUTO_CLAW_DIRECTION_NEUTRAL:
            ap_claw->setIntakeDirectionNeutral();
            break;
        case AUTO_CLAW_DIRECTION_REV_BALLS_TO_WALL:
            ap_claw->setIntakeDirectionReverseBallsToWall();
        case AUTO_CLAW_DIRECTION_NONE:
        case AUTO_CLAW_DIRECTION_NUM:
        default:
            break;
    }

    setFinished( true );
}

std::string ClawStep::getClassName( void )
{
    return std::string( "ClawStep" );
}

AutoStepFactoryArgs ClawStep::getArguments( void )
{
    AutoStepFactoryArgs a;

    int pos = ( int )a_position;
    a.setPosition( pos );

    int dir = ( int )a_direction;
    a.setDirection( dir );

    return a;
}
