#include "subsystems/drive/ArcAnglePidIO.h"

#include "utils/ws_utils.h"

ArcAnglePidIO::ArcAnglePidIO( void )
    : PidOutput()
    , PidInput()
    , a_encoderValue( 0.0 )
    , a_gyroAngle( 0.0 )
    , a_radius( 0.0 )
{
}

ArcAnglePidIO::~ArcAnglePidIO( void )
{
}

void ArcAnglePidIO::pidWrite( double output )
{
    WsRobot::getInstance()->getDrive()->setArcPidHeading( output );
}

double ArcAnglePidIO::pidRead( void )
{
    return ( ( 2.0 * PI * a_radius * a_gyroAngle ) / 360.0 ) - a_encoderValue;
}

void ArcAnglePidIO::setEncoderValue( double value )
{
    a_encoderValue = value;
}

void ArcAnglePidIO::setGyroAngle( double value )
{
    a_gyroAngle = value;
}

void ArcAnglePidIO::setRadius( double value )
{
    a_radius = value;
}
