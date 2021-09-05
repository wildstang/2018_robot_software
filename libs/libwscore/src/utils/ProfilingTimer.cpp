#include "ProfilingTimer.h"

#include "core/RobotCore.h"

ProfilingTimer::ProfilingTimer( std::string name, int iterations )
    : a_startingTime( 0.0 )
    , a_endingTime( 0.0 )
    , a_iterations( iterations )
    , a_name( name )
{
}

ProfilingTimer::~ProfilingTimer( void )
{
}

void ProfilingTimer::startTimingSection( void )
{
    a_startingTime = 
        RobotCore::getRobotCore()->getHardware()->getFPGATimestamp();
}

double ProfilingTimer::endTimingSection( void )
{
    double spentTime = 0;
    a_endingTime = 
        RobotCore::getRobotCore()->getHardware()->getFPGATimestamp();
    spentTime = a_endingTime - a_startingTime;
    return spentTime;
}

