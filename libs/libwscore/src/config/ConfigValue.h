#ifndef __CONFIGVALUE_H__
#define __CONFIGVALUE_H__

#include <string>

class ConfigValue
{
    public:
        ConfigValue( std::string name );
        ConfigValue( const char* name );
        virtual ~ConfigValue( void );

        virtual int updateValue( const std::string& rc_value ) = 0;
        virtual int getValue( std::string& r_value ) = 0;

        inline std::string getName( void ) { return a_name; }

    protected:
        std::string a_name;
};

#endif

