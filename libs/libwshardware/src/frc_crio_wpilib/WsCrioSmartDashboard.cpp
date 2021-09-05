#include "WsCrioSmartDashboard.h"

#include "SmartDashboard/SmartDashboard.h"

WsCrioSmartDashboard::WsCrioSmartDashboard( void )
{
}

WsCrioSmartDashboard::~WsCrioSmartDashboard( void )
{
}

int WsCrioSmartDashboard::put( std::string key, int value )
{
    double x = ( double )value;
    frc::SmartDashboard::PutNumber( key, x );
    return 0;
}

int WsCrioSmartDashboard::put( std::string key, double value )
{
    frc::SmartDashboard::PutNumber( key, value );
    return 0;
}

int WsCrioSmartDashboard::put( std::string key, bool value )
{
    frc::SmartDashboard::PutBoolean( key, value );
    return 0;
}

int WsCrioSmartDashboard::put( std::string key, std::string value )
{
    frc::SmartDashboard::PutString( key, value );
    return 0;
}

