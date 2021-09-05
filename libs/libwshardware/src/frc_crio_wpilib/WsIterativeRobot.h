#ifndef __WSITERATIVEROBOT_H__
#define __WSITERATIVEROBOT_H__

#include "IterativeRobot.h"

#include "utils/ProfilingTimer.h"

//
// We wrote this class to implement IterativeRobot and
// drive our robot class that is not aware of WPIlib.
//

class RobotCore;

template< typename T >
class WsIterativeRobot : public frc::IterativeRobot, public T
{
    public:
        WsIterativeRobot( void );
        virtual ~WsIterativeRobot( void );

        virtual void RobotInit( void ) override;
        virtual void DisabledInit( void ) override;
        virtual void AutonomousInit( void ) override;
        virtual void TeleopInit( void ) override;
        virtual void TestInit( void ) override;

        virtual void RobotPeriodic( void ) override;
        virtual void DisabledPeriodic( void ) override;
        virtual void AutonomousPeriodic( void ) override;
        virtual void TeleopPeriodic( void ) override;
        virtual void TestPeriodic( void ) override;

    protected:
        ProfilingTimer a_timer;
};

#define __WSITERATIVEROBOT_H__TEMPLATE__
#include "WsIterativeRobot.cpp"
#undef __WSITERATIVEROBOT_H__TEMPLATE__

#endif

