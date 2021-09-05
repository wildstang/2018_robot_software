#include "WsAnalogInput.h"

#include "AnalogInput.h"

#include "config/ConfigValueRef.h"

WsAnalogInput::WsAnalogInput( std::string name, int channel )
    : AnalogInput( name, 0.0 )
    , ap_input( NULL )
    , a_channel( channel )
{
}

WsAnalogInput::WsAnalogInput( std::string name )
    : AnalogInput( name, 0.0 )
    , ap_input( NULL )
    , a_channel( 0 )
{
    CONFIG_VAR( "channel", a_channel, int );
}

WsAnalogInput::~WsAnalogInput( void )
{
    if( ap_input != NULL )
    {
        delete ap_input;
        ap_input = NULL;
    }
}

int WsAnalogInput::instantiate( void )
{
    if( ap_input != NULL ) delete ap_input;
    ap_input = new frc::AnalogInput( a_channel );
    return 0;
}

double WsAnalogInput::readRawValue( void )
{
    return ap_input->GetAverageVoltage();
}

