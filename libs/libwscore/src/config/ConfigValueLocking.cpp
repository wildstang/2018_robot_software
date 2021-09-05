#include "ConfigValueLocking.h"

#ifdef __CONFIGVALUELOCKING_H__TEMPLATE__

template< typename T >
ConfigValueLocking< T >::ConfigValueLocking( std::string name, 
                                        Subsystem* p_sub,
                                        T default_value )
    : ConfigValue( name )
    , a_rwlock( PTHREAD_MUTEX_INITIALIZER )
    , a_value( default_value )
{
    pthread_rwlock_init( &a_rwlock, NULL );
}

template< typename T >
ConfigValueLocking< T >::ConfigValueLocking( const char* name,
                                        Subsystem* p_sub,
                                        T default_value )
    : ConfigValue( name )
    , a_rwlock( PTHREAD_MUTEX_INITIALIZER )
    , a_value( default_value )
{
    pthread_rwlock_init( &a_rwlock, NULL );
}

template< typename T >
ConfigValueLocking< T >::~ConfigValueLocking( void )
{
    pthread_rwlock_destroy( &a_rwlock );
}

template< typename T >
int ConfigValueLocking< T >::updateValue( const std::string& rc_value )
{
    int err = pthread_rwlock_wrlock( &a_rwlock );

    if( !err )
    {
        err += parse( rc_value, &a_value ) ? 1 : 0;

        err += pthread_rwlock_unlock( &a_rwlock ) ? 1 : 0;
    }

    return err;
}

template< typename T >
int ConfigValueLocking< T >::getValue( std::string& r_value )
{
    int err = pthread_rwlock_rdlock( &a_rwlock );

    if( !err )
    {
        r_value.assign( toString( a_value ) );

        err = pthread_rwlock_unlock( &a_rwlock );
    }

    return err;
}

template< typename T >
T ConfigValueLocking< T >::get( void )
{
    T value;
    int err = pthread_rwlock_rdlock( &a_rwlock );

    if( !err )
    {
        value = a_value;

        err = pthread_rwlock_unlock( &a_rwlock );
    }

    return value;
}

template< typename T >
int ConfigValueLocking< T >::set( T value )
{
    int err = pthread_rwlock_wrlock( &a_rwlock );

    if( !err )
    {
        a_value = value;

        err = pthread_rwlock_unlock( &a_rwlock );
    }

    return err;
}

#endif

