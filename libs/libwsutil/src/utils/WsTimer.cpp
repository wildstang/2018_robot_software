#include "WsTimer.h"

#include "utils/ws_utils.h"

WsTimer::WsTimer( void )
    : a_resetTimestamp( ws_microseconds() )
    , a_stopTimestamp( a_resetTimestamp )
    , a_state( WS_TIMER_STATE_STOPPED )
{
}

WsTimer::~WsTimer( void )
{
}

double WsTimer::get( void )
{
    uint64_t ts = ws_microseconds();
    uint64_t period = 0;

    switch( a_state )
    {
        case WS_TIMER_STATE_RUNNING:
            period = ts - a_resetTimestamp;
            break;
        case WS_TIMER_STATE_STOPPED:
            period = a_stopTimestamp - a_resetTimestamp;
            break;
        case WS_TIMER_STATE_NONE:
        case WS_TIMER_STATE_NUM:
        default:
            this->reset();
            break;
    }

    double sec = ( double )period / 1000000.0;

    return sec;
}

void WsTimer::reset( void )
{
    a_resetTimestamp = ws_microseconds();
    a_stopTimestamp = a_resetTimestamp;
    a_state = WS_TIMER_STATE_STOPPED;
}

void WsTimer::start( void )
{
    uint64_t ts = ws_microseconds();
    switch( a_state )
    {
        case WS_TIMER_STATE_RUNNING:
            break;
        case WS_TIMER_STATE_NONE:
        case WS_TIMER_STATE_NUM:
        default:
            this->reset();
        case WS_TIMER_STATE_STOPPED:
            a_resetTimestamp = ts - a_stopTimestamp + a_resetTimestamp;
            a_state = WS_TIMER_STATE_RUNNING;
            break;
    }
}

void WsTimer::stop( void )
{
    uint64_t ts = ws_microseconds();
    switch( a_state )
    {
        case WS_TIMER_STATE_RUNNING:
            a_stopTimestamp = ts;
            a_state = WS_TIMER_STATE_STOPPED;
            break;
        case WS_TIMER_STATE_STOPPED:
            break;
        case WS_TIMER_STATE_NONE:
        case WS_TIMER_STATE_NUM:
        default:
            this->reset();
            break;
    }
}

bool WsTimer::hasPeriodPassed( double period )
{
    return ( this->get() > period );
}

