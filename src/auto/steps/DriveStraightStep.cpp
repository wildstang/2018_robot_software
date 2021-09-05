#include "DriveStraightStep.h"

#include <cmath>

#include "subsystems/Drive.h"
#include "robot/WsRobot.h"

#include "auto/script/AutoStepFactoryArgs.h"
#include "utils/ws_utils.h"
#include "logger/WsLogger.h"

DriveStraightStep::DriveStraightStep( double speed, 
                                                      int inchesToTravel, 
                                                      double timeout )
    : AutoStep()
    , ap_drive( NULL )
    , a_speed( speed )
    , a_distanceToGo( inchesToTravel )
    , a_timeout( timeout )
    , a_timeoutTimer()
{
    if( a_speed < 0.0 )
    {
        a_speed *= -1.0;
    }
}

DriveStraightStep::DriveStraightStep( AutoStepFactoryArgs* p_args )
    : AutoStep()
    , ap_drive( NULL )
    , a_speed( 0.0 )
    , a_distanceToGo( 0 )
    , a_timeout( -1.0 )
    , a_timeoutTimer()
{
    if( p_args )
    {
        p_args->getSpeed( this->a_speed );
        p_args->getInches( this->a_distanceToGo );
        p_args->optionalTimeout( this->a_timeout );

        if( a_speed < 0.0 )
        {
            a_speed *= -1.0;
        }
    }
    else
    {
        WS_ERROR( "improper arguments received for step %s",
                  __PRETTY_FUNCTION__ );
    }
}

DriveStraightStep::~DriveStraightStep( void )
{
}

void DriveStraightStep::initialize( void )
{
    ap_drive = WsRobot::getInstance()->getDrive();
    double ticks = a_distanceToGo * ( 4096.0 / ( 6.0 * PI ) );
    ap_drive->setDriveModeDriveStraight( a_speed, ticks );

    if( a_timeout > 0.0 )
    {
        a_timeoutTimer.stop();
        a_timeoutTimer.reset();
        a_timeoutTimer.start();
    }
}

void DriveStraightStep::update( void )
{
    if( ap_drive->drivePidStabilized() && ap_drive->driveHeadingPidStabilized() )
    {
        ap_drive->setDriveModeCheesy();
        setFinished( true );
    }

    if( a_timeout > 0.0 )
    {
        if( a_timeoutTimer.hasPeriodPassed( a_timeout ) )
        {
            ap_drive->setDriveModeCheesy();
            WS_INFO( "finished drive step due to timeout." );
            setFinished( true );
        }
    }
}

std::string DriveStraightStep::getClassName( void )
{
    return std::string( "DriveStraightStep" );
}

AutoStepFactoryArgs DriveStraightStep::getArguments( void )
{
    AutoStepFactoryArgs a;

    a.setSpeed( this->a_speed );
    a.setInches( this->a_distanceToGo );
    a.setTimeout( this->a_timeout );

    return a;
}

