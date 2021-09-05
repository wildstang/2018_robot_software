#include "WsSmartDashboard.h"

#include "logger/WsLogger.h"

WsSmartDashboard::WsSmartDashboard( void )
{
}

WsSmartDashboard::~WsSmartDashboard( void )
{
}

int WsSmartDashboard::put( std::string key, int value )
{
    WS_ERROR( "%s not implemented", __PRETTY_FUNCTION__ );
    return 1;
}

int WsSmartDashboard::put( std::string key, double value )
{
    WS_ERROR( "%s not implemented", __PRETTY_FUNCTION__ );
    return 1;
}

int WsSmartDashboard::put( std::string key, bool value )
{
    WS_ERROR( "%s not implemented", __PRETTY_FUNCTION__ );
    return 1;
}

int WsSmartDashboard::put( std::string key, std::string value )
{
    WS_ERROR( "%s not implemented", __PRETTY_FUNCTION__ );
    return 1;
}

