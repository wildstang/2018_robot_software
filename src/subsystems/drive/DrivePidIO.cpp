#include "subsystems/drive/DrivePidIO.h"

DrivePidIO::DrivePidIO( void )
    : PidOutput()
    , PidInput()
    , a_encoderValue( 0.0 )
{
}

DrivePidIO::~DrivePidIO( void )
{
}

void DrivePidIO::pidWrite( double output )
{
    WsRobot::getInstance()->getDrive()->setPidDriveThrottle( output );
}

double DrivePidIO::pidRead( void )
{
    return a_encoderValue;
}

void DrivePidIO::setEncoderValue( double value )
{
    a_encoderValue = value;
}

