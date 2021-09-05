#include "WsProtocol.h"

#include "logger/WsLogger.h"
#include "net/WsSocket.h"

WsProtocol::WsProtocol( WsSocket* p_socket )
    : ap_socket( p_socket )
{
}

WsProtocol::WsProtocol( int port, size_t buffer_size )
    : ap_socket( NULL )
{
    ap_socket = new WsServerSocket( port, buffer_size, this );
}

WsProtocol::~WsProtocol( void )
{
    delete ap_socket;
    ap_socket = NULL;
}

size_t WsProtocol::handleReceive( char* buf, size_t bytes )
{
    WS_WARNING( "%s not implemented.", __PRETTY_FUNCTION__ );
    return 0;
}

int WsProtocol::sendBytes( const char* buf, size_t bytes )
{
    return ap_socket->sendBytes( buf, bytes );
}

int WsProtocol::startSocket( void )
{
    return ap_socket->startSocket();
}

