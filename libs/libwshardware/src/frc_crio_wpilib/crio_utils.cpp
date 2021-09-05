#include "crio_utils.h"

#include <cstring>

#include "utils/ws_string.h"
#include "logger/WsLogger.h"

enum frc::I2C::Port parse_i2c_port( const std::string& rc_str )
{
    enum frc::I2C::Port p = frc::I2C::kOnboard;
    std::string copy( rc_str );

    ws_to_lower( copy );

    if( 0 == strncmp( copy.c_str(), "onboard", 7 ) )
    {
        p = frc::I2C::kOnboard;
    }
    else if( 0 == strncmp( copy.c_str(), "mxp", 3 ) )
    {
        p = frc::I2C::kMXP;
    }
    else
    {
        WS_ERROR( "invalid value for I2C: %s, options are onboard or mxp", copy.c_str() );
    }

    return p;
}

enum frc::Relay::Direction parse_relay_dir( const std::string& rc_str )
{
    enum frc::Relay::Direction d = frc::Relay::kForwardOnly;
    std::string copy( rc_str );

    ws_to_lower( copy );

    if( 0 == strncmp( copy.c_str(), "both", 4 ) )
    {
        d = frc::Relay::kBothDirections;
    }
    else if( 0 == strncmp( copy.c_str(), "forward", 7 ) )
    {
        d = frc::Relay::kForwardOnly;
    }
    else if( 0 == strncmp( copy.c_str(), "reverse", 7 ) )
    {
        d = frc::Relay::kReverseOnly;
    }
    else
    {
        WS_ERROR( "invalid value for relay dir: %s, options are forward reverse or both",
            copy.c_str() );
    }

    return d;
}

