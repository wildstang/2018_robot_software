#ifndef __CONFIGVALUEREF_H__
#define __CONFIGVALUEREF_H__

#include "config/ConfigValue.h"

#define CONFIG_VAR( name, var, type ) \
        { \
            this->addConfigValue( new ConfigValueRef< type >( \
                name, &var ) ); \
        }

template< typename T >
class ConfigValueRef : public ConfigValue
{
    public:
        ConfigValueRef( std::string name, T* p_value );
        ConfigValueRef( const char* name, T* p_value );
        virtual ~ConfigValueRef( void );

        virtual int updateValue( const std::string& rc_value );
        virtual int getValue( std::string& r_value );

    protected:
        T* ap_value;
};

#define __CONFIGVALUEREF_H__TEMPLATE__
#include "ConfigValueRef.cpp"
#undef __CONFIGVALUEREF_H__TEMPLATE__

#endif

