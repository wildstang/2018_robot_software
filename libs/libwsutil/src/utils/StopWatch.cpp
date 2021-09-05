#include "StopWatch.h"

StopWatch::StopWatch( void )
    : a_totalTimeInSec( 0.0 )
    , a_startTimeInNanoSec( 0L )
    , a_totalTimeInNanoSec( 0L )
    , a_timerRunning( false )
{
    this->Reset();
}

StopWatch::~StopWatch( void )
{
}

void StopWatch::Reset( void )
{
    a_startTimeInNanoSec = 0;
    a_totalTimeInNanoSec = 0;
    a_totalTimeInSec = 0.0;
    a_timerRunning = false;
}

void StopWatch::Start( void )
{
    a_startTimeInNanoSec = 0; //System.nanoTime();
    a_timerRunning = true;
}

void StopWatch::Stop( void )
{
    if( a_timerRunning == true )
    {
        long currentTimeInNanoSec = 0; //System.nanoTime();
        long updateTimeInNanoSec;

        updateTimeInNanoSec = currentTimeInNanoSec - a_startTimeInNanoSec;

        a_totalTimeInNanoSec += updateTimeInNanoSec;

        a_startTimeInNanoSec = currentTimeInNanoSec;

        a_timerRunning = false;
    }
}

double StopWatch::GetTimeInSec( void )
{
    if( a_timerRunning == true )
    {
        long currentTimeInNanoSec = 0; //System.nanoTime();
        long updateTimeInNanoSec;

        updateTimeInNanoSec = currentTimeInNanoSec - a_startTimeInNanoSec;

        a_totalTimeInNanoSec += updateTimeInNanoSec;

        a_startTimeInNanoSec = currentTimeInNanoSec;
    }

    a_totalTimeInSec = ( double )a_totalTimeInNanoSec / 1000000000.0;

    return a_totalTimeInSec;
}

