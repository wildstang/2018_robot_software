#ifndef __WSPROTOCOL_H__
#define __WSPROTOCOL_H__

#include "net/WsSocket.h"

#include <stdint.h>

class WsProtocol
{
    friend class WsSocket;

    public:
        WsProtocol( WsSocket* p_socket );
        WsProtocol( int port, size_t buffer_size );
        virtual ~WsProtocol( void );

        //
        // you are expected to implement your own mutex
        // in this case
        //

        int startSocket( void );

    protected:
        virtual size_t handleReceive( char* buf, size_t bytes );
        int sendBytes( const char* buf, size_t bytes );

        inline bool isConnected( void ) { return ap_socket->isConnected(); }

    private:
        WsSocket* ap_socket;
};

#endif

