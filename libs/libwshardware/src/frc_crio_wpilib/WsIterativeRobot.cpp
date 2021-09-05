#include "WsIterativeRobot.h"

#include "logger/WsLogger.h"
#include "utils/ws_utils.h"

#ifndef __WSITERATIVEROBOT_H__TEMPLATE__

//
// non-template code
//

#else

template< typename T >
WsIterativeRobot< T >::WsIterativeRobot( void )
    : frc::IterativeRobot()
    , a_timer( "iterative_robot_profile", 50 )
{
}

template< typename T >
WsIterativeRobot< T >::~WsIterativeRobot( void )
{
}

template< typename T >
void WsIterativeRobot< T >::RobotInit( void )
{
    WS_FUNC_HEAD;
    a_timer.startTimingSection();
    this->initialize();
    this->wsRobotInit();
    double time = a_timer.endTimingSection();
    WS_TRACE( "%s run time %f", __PRETTY_FUNCTION__, time );
    WS_FUNC_FOOT;
}

template< typename T >
void WsIterativeRobot< T >::DisabledInit( void )
{
    WS_FUNC_HEAD;
    a_timer.startTimingSection();
    this->wsDisabledInit();
    double time = a_timer.endTimingSection();
    WS_TRACE( "%s run time %f", __PRETTY_FUNCTION__, time );
    WS_FUNC_FOOT;
}

template< typename T >
void WsIterativeRobot< T >::AutonomousInit( void )
{
    WS_FUNC_HEAD;
    a_timer.startTimingSection();
    this->wsAutonomousInit();
    double time = a_timer.endTimingSection();
    WS_TRACE( "%s run time %f", __PRETTY_FUNCTION__, time );
    WS_FUNC_FOOT;
}

template< typename T >
void WsIterativeRobot< T >::TeleopInit( void )
{
    WS_FUNC_HEAD;
    a_timer.startTimingSection();
    this->wsTeleopInit();
    double time = a_timer.endTimingSection();
    WS_TRACE( "%s run time %f", __PRETTY_FUNCTION__, time );
    WS_FUNC_FOOT;
}

template< typename T >
void WsIterativeRobot< T >::TestInit( void )
{
    WS_FUNC_HEAD;
    a_timer.startTimingSection();
    this->wsTestInit();
    double time = a_timer.endTimingSection();
    WS_TRACE( "%s run time %f", __PRETTY_FUNCTION__, time );
    WS_FUNC_FOOT;
}

template< typename T >
void WsIterativeRobot< T >::RobotPeriodic( void )
{
    WS_FUNC_HEAD;

    a_timer.startTimingSection();
    this->wsRobotPeriodic();
    double time = a_timer.endTimingSection();
    WS_TRACE( "%s run time %f", __PRETTY_FUNCTION__, time );

    uint64_t start = ws_milliseconds();
    this->executeUpdate();
    uint64_t end = ws_milliseconds();

    WS_TRACE( "executeUpdate cycle time: %llu", end - start );

    WS_FUNC_FOOT;
}

template< typename T >
void WsIterativeRobot< T >::DisabledPeriodic( void )
{
    WS_FUNC_HEAD;

    a_timer.startTimingSection();
    this->wsDisabledPeriodic();
    double time = a_timer.endTimingSection();
    WS_TRACE( "%s run time %f", __PRETTY_FUNCTION__, time );

    // TODO, maybe don't run the systems in disabled.
    uint64_t start = ws_milliseconds();
    this->executeUpdate();
    uint64_t end = ws_milliseconds();

    WS_TRACE( "executeUpdate cycle time: %llu", end - start );

    WS_FUNC_FOOT;
}

template< typename T >
void WsIterativeRobot< T >::AutonomousPeriodic( void )
{
    WS_FUNC_HEAD;

    a_timer.startTimingSection();
    this->wsAutonomousPeriodic();
    double time = a_timer.endTimingSection();
    WS_TRACE( "%s run time %f", __PRETTY_FUNCTION__, time );

    uint64_t start = ws_milliseconds();
    this->executeUpdate();
    uint64_t end = ws_milliseconds();

    WS_TRACE( "executeUpdate cycle time: %llu", end - start );

    WS_FUNC_FOOT;
}

template< typename T >
void WsIterativeRobot< T >::TeleopPeriodic( void )
{
    WS_FUNC_HEAD;

    a_timer.startTimingSection();
    this->wsTeleopPeriodic();
    double time = a_timer.endTimingSection();
    WS_TRACE( "%s run time %f", __PRETTY_FUNCTION__, time );

    uint64_t start = ws_milliseconds();
    this->executeUpdate();
    uint64_t end = ws_milliseconds();

    WS_TRACE( "executeUpdate cycle time: %llu", end - start );

    WS_FUNC_FOOT;
}

template< typename T >
void WsIterativeRobot< T >::TestPeriodic( void )
{
    WS_FUNC_HEAD;

    a_timer.startTimingSection();
    this->wsTestPeriodic();
    double time = a_timer.endTimingSection();
    WS_TRACE( "%s run time %f", __PRETTY_FUNCTION__, time );

    uint64_t start = ws_milliseconds();
    this->executeUpdate();
    uint64_t end = ws_milliseconds();

    WS_TRACE( "executeUpdate cycle time: %llu", end - start );

    WS_FUNC_FOOT;
}

#endif

