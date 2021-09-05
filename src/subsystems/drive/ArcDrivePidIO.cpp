#include "subsystems/drive/ArcDrivePidIO.h"

ArcDrivePidIO::ArcDrivePidIO( void )
    : PidOutput()
    , PidInput()
    , a_encoderValue( 0.0 )
{
}

ArcDrivePidIO::~ArcDrivePidIO( void )
{
}

void ArcDrivePidIO::pidWrite( double output )
{
    WsRobot::getInstance()->getDrive()->setArcPidThrottle( output );
}

double ArcDrivePidIO::pidRead( void )
{
    return a_encoderValue;
}

void ArcDrivePidIO::setEncoderValue( double value )
{
    a_encoderValue = value;
}

