#include "WsDoubleSolenoid.h"

#include "DoubleSolenoid.h"

#include "config/ConfigValueRef.h"

WsDoubleSolenoid::WsDoubleSolenoid( std::string name, 
                                    int module, 
                                    int channel1, 
                                    int channel2, 
                                    wsDoubleSolenoidStateT default_value )
    : DiscreteOutput( name, ( int )default_value )
    , ap_solenoid( NULL )
    , a_module( module )
    , a_firstChannel( channel1 )
    , a_secondChannel( channel2 )
{
}

WsDoubleSolenoid::WsDoubleSolenoid( std::string name )
    : DiscreteOutput( name, 0 )
    , ap_solenoid( NULL )
    , a_module( 0 )
    , a_firstChannel( 0 )
    , a_secondChannel( 0 )
{
    CONFIG_VAR( "module", a_module, int );
    CONFIG_VAR( "first_channel", a_firstChannel, int );
    CONFIG_VAR( "second_channel", a_secondChannel, int );
}

WsDoubleSolenoid::~WsDoubleSolenoid( void )
{
    if( ap_solenoid != NULL )
    {
        delete ap_solenoid;
        ap_solenoid = NULL;
    }
}

int WsDoubleSolenoid::instantiate( void )
{
    if( ap_solenoid != NULL ) delete ap_solenoid;
    ap_solenoid = new frc::DoubleSolenoid( a_module, a_firstChannel, a_secondChannel );
    return 0;
}

void WsDoubleSolenoid::sendDataToOutput( void )
{
    enum frc::DoubleSolenoid::Value solValue = frc::DoubleSolenoid::Value::kOff;

    wsDoubleSolenoidStateT state = 
        ( wsDoubleSolenoidStateT )( this->getValue() );

    if( state == WS_DOUBLE_SOLENOID_STATE_FORWARD )
    {
        solValue = frc::DoubleSolenoid::Value::kForward;
    }
    else if( state == WS_DOUBLE_SOLENOID_STATE_REVERSE )
    {
        solValue = frc::DoubleSolenoid::Value::kReverse;
    }

    ap_solenoid->Set( solValue );
}

