#include "WsDigitalInput.h"

#include "DigitalInput.h"

#include "config/ConfigValueRef.h"

WsDigitalInput::WsDigitalInput( std::string name, int channel, bool pullup )
    : DigitalInput( name, false )
    , ap_input( NULL )
    , a_pullup( pullup )
    , a_channel( channel )
{
}

WsDigitalInput::WsDigitalInput( std::string name )
    : DigitalInput( name, false )
    , ap_input( NULL )
    , a_pullup( false )
    , a_channel( 0 )
{
    CONFIG_VAR( "channel", a_channel, int );
    CONFIG_VAR( "pull_up", a_pullup, bool );
}

WsDigitalInput::~WsDigitalInput( void )
{
    if( ap_input != NULL )
    {
        delete ap_input;
        ap_input = NULL;
    }
}

int WsDigitalInput::instantiate( void )
{
    if( ap_input != NULL ) delete ap_input;
    ap_input = new frc::DigitalInput( a_channel );
    return 0;
}

bool WsDigitalInput::readRawValue( void )
{
    if( a_pullup )
    {
        return !ap_input->Get();
    }
    else
    {
        return ap_input->Get();
    }
}

