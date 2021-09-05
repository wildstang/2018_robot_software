#include "WsSolenoid.h"

#include "Solenoid.h"

#include "config/ConfigValueRef.h"

WsSolenoid::WsSolenoid( std::string name, int module,
                        int channel, bool default_value )
    : DigitalOutput( name, default_value )
    , ap_solenoid( NULL )
    , a_module( module )
    , a_channel( channel )
{
}

WsSolenoid::WsSolenoid( std::string name, int module, int channel )
    : DigitalOutput( name, false )
    , ap_solenoid( NULL )
    , a_module( module )
    , a_channel( channel )
{
}

WsSolenoid::WsSolenoid( std::string name )
    : DigitalOutput( name, false )
    , ap_solenoid( NULL )
    , a_module( 0 )
    , a_channel( 0 )
{
    CONFIG_VAR( "module", a_module, int );
    CONFIG_VAR( "channel", a_channel, int );
}

WsSolenoid::~WsSolenoid( void )
{
    if( ap_solenoid != NULL )
    {
        delete ap_solenoid;
        ap_solenoid = NULL;
    }
}

int WsSolenoid::instantiate( void )
{
    if( ap_solenoid != NULL ) delete ap_solenoid;
    ap_solenoid = new frc::Solenoid( a_module, a_channel );
    ap_solenoid->Set( this->getValue() );
    return 0;
}

void WsSolenoid::sendDataToOutput( void )
{
    ap_solenoid->Set( this->getValue() );
}

