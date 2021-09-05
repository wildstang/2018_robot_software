#include "CheesyDriveHelper.h"

#include <cmath>

#include "subsystems/drive/Util.h"

const double CheesyDriveHelper::sc_wheelDeadband = 0.02;
const double CheesyDriveHelper::sc_turnSensitivity = 1.6;
const double CheesyDriveHelper::sc_throttleDeadband = 0.02;

CheesyDriveHelper::CheesyDriveHelper( void )
    : a_quickStopAccumulator( 0.0 )
    , a_signal( ds_neutralSignal )
{
}

CheesyDriveHelper::~CheesyDriveHelper( void )
{
}

wsDriveSignalT CheesyDriveHelper::cheesyDrive( double throttle, double wheel, bool isQuickTurn )
{
    wheel = handleDeadband( wheel, sc_wheelDeadband );
    throttle = handleDeadband( throttle, sc_throttleDeadband );

    double overPower;

    double angularPower;

    if( isQuickTurn )
    {
        if( fabs( throttle ) < 0.2 )
        {
            double alpha = 0.1;
            a_quickStopAccumulator = ( 1 - alpha ) * a_quickStopAccumulator + alpha * limit( wheel, 1.0 ) * 2;
        }

        overPower = 1.0;
        angularPower = wheel;
    }
    else
    {
        overPower = 0.0;
        angularPower = fabs( throttle ) * wheel * sc_turnSensitivity - a_quickStopAccumulator;

        if( a_quickStopAccumulator > 1 )
        {
            a_quickStopAccumulator -= 1;
        }
        else if( a_quickStopAccumulator < -1 )
        {
            a_quickStopAccumulator += 1;
        }
        else
        {
            a_quickStopAccumulator = 0.0;
        }
    }

    double rightPwm = throttle - angularPower;
    double leftPwm = throttle + angularPower;

    if( leftPwm > 1.0 )
    {
        rightPwm -= overPower * ( leftPwm - 1.0 );
        leftPwm = 1.0;
    }
    else if( rightPwm > 1.0 )
    {
        leftPwm -= overPower * ( rightPwm - 1.0 );
        rightPwm = 1.0;
    }
    else if( leftPwm < -1.0 )
    {
        rightPwm += overPower * ( -1.0 - leftPwm );
        leftPwm = -1.0;
    }
    else if( rightPwm < -1.0 )
    {
        leftPwm += overPower * ( -1.0 - rightPwm );
        rightPwm = -1.0;
    }

    a_signal.rightMotor = rightPwm;
    a_signal.leftMotor = leftPwm;
    return a_signal;
}

double CheesyDriveHelper::handleDeadband( double val, double deadband )
{
    return ( fabs( val ) > fabs( deadband ) ) ? val : 0.0;
}

