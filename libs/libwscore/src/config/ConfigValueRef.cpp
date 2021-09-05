#include "ConfigValueRef.h"

#include "utils/ws_string.h"
#include "utils/ws_debug.h"

#ifndef __CONFIGVALUEREF_H__TEMPLATE__

//
// non-template code
//

#else

//
// template code
//

template< typename T >
ConfigValueRef< T >::ConfigValueRef( std::string name, 
                                     T* p_value )
    : ConfigValue( name )
    , ap_value( p_value )
{
    WS_ASSERT( ap_value != NULL );
}

template< typename T >
ConfigValueRef< T >::ConfigValueRef( const char* name,
                                     T* p_value )
    : ConfigValue( std::string( name ) )
    , ap_value( p_value )
{
    WS_ASSERT( ap_value != NULL );
}

template< typename T >
ConfigValueRef< T >::~ConfigValueRef( void )
{
    ap_value = NULL;
}

template< typename T >
int ConfigValueRef< T >::updateValue( const std::string& rc_value )
{
    parse( rc_value, ap_value );
    return 0;
}

template< typename T >
int ConfigValueRef< T >::getValue( std::string& r_value )
{
    r_value.assign( toString( *ap_value ) );
    return 0;
}

#endif

