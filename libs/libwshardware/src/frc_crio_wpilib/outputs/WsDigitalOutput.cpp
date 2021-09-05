#include "WsDigitalOutput.h"

#include "DigitalOutput.h"

#include "config/ConfigValueRef.h"

WsDigitalOutput::WsDigitalOutput( std::string name, int channel, bool default_value )
    : DigitalOutput( name, default_value )
    , ap_output( NULL )
    , a_channel( channel )
{
}

WsDigitalOutput::WsDigitalOutput( std::string name )
    : DigitalOutput( name, false )
    , ap_output( NULL )
    , a_channel( -1 )
{
    CONFIG_VAR( "channel", a_channel, int );
}

WsDigitalOutput::~WsDigitalOutput( void )
{
    if( ap_output != NULL )
    {
        delete ap_output;
        ap_output = NULL;
    }
}

int WsDigitalOutput::instantiate( void )
{
    int err = 0;
    if( ap_output != NULL ) delete ap_output;

    if( a_channel < 0 )
    {
        WS_ERROR( "Can't instantiate the output %s because it's not configured.",
                  this->getName().c_str() );
        err++;
    }
    else
    {
        ap_output = new frc::DigitalOutput( a_channel );
    }

    return err;
}

void WsDigitalOutput::sendDataToOutput( void )
{
    if( !ap_output )
    {
        WS_ERROR( "Update called on null output %s. Check config?",
                  this->getName().c_str() );
        return;
    }

    this->ap_output->Set( getValue() );
}

