#include "Probe.h"

#include "utils/ws_string.h"
#include "utils/ws_debug.h"

#ifndef __PROBE_H__TEMPLATE__

Probe::Probe( std::string name )
    : ac_name( name )
{
    WS_ASSERT( ws_is_well_formed_name( name.c_str() ) );
}

Probe::~Probe( void )
{
}

#else

template< typename T >
ProbeValue< T >::ProbeValue( std::string name, T default_value )
    : Probe( name )
    , a_value( default_value )
{
}

template< typename T >
ProbeValue< T >::~ProbeValue( void )
{
}

template< typename T >
std::string ProbeValue< T >::get( void )
{
    return toString( a_value );
}

template< typename T >
void ProbeValue< T >::update( T value )
{
    a_value = value;
}

#endif

