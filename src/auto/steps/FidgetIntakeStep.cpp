#include "FidgetIntakeStep.h"

#include "subsystems/Claw.h"
#include "robot/WsRobot.h"
#include "logger/WsLogger.h"

#include "auto/script/AutoStepFactoryArgs.h"

FidgetIntakeStep::FidgetIntakeStep( int milliseconds )
    : AutoStep()
    , ap_claw( NULL )
    , a_delay( ( double )milliseconds / 1000.0 )
    , a_fidgetOne( false )
    , a_fidgetTwo( false )
    , a_timer()
{
    if( a_delay < 0.0 )
    {
        WS_WARNING( "delay must be greater than 0" );
        a_delay = 0.0;
    }
}

FidgetIntakeStep::FidgetIntakeStep( AutoStepFactoryArgs* p_args )
    : AutoStep()
    , ap_claw( NULL )
    , a_delay( 0.8 )
    , a_fidgetOne( false )
    , a_fidgetTwo( false )
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

FidgetIntakeStep::~FidgetIntakeStep( void )
{
}

void FidgetIntakeStep::initialize( void )
{
    ap_claw = WsRobot::getInstance()->getClaw();

    a_timer.reset();
    a_timer.start();
}

void FidgetIntakeStep::update( void )
{
    if( !a_fidgetOne )
    {
        ap_claw->setIntakeDirectionForward();
        ap_claw->setClawPositionClosed();
        a_fidgetOne = true;
    }

    if( !a_fidgetTwo && a_timer.hasPeriodPassed( a_delay ) )
    {
        ap_claw->setIntakeDirectionForward();
        ap_claw->setClawPositionOpen();
        a_fidgetTwo = true;
    }

    if( a_timer.hasPeriodPassed( a_delay * 2.0 ) )
    {
        ap_claw->setIntakeDirectionForward();
        ap_claw->setClawPositionClosed();
        setFinished( true );
    }
}

std::string FidgetIntakeStep::getClassName( void )
{
    return std::string( "FidgetIntakeStep" );
}

AutoStepFactoryArgs FidgetIntakeStep::getArguments( void )
{
    AutoStepFactoryArgs a;

    int ms = ( int )( a_delay * 1000.0 );
    a.setMilliseconds( ms );

    return a;
}

