#include "WsAbsoluteEncoder.h"

#include "config/ConfigValueRef.h"

WsAbsoluteEncoder::WsAbsoluteEncoder( std::string name, int channel, double max_voltage )
    : WsAnalogInput( name, channel )
    , a_maxVoltage( max_voltage )
{
}

WsAbsoluteEncoder::WsAbsoluteEncoder( std::string name )
    : WsAnalogInput( name )
    , a_maxVoltage( 0.0 )
{
    CONFIG_VAR( "max_voltage", a_maxVoltage, double );
}

WsAbsoluteEncoder::~WsAbsoluteEncoder( void )
{
}

int WsAbsoluteEncoder::instantiate( void )
{
    return WsAnalogInput::instantiate();
}

double WsAbsoluteEncoder::readRawValue( void )
{
    double rawValue = WsAnalogInput::readRawValue();

    double position = ( rawValue / a_maxVoltage ) * 360.0;

    return position;
}

