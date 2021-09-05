#include "StepFactory.h"

#include "utils/ws_debug.h"

#ifndef __STEPFACTORY_H__TEMPLATE__

StepFactory::StepFactory( std::string name )
    : a_name( name )
{
    WS_ASSERT( ws_is_well_formed_name( name.c_str() ) );
}

StepFactory::~StepFactory( void )
{
}

#else

template< typename T > __StepFactory< T >::__StepFactory( std::string name )
    : StepFactory( name )
{
}

template< typename T > __StepFactory< T >::~__StepFactory( void )
{
}

template< typename T > AutoStep* __StepFactory< T >::createStep( AutoStepFactoryArgs* p_args )
{
    T* p_t = new T( p_args );

    if( p_args->errorsOccurred() )
    {
        delete p_t;
        return NULL;
    }
    
    return p_t;
}

#endif

