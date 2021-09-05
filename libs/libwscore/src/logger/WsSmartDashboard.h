#ifndef __WSSMARTDASHBOARD_H__
#define __WSSMARTDASHBOARD_H__

#include "core/RobotCore.h"

//
// Smart Dashboard
//
#define SD_PUT( ... ) \
    RobotCore::getRobotCore()->getSmartDashboard()->put( __VA_ARGS__ ); \

#include <string>

class WsSmartDashboard
{
    public:
        WsSmartDashboard( void );
        virtual ~WsSmartDashboard( void );

        virtual int put( std::string key, int value );
        virtual int put( std::string key, double value );
        virtual int put( std::string key, bool value );
        virtual int put( std::string key, std::string value );

        inline int put( const char* key, int value )
            { return this->put( std::string( key ), value ); }
        inline int put( const char* key, double value )
            { return this->put( std::string( key ), value ); }
        inline int put( const char* key, bool value )
            { return this->put( std::string( key ), value ); }
        inline int put( const char* key, std::string value )
            { return this->put( std::string( key ), value ); }
        inline int put( std::string key, const char* value )
            { return this->put( key, std::string( value ) ); }
        inline int put( const char* key, const char* value )
            { return this->put( std::string( key ), std::string( value ) ); }
        inline int put( std::string key, uint64_t value )
            { return this->put( key, ( int )( 0x00000000FFFFFFFF & value ) ); }
        inline int put( const char* key, uint64_t value )
            { return this->put( std::string( key ), value ); }
};

#endif

