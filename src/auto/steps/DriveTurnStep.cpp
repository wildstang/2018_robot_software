#include "DriveTurnStep.h"

#include <cmath>

#include "subsystems/Drive.h"
#include "robot/WsRobot.h"
#include "auto/AutoManager.h"

#include "auto/script/AutoStepFactoryArgs.h"
#include "logger/WsLogger.h"

DriveTurnStep::DriveTurnStep( double angle, double speed, double timeout )
    : AutoStep()
    , ap_drive( NULL )
    , a_angle( angle )
    , a_speed( speed )
    , a_timeout( timeout )
    , a_timeoutTimer()
{
    if( a_speed < 0.0 )
    {
        a_speed *= -1.0;
    }

    if( WsRobot::getInstance()->getAutoManager()->isReversed() )
    {
        a_angle *= -1.0;
    }
}

DriveTurnStep::DriveTurnStep( AutoStepFactoryArgs* p_args )
    : AutoStep()
    , ap_drive( NULL )
    , a_angle( 0.0 )
    , a_speed( 0.0 )
    , a_timeout( -1.0 )
    , a_timeoutTimer()
{
    if( p_args )
    {
        p_args->getAngle( a_angle );
        p_args->getSpeed( a_speed );
        p_args->optionalTimeout( a_timeout );

        if( a_speed < 0.0 )
        {
            a_speed *= -1.0;
        }

        if( WsRobot::getInstance()->getAutoManager()->isReversed() )
        {
            a_angle *= -1.0;
        }
    }
    else
    {
        WS_ERROR( "improper arguments received for step %s",
                  __PRETTY_FUNCTION__ );
    }
}

DriveTurnStep::~DriveTurnStep( void )
{
}

void DriveTurnStep::initialize( void )
{
    ap_drive = WsRobot::getInstance()->getDrive();
    ap_drive->setDriveModeTurn( a_angle, a_speed );

    if( a_timeout > 0.0 )
    {
        a_timeoutTimer.stop();
        a_timeoutTimer.reset();
        a_timeoutTimer.start();
    }
}

void DriveTurnStep::update( void )
{
    if( ap_drive->turnPidStabilized() )
    {
        ap_drive->setDriveModeCheesy();
        setFinished( true );
    }

    if( a_timeout > 0.0 )
    {
        if( a_timeoutTimer.hasPeriodPassed( a_timeout ) )
        {
            ap_drive->setDriveModeCheesy();
            WS_INFO( "finished turn step due to timeout." );
            setFinished( true );
        }
    }
}

std::string DriveTurnStep::getClassName( void )
{
    return std::string( "DriveTurnStep" );
}

AutoStepFactoryArgs DriveTurnStep::getArguments( void )
{
    AutoStepFactoryArgs a;

    a.setAngle( this->a_angle );
    a.setSpeed( this->a_speed );
    a.setTimeout( this->a_timeout );

    return a;
}

