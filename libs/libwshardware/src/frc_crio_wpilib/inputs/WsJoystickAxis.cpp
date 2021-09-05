#include "WsJoystickAxis.h"

#include "Joystick.h"

#include "config/ConfigValueRef.h"

WsJoystickAxis::WsJoystickAxis( std::string name, int port, int axisIndex )
    : AnalogInput( name, 0.0 )
    , ap_joystick( NULL )
    , a_axisIndex( axisIndex )
    , a_port( port )
{
}

WsJoystickAxis::WsJoystickAxis( std::string name )
    : AnalogInput( name, 0.0 )
    , ap_joystick( NULL )
    , a_axisIndex( 0 )
    , a_port( 0 )
{
    CONFIG_VAR( "port", a_port, int );
    CONFIG_VAR( "axis_index", a_axisIndex, int );
}

WsJoystickAxis::~WsJoystickAxis( void )
{
    if( ap_joystick != NULL )
    {
        delete ap_joystick;
        ap_joystick = NULL;
    }
}

int WsJoystickAxis::instantiate( void )
{
    if( ap_joystick != NULL ) delete ap_joystick;
    ap_joystick = new frc::Joystick( a_port );
    return 0;
}

double WsJoystickAxis::readRawValue( void )
{
    double value;

    // Invert the vertical axes so that full up is 1
    if( a_axisIndex % 2 == 0 )
    {
        value = ap_joystick->GetRawAxis( a_axisIndex );
    }
    else
    {
        value = ap_joystick->GetRawAxis( a_axisIndex ) * -1;
    }

    return value;
}

