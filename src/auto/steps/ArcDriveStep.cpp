 #include "ArcDriveStep.h"

#include <cmath>

#include "subsystems/Drive.h"
#include "robot/WsRobot.h"

#include "auto/script/AutoStepFactoryArgs.h"
#include "utils/ws_utils.h"
#include "logger/WsLogger.h"

ArcDriveStep::ArcDriveStep( double radius, double angle, double speed, double timeout )
    : AutoStep()
    , ap_drive( NULL )
    , a_radius( radius )
    , a_angle( angle )
    , a_speed( speed )
    , a_timeout( timeout )
    , a_timeoutTimer()
{
    if( a_speed < 0.0 )
    {
        a_speed *= -1.0;
    }

    if( a_radius < 0.0 )
    {
        a_radius *= -1.0;
    }
}

ArcDriveStep::ArcDriveStep( AutoStepFactoryArgs* p_args )
    : AutoStep()
    , ap_drive( NULL )
    , a_radius( 0.0 )
    , a_angle( 0.0 )
    , a_speed( 0.0 )
    , a_timeout( 0.0 )
    , a_timeoutTimer()
{
    if( p_args )
    {
        p_args->getSpeed( this->a_speed );
        p_args->getAngle( this->a_angle );
        p_args->getInches( this->a_radius );
        p_args->optionalTimeout( this->a_timeout );

        if( a_speed < 0.0 )
        {
            a_speed *= -1.0;
        }

        if( a_radius < 0.0 )
        {
            a_radius *= -1.0;
        }
    }
    else
    {
        WS_ERROR( "improper arguments received for step %s",
                  __PRETTY_FUNCTION__ );
    }
}

ArcDriveStep::~ArcDriveStep( void )
{
}

void ArcDriveStep::initialize( void )
{
    ap_drive = WsRobot::getInstance()->getDrive();
    ap_drive->setDriveModeArc( a_radius, a_angle, a_speed );

    if( a_timeout > 0.0 )
    {
        a_timeoutTimer.stop();
        a_timeoutTimer.reset();
        a_timeoutTimer.start();
    }
}

void ArcDriveStep::update( void )
{
    if( ap_drive->arcHeadingPidStabilized() && ap_drive->arcDrivePidStabilized() )
    {
        ap_drive->setDriveModeCheesy();
        setFinished( true );
    }

    if( a_timeout > 0.0 )
    {
        if( a_timeoutTimer.hasPeriodPassed( a_timeout ) )
        {
            ap_drive->setDriveModeCheesy();
            WS_INFO( "finished arc drive step due to timeout." );
            setFinished( true );
        }
    }
}

std::string ArcDriveStep::getClassName( void )
{
    return std::string( "ArcDriveStep" );
}

AutoStepFactoryArgs ArcDriveStep::getArguments( void )
{
    AutoStepFactoryArgs a;

    a.setSpeed( this->a_speed );
    a.setAngle( this->a_angle );
    a.setInches( this->a_radius );
    a.setTimeout( this->a_timeout );

    return a;
}
