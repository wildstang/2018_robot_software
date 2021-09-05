#include "TurnPidIO.h"

TurnPidIO::TurnPidIO( void )
    : PidOutput()
    , PidInput()
    , a_gyroValue( 0.0 )
{
}

TurnPidIO::~TurnPidIO( void )
{
}

void TurnPidIO::pidWrite( double value )
{
    WsRobot::getInstance()->getDrive()->setTurnPidHeadingCorrection( value );
}

double TurnPidIO::pidRead( void )
{
    return a_gyroValue;
}

void TurnPidIO::setGyroValue( double value )
{
    a_gyroValue = value;
}

