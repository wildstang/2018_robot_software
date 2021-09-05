#include "InputTemplate.h"

#include "logger/WsLogger.h"
#include "utils/ws_string.h"
#include "core/RobotCore.h"
#include "io/routing/InputListener.h"
#include "io/inserts/WsIOInsert.h"

#include <cstdlib>
#include <cstring>
#include <cmath>

#define DOUBLE_COMPARISON_THRESH 0.0001

#ifndef __INPUTTEMPLATE_H__TEMPLATE__

bool __InputTemplate__compare( const std::vector< unsigned char >& rc_lhs,
                               const std::vector< unsigned char >& rc_rhs )
{
    bool identical = true;

    if( rc_lhs.size() != rc_rhs.size() ) identical = false;

    if( identical )
    {
        std::vector< unsigned char >::const_iterator i = rc_lhs.cbegin();
        std::vector< unsigned char >::const_iterator j = rc_rhs.cbegin();

        for(; ( i != rc_lhs.cend() ) &&
              ( j != rc_rhs.cend() ); ++i, ++j )
        {
            if( *i != *j )
            {
                identical = false;
                break;
            }
        }
    }

    return identical;
}

bool __InputTemplate__compare( const double& rc_lhs, const double& rc_rhs )
{
    return ( fabs( rc_lhs - rc_rhs ) < DOUBLE_COMPARISON_THRESH );
}

bool __InputTemplate__compare( const bool& rc_lhs, const bool& rc_rhs )
{
    return ( rc_lhs == rc_rhs );
}

bool __InputTemplate__compare( const int& rc_lhs, const int& rc_rhs )
{
    return ( rc_lhs == rc_rhs );
}

#else

template< typename T >
InputTemplate< T >::InputTemplate( std::string name, T default_value )
    : Input( RobotCore::getRobotCore()->getInputManager(), name )
    , a_currentValue( default_value )
    , a_valueProbe( "value", default_value )
    , a_beforeInsertProbe( "pre_insert", default_value )
{
    this->addProbe( &a_valueProbe );
    this->addProbe( &a_beforeInsertProbe );
}

template< typename T >
InputTemplate< T >::~InputTemplate( void )
{
}

template< typename T >
void InputTemplate< T >::readDataFromInput( void )
{
    T value = readRawValue();

    T processed_value = this->processInserts( value );

    a_beforeInsertProbe.update( value );
    a_valueProbe.update( processed_value );

    a_valueChanged = !__InputTemplate__compare( processed_value, a_currentValue );
    if ( a_valueChanged )
    {
        a_currentValue = processed_value;
    }
}

template< typename T >
T InputTemplate< T >::getValue( void )
{
    return a_currentValue;
}

template< typename T >
void InputTemplate< T >::notifyListener( InputListener* p_listener )
{
    if( p_listener == NULL ) return;
    p_listener->inputUpdate( this, a_currentValue );
}

template< typename T >
T InputTemplate< T >::processInserts( T value )
{
    std::vector< WsIOInsert* >::iterator i = a_inserts.begin();
    for(; i != a_inserts.end(); ++i )
    {
        value = ( *i )->process( value );
    }

    return value;
}

#endif

