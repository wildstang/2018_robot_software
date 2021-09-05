#include "LiftFinishedStep.h"

#include "robot/WsRobot.h"
#include "subsystems/Lift.h"

LiftFinishedStep::LiftFinishedStep( targetTypeT rangeType )
    : AutoStep()
    , ap_lift( NULL )
    , a_rangeType( rangeType )
{
}

LiftFinishedStep::LiftFinishedStep( AutoStepFactoryArgs* p_args )
{
    if( p_args )
    {
        int range = 0;
        p_args->getState( range );

        a_rangeType = ( targetTypeT )range;
    }
}

LiftFinishedStep::~LiftFinishedStep( void )
{
}

void LiftFinishedStep::initialize( void )
{
    ap_lift = WsRobot::getInstance()->getLift();
}

void LiftFinishedStep::update( void )
{
    if( ( a_rangeType == TARGET_TYPE_ON_TARGET ) && 
        ap_lift->isStabilized() )
    {
        setFinished( true );
    }
    if( ( a_rangeType == TARGET_TYPE_IN_RANGE ) && 
        ap_lift->isInRange() )
    {
        setFinished( true );
    }
}

std::string LiftFinishedStep::getClassName( void )
{
    return std::string( "LiftFinishedStep" );
}

AutoStepFactoryArgs LiftFinishedStep::getArguments( void )
{
    AutoStepFactoryArgs a;

    int range = ( int )a_rangeType;
    a.setState( range );

    return a;
}

