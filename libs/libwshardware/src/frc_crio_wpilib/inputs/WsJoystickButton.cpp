#include "WsJoystickButton.h"

#include "Joystick.h"

#include "config/ConfigValueRef.h"

WsJoystickButton::WsJoystickButton( std::string name, int port, 
                                    bool d_pad_button, int button_index )
    : DigitalInput( name, false )
    , ap_joystick( NULL )
    , a_buttonIndex( button_index )
    , a_dPadButton( d_pad_button )
    , a_port( port )
{
}

WsJoystickButton::WsJoystickButton( std::string name )
    : DigitalInput( name, false )
    , ap_joystick( NULL )
    , a_buttonIndex( 0 )
    , a_dPadButton( false )
    , a_port( 0 )
{
    CONFIG_VAR( "port", a_port, int );
    CONFIG_VAR( "button_index", a_buttonIndex, int );
    CONFIG_VAR( "d_pad", a_dPadButton, bool );
}

WsJoystickButton::~WsJoystickButton( void )
{
    if( ap_joystick != NULL )
    {
        delete ap_joystick;
        ap_joystick = NULL;
    }
}

int WsJoystickButton::instantiate( void )
{
    if( ap_joystick != NULL ) delete ap_joystick;
    ap_joystick = new frc::Joystick( a_port );
    return 0;
}

bool WsJoystickButton::readRawValue( void )
{
    bool value = false;

    if( a_dPadButton )
    {
        double axis = 0.0;
        switch( a_buttonIndex )
        {
            case 0:
                axis = ap_joystick->GetPOV();
                value = ( axis == 270 );
                break;

            case 1:
                axis = ap_joystick->GetPOV();
                value = ( axis == 90 );
                break;

            case 2:
                axis = ap_joystick->GetPOV();
                value = ( axis == 0 );
                break;

            case 3:
                axis = ap_joystick->GetPOV();;
                value = ( axis == 180 );
                break;

            default:
                WS_WARNING( "d-pad only recognizes buttons 0-3, not %d", 
                            a_buttonIndex );
                break;
        }
    }
    else
    {
        value = ap_joystick->GetRawButton( a_buttonIndex + 1 );
    }
    
    return value;
}

