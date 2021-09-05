#include "InputListenerValue.h"

#ifndef __INPUTLISTENERVALUE_H__TEMPLATE__

//
// Non-template code
//

#else

//
// template code
//

#include "utils/ws_debug.h"
#include <cstdlib>

template< typename T >
InputListenerValue< T >::InputListenerValue( T* p_value_ref )
    : InputListener()
    , ap_valueRef( p_value_ref )
{
    WS_ASSERT( ap_valueRef != NULL );
}

template< typename T >
InputListenerValue< T >::~InputListenerValue( void )
{
}

template< typename T >
void InputListenerValue< T >::inputUpdate( Input* p_source, T value )
{
    *ap_valueRef = value;
}

#endif

