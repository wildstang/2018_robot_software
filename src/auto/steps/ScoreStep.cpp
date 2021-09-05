#include "ScoreStep.h"

#include "subsystems/Claw.h"
#include "robot/WsRobot.h"
#include "logger/WsLogger.h"

#include "auto/script/AutoStepFactoryArgs.h"

ScoreStep::ScoreStep( int milliseconds )
    : AutoStep()
    , ap_claw( NULL )
    , a_delay( ( double )milliseconds / 1000.0 )
    , a_hasStarted( false )
    , a_timer()
{
    if( a_delay < 0.0 )
    {
        WS_WARNING( "delay must be greater than 0" );
        a_delay = 0.0;
    }
}

ScoreStep::ScoreStep( AutoStepFactoryArgs* p_args )
    : AutoStep()
    , ap_claw( NULL )
    , a_delay( 0.8 )
    , a_hasStarted( false )
    , a_timer()
{
    if( p_args )
    {
        int d = 0;
        p_args->optionalMilliseconds( d );
        a_delay = ( double )d / 1000.0;
    }
    else
    {
        WS_ERROR( "Did not receive proper arguments for step %s",
                  __PRETTY_FUNCTION__ );
    }

    if( a_delay < 0.0 )
    {
        WS_WARNING( "delay must be greater than 0" );
        a_delay = 0.0;
    }
}

ScoreStep::~ScoreStep( void )
{
}

void ScoreStep::initialize( void )
{
    ap_claw = WsRobot::getInstance()->getClaw();

    a_timer.reset();
    a_timer.start();
}

void ScoreStep::update( void )
{
    if( !a_hasStarted )
    {
        ap_claw->setIntakeDirectionReverseSlowest();
        ap_claw->setClawPositionClosed();
        a_hasStarted = true;
    }

    if( a_timer.hasPeriodPassed( a_delay ) )
    {
        ap_claw->setIntakeDirectionNeutral();
        ap_claw->setClawPositionRagdoll();
        setFinished( true );
    }
}

std::string ScoreStep::getClassName( void )
{
    return std::string( "ScoreStep" );
}

AutoStepFactoryArgs ScoreStep::getArguments( void )
{
    AutoStepFactoryArgs a;

    int ms = ( int )( a_delay * 1000.0 );
    a.setMilliseconds( ms );

    return a;
}

