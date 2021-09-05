#include "DriveHeadingPidIO.h"

DriveHeadingPidIO::DriveHeadingPidIO( void )
    : PidOutput()
    , PidInput()
    , a_gyroValue( 0.0 )
{
}

DriveHeadingPidIO::~DriveHeadingPidIO( void )
{
}

void DriveHeadingPidIO::pidWrite( double value )
{
    WsRobot::getInstance()->getDrive()->setDrivePidHeadingCorrection( value );
}

double DriveHeadingPidIO::pidRead( void )
{
    return a_gyroValue;
}

void DriveHeadingPidIO::setGyroValue( double value )
{
    a_gyroValue = value;
}

