#include "WsDSAnalogInput.h"

#include "config/ConfigValueRef.h"

WsDSAnalogInput::WsDSAnalogInput( std::string name, int channel )
    : AnalogInput( name, 0.0 )
    , a_channel( channel )
{
}

WsDSAnalogInput::WsDSAnalogInput( std::string name )
    : AnalogInput( name, 0.0 )
    , a_channel( 0 )
{
    CONFIG_VAR( "channel", a_channel, int );
}

WsDSAnalogInput::~WsDSAnalogInput( void )
{
}

int WsDSAnalogInput::instantiate( void )
{
    return 0;
}

double WsDSAnalogInput::readRawValue( void )
{
    // NOTE: getAnalogIn() no longer available in Driverstation API
    // analogValue.setValue(DriverStation.getInstance().getAnalogIn(channel));
    // SWH TODO

    return 0;
}

