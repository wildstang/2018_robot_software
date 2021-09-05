#include "AutoStepDelay.h"

#include "logger/WsLogger.h"
#include "auto/script/AutoStepFactory.h"

AutoStepDelay::AutoStepDelay( int msDelay )
    : AutoStep()
    , a_delay( ( double )msDelay / 1000.0 )
    , a_timer()
{
    if( a_delay < 0 )
    {
        WS_WARNING( "Delay must be greater than 0" );
        a_delay = 0.0;
    }
}

AutoStepDelay::AutoStepDelay( AutoStepFactoryArgs* p_args )
    : AutoStep()
    , a_delay( 0.0 )
    , a_timer()
{
    if( p_args )
    {
        int d = 0;
        p_args->getMilliseconds( d );
        a_delay = ( double )d / 1000.0;
    }
    else
    {
        WS_ERROR( "Did not receive proper arguments for step %s",
                  __PRETTY_FUNCTION__ );
    }

    if( a_delay < 0 )
    {
        WS_WARNING( "Delay must be greater than 0" );
        a_delay = 0.0;
    }
}

AutoStepDelay::~AutoStepDelay( void )
{
}

void AutoStepDelay::initialize( void )
{
    a_timer.reset();
    a_timer.start();
    WS_INFO( "Timer in init %d", a_timer.get() );
}

void AutoStepDelay::update( void )
{
    if( a_timer.hasPeriodPassed( a_delay ) )
    {
        a_timer.stop();
        setFinished( true );
        WS_INFO( "Timer at finished %d", a_timer.get() );
    }
}

std::string AutoStepDelay::getClassName( void )
{
    return std::string( "AutoStepDelay" );
}

AutoStepFactoryArgs AutoStepDelay::getArguments( void )
{
    AutoStepFactoryArgs a;

    int ms = ( int )( a_delay * 1000.0 );
    a.setMilliseconds( ms );

    return a;
}

