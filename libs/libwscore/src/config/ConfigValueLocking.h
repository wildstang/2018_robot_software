#ifndef __CONFIGVALUELOCKING_H__
#define __CONFIGVALUELOCKING_H__

#include "config/ConfigValue.h"

#include <pthread.h>

#define CONFIG_VAR_LOCK( name, type, var, value ) \
        { \
            var = new ConfigValueLocking< type >( name, this, value ); \
            this->addConfigValue( p_var ); \
        }

class Subsystem;

template< typename T >
class ConfigValueLocking
{
    public:
        ConfigValueLocking( std::string name, Subsystem* p_sub, T default_value );
        ConfigValueLocking( const char* name, Subsystem* p_sub, T default_value );
        virtual ~ConfigValueLocking( void );

        virtual int updateValue( const std::string& rc_value );
        virtual int getValue( std::string& r_value );

        T get( void );
        int set( T value );

    protected:
        pthread_rwlock_t a_rwlock;
        T a_value;
};

#define __CONFIGVALUELOCKING_H__TEMPLATE__
#include "config/ConfigValueLocking.cpp"
#undef __CONFIGVALUELOCKING_H__TEMPLATE__

#endif

