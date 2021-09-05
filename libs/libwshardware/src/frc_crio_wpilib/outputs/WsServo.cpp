#include "WsServo.h"

#include "Servo.h"

#include "config/ConfigValueRef.h"

WsServo::WsServo( std::string name, int channel, double default_value )
    : AnalogOutput( name, default_value )
    , ap_servo( NULL )
    , a_channel( channel )
{
}

WsServo::WsServo( std::string name )
    : AnalogOutput( name, 0.0 )
    , ap_servo( NULL )
    , a_channel( 0 )
{
    CONFIG_VAR( "channel", a_channel, int );
}

WsServo::~WsServo( void )
{
    if( ap_servo != NULL )
    {
        delete ap_servo;
        ap_servo = NULL;
    }
}

int WsServo::instantiate( void )
{
    if( ap_servo != NULL ) delete ap_servo;
    ap_servo = new frc::Servo( a_channel );
    return 0;
}

void WsServo::sendDataToOutput( void )
{
    ap_servo->SetAngle( this->getValue() );
}

