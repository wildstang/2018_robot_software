#ifndef __CRIO_UTILS_H__
#define __CRIO_UTILS_H__

#include "I2C.h"
#include "Relay.h"

enum frc::I2C::Port parse_i2c_port( const std::string& rc_str );
enum frc::Relay::Direction parse_relay_dir( const std::string& rc_str );

#endif

