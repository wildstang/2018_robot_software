#include "OutputSource.h"

#include "io/Output.h"

#ifndef __OUTPUTSOURCE_H__TEMPLATE__

OutputSource::OutputSource( void )
{
}

OutputSource::~OutputSource( void )
{
}

#else

template< typename T >
ReferenceOutputSource< T >::ReferenceOutputSource( T* p_value )
    : OutputSource()
    , ap_value( p_value )
{
}

template< typename T >
ReferenceOutputSource< T >::~ReferenceOutputSource( void )
{
}

template< typename T >
void ReferenceOutputSource< T >::pushToOutput( Output* p_output )
{
    p_output->setValue( *ap_value );
}

#endif

