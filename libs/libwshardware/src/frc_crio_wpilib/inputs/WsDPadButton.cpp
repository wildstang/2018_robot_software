#include "WsDPadButton.h"

#include "Joystick.h"
#include "JoystickConstants.h"

#include "config/ConfigValueRef.h"

WsDPadButton::WsDPadButton( std::string name, int port, int button_index )
    : DigitalInput( name, false )
    , ap_joystick( NULL )
    , a_buttonIndex( button_index )
    , a_port( port )
{
}

WsDPadButton::WsDPadButton( std::string name )
    : DigitalInput( name, false )
    , ap_joystick( NULL )
    , a_buttonIndex( 0 )
    , a_port( 0 )
{
    CONFIG_VAR( "port", a_port, int );
    CONFIG_VAR( "button_index", a_buttonIndex, int );
}

WsDPadButton::~WsDPadButton( void )
{
    if( ap_joystick != NULL )
    {
        delete ap_joystick;
        ap_joystick = NULL;
    }
}

int WsDPadButton::instantiate( void )
{
    if( ap_joystick != NULL ) delete ap_joystick;
    ap_joystick = new frc::Joystick( a_port );
    return 0;
}

bool WsDPadButton::readRawValue( void )
{
    double value;
    bool boolValue = false;

    switch( a_buttonIndex )
    {
        case DPAD_X_LEFT:
            value = ap_joystick->GetPOV();
            boolValue = ( value == 270 );
            break;

        case DPAD_X_RIGHT:
            value = ap_joystick->GetPOV();
            boolValue = ( value == 90 );
            break;

        case DPAD_Y_UP:
            value = ap_joystick->GetPOV();
            boolValue = ( value == 0 );
            break;

        case DPAD_Y_DOWN:
            value = ap_joystick->GetPOV();;
            boolValue = ( value == 180 );
            break;

        default:
            break;
    }

    return boolValue;
}

