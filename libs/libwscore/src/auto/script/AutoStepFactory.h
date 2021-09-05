#ifndef __AUTOSTEPFACTORY_H__
#define __AUTOSTEPFACTORY_H__

#include <string>
#include <map>
#include <vector>
#include <deque>

#include "utils/ws_string.h"

#define WS_DICTIONARY_ENUM( my_enum ) \
        { \
            std::string __key = std::string( #my_enum ); \
            ws_to_lower( __key ); \
            WS_ASSERT( ws_is_well_formed_name( __key.c_str() ) ); \
            this->addDictionaryValue( __key, ( int )( my_enum ) ); \
        }

class StepFactory;
class AutoStep;
class AutoStepGroup;

class AutoStepFactory
{
    public:
        AutoStepFactory( void );
        virtual ~AutoStepFactory( void );

        virtual int createStepFactories( void ) = 0;
        
        std::vector< std::string > getInstalledSteps( void );
        std::string getStepUsage( const std::string& rc_step );

        AutoStep* createStep( std::deque< std::string >& r_tok );
        AutoStepGroup* createGroup( std::deque< std::string >& r_tok );

        int getDictionaryValue( const std::string& rc_key, std::string& r_value );
        std::vector< std::string > listDictionaryKeys( void );

    protected:
        std::map< std::string, StepFactory* > a_factories;
        std::map< std::string, std::string > a_dictionary;

        int addStepFactory( StepFactory* p_factory );

        int addDictionaryValue( const std::string c_key, const std::string c_value );
        inline int addDictionaryValue( const std::string c_key, const int c_value )
            { return this->addDictionaryValue( c_key, toString( c_value ) ); }
        inline int addDictionaryValue( const std::string c_key, const double c_value )
            { return this->addDictionaryValue( c_key, toString( c_value ) ); }
        inline int addDictionaryValue( const char* p_key, const std::string c_value )
            { return this->addDictionaryValue( std::string( p_key ), c_value ); }
        inline int addDictionaryValue( const std::string c_key, const char* p_value )
            { return this->addDictionaryValue( c_key, std::string( p_value ) ); }
        inline int addDictionaryValue( const char* p_key, const char* p_value )
            { return this->addDictionaryValue( std::string( p_key ), std::string( p_value ) ); }
        inline int addDictionaryValue( const char* p_key, const int c_value )
            { return this->addDictionaryValue( std::string( p_key ), c_value ); }
        inline int addDictionaryValue( const char* p_key, const double c_value )
            { return this->addDictionaryValue( std::string( p_key ), c_value ); }
};

#endif

