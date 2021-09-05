#include "WsTalon.h"

#include "Talon.h"

#include "config/ConfigValueRef.h"

WsTalon::WsTalon( std::string name, int channel, double default_value )
    : AnalogOutput( name, default_value )
    , ap_talon( NULL )
    , a_channel( channel )
{
}

WsTalon::WsTalon( std::string name )
    : AnalogOutput( name, 0.0 )
    , ap_talon( NULL )
    , a_channel( 0 )
{
    CONFIG_VAR( "channel", a_channel, int );
}

WsTalon::~WsTalon( void )
{
    if( ap_talon != NULL )
    {
        delete ap_talon;
        ap_talon = NULL;
    }
}

int WsTalon::instantiate( void )
{
    if( ap_talon != NULL ) delete ap_talon;
    ap_talon = new frc::Talon( a_channel );
    return 0;
}

void WsTalon::sendDataToOutput( void )
{
    ap_talon->Set( this->getValue() );
}

