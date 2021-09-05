#include "WsVictor.h"

#include "Victor.h"

#include "config/ConfigValueRef.h"

WsVictor::WsVictor( std::string name, int channel, double default_value )
    : AnalogOutput( name, default_value )
    , ap_victor( NULL )
    , a_channel( channel )
{
}

WsVictor::WsVictor( std::string name )
    : AnalogOutput( name, 0.0 )
    , ap_victor( NULL )
    , a_channel( 0 )
{
    CONFIG_VAR( "channel", a_channel, int );
}

WsVictor::~WsVictor( void )
{
    if( ap_victor != NULL )
    {
        delete ap_victor;
        ap_victor = NULL;
    }
}

int WsVictor::instantiate( void )
{
    if( ap_victor != NULL ) delete ap_victor;
    ap_victor = new frc::Victor( a_channel );
    return 0;
}

void WsVictor::sendDataToOutput( void )
{
    ap_victor->Set( this->getValue() );
}

