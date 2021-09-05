#include "WsRelay.h"

#include "config/ConfigValueRef.h"
#include "crio_utils.h"

WsRelay::WsRelay( std::string name, int channel, 
                  enum frc::Relay::Direction direction )
    : DiscreteOutput( name, 0 )
    , ap_relay( NULL )
    , a_channel( channel )
    , a_direction( direction )
    , a_directionStr()
    , ac_factoryConstructor( false )
{
}

WsRelay::WsRelay( std::string name )
    : DiscreteOutput( name, 0 )
    , ap_relay( NULL )
    , a_channel( 0 )
    , a_direction( frc::Relay::Direction::kForwardOnly )
    , a_directionStr()
    , ac_factoryConstructor( true )
{
    CONFIG_VAR( "direction", a_directionStr, std::string );
    CONFIG_VAR( "channel", a_channel, int );
}

WsRelay::~WsRelay( void )
{
    if( ap_relay != NULL )
    {
        delete ap_relay;
        ap_relay = NULL;
    }
}

int WsRelay::instantiate( void )
{
    if( ac_factoryConstructor )
    {
        a_direction = parse_relay_dir( a_directionStr );
    }

    if( ap_relay != NULL ) delete ap_relay;
    ap_relay = new frc::Relay( a_channel, a_direction );
    return 0;
}

void WsRelay::sendDataToOutput( void )
{
    frc::Relay::Value value = frc::Relay::Value::kOff;
    wsRelayStateT state = ( wsRelayStateT )( this->getValue() );

    switch( state )
    {
        case WS_RELAY_STATE_ON:
            value = frc::Relay::Value::kOn;
            break;
        case WS_RELAY_STATE_FORWARD:
            value = frc::Relay::Value::kForward;
            break;
        case WS_RELAY_STATE_REVERSE:
            value = frc::Relay::Value::kReverse;
            break;
        case WS_RELAY_STATE_NONE:
        case WS_RELAY_STATE_NUM:
        default:
            WS_ERROR( "bad relay state: %d", ( int )state );
            break;
    }

    ap_relay->Set( value );
}

