#include "ConfigValue.h"

#include "logger/WsLogger.h"

#include "utils/ws_debug.h"
#include "utils/ws_string.h"

ConfigValue::ConfigValue( std::string name )
    : a_name( name )
{
    WS_INFO( "registering config value %s", name.c_str() );
    WS_ASSERT( ws_is_well_formed_name( name.c_str() ) );
}

ConfigValue::ConfigValue( const char* name )
    : a_name( std::string( name ) )
{
    WS_INFO( "registering config value %s", name );
    WS_ASSERT( ws_is_well_formed_name( name ) );
}

ConfigValue::~ConfigValue( void )
{
}

