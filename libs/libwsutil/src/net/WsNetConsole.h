#ifndef __WSNETCONSOLE_H__
#define __WSNETCONSOLE_H__

#include "net/WsProtocol.h"

class WsNetConsole : public WsProtocol
{
    public:
        WsNetConsole( int port );
        WsNetConsole( WsSocket* p_socket );
        virtual ~WsNetConsole( void );

        //
        // if you directly inherit from WsNetConsole
        // you are expected to implement your own mutex
        //

    protected:
        virtual size_t handleReceive( char* buf, size_t bytes ) override;

        int net_printf( const char* fmt, ... );
        int net_vprintf( const char* fmt, va_list args );
        
        virtual int handleLine( const char* line ) = 0;
};

#endif

