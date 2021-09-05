#include "WsDSDigitalInput.h"

#include "config/ConfigValueRef.h"

WsDSDigitalInput::WsDSDigitalInput( std::string name, int channel )
    : DigitalInput( name, false )
    , a_channel( channel )
{
}

WsDSDigitalInput::WsDSDigitalInput( std::string name )
    : DigitalInput( name, false )
    , a_channel( 0 )
{
    CONFIG_VAR( "channel", a_channel, int );
}

WsDSDigitalInput::~WsDSDigitalInput( void )
{
}

int WsDSDigitalInput::instantiate( void )
{
    return 0;
}

bool WsDSDigitalInput::readRawValue( void )
{
    // NOTE: getDigitalIn() is no longer available in DriverStation API
    // digitalValue.setValue(DriverStation.getInstance().getDigitalIn(channel));
    // SWH TODO

    return false;
}

