#include "OutputTemplate.h"

#include "utils/ws_string.h"
#include "core/RobotCore.h"
#include "logger/WsLogger.h"
#include "io/inserts/WsIOInsert.h"

#ifndef __OUTPUTTEMPLATE_H__TEMPLATE__

//
// non template code
//
// of which there is none
//

#else

template< typename T >
OutputTemplate< T >::OutputTemplate( std::string name, T default_value )
    : Output( RobotCore::getRobotCore()->getOutputManager(), name )
    , a_value( default_value )
    , a_rawValue( default_value )
    , a_valueProbe( "value", default_value )
    , a_beforeInsertProbe( "pre_insert", default_value )
{
    this->addProbe( &a_valueProbe );
    this->addProbe( &a_beforeInsertProbe );
}

template< typename T >
OutputTemplate< T >::~OutputTemplate( void )
{
}

template< typename T >
T OutputTemplate< T >::getValue( void )
{
    return a_value;
}

template< typename T >
void OutputTemplate< T >::setValue( T value )
{
    a_rawValue = value;
    a_beforeInsertProbe.update( a_rawValue );
    a_value = this->processInserts( value );
    a_valueProbe.update( a_value );
}

template< typename T >
T OutputTemplate< T >::processInserts( T value )
{
    std::vector< WsIOInsert* >::iterator i = a_inserts.begin();
    for(; i != a_inserts.end(); ++i )
    {
        value = ( *i )->process( value );
    }

    return value;
}

#endif

