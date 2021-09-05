#include "WsSocket.h"

#include <netdb.h>
#include <cassert>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <sys/types.h>
#include <time.h>

#include <pthread.h>

#include "logger/WsLogger.h"
#include "net/WsProtocol.h"

#define WS_PRINTF_BUFF_SIZE 2048

WsSocket::WsSocket( int port, size_t buffer_size, WsProtocol* p_protocol )
    : a_running( false )
    , a_port( port )
    , a_bufferSize( buffer_size )
    , a_connectionFd( 0 )
    , a_thread( 0 )
    , a_mutex( PTHREAD_MUTEX_INITIALIZER )
    , a_connected( false )
    , ap_protocol( p_protocol )
{
    pthread_mutex_init( &a_mutex, NULL );
    assert( ap_protocol != NULL );
}

WsSocket::~WsSocket( void )
{
    a_running = false; // atomic write

    void* ret_val = NULL;
    pthread_join( a_thread, &ret_val );

    pthread_mutex_destroy( &a_mutex );
}

int WsSocket::startSocket( void )
{
    pthread_mutex_lock( &a_mutex );
    if( !a_running )
    {
        a_running = true;
        pthread_create( &a_thread, NULL, WsSocket::__run, ( void* )this );
    }
    pthread_mutex_unlock( &a_mutex );
    return 0;
}

int WsSocket::runConnection( int fd )
{
    int err = 0;
    a_connectionFd = fd;

    char* buffer = ( char* )malloc( a_bufferSize * sizeof( char ) );

    size_t bytes_received = 0;
    a_connected = true; // atomic write.
    while( a_connected && a_running )
    {
        fd_set fds;
        FD_ZERO( &fds );
        FD_SET( a_connectionFd, &fds );

        // 0.2 s
        struct timeval t;
        t.tv_sec = 0;
        t.tv_usec = 200 * 1000;

        err = select( a_connectionFd + 1, &fds, NULL, NULL, &t );
        if( err < 0 )
        {
            a_connected = false;
            WS_ERROR( "select error port %d", a_port );
            break;
        }

        if( FD_ISSET( a_connectionFd, &fds ) )
        {
            ssize_t rx = recv( a_connectionFd,
                               buffer + bytes_received, 
                               a_bufferSize - bytes_received, 0 );
            if( rx <= 0 )
            {
                a_connected = false;
                WS_ERROR( "an error occurred in recv port %d", a_port );
                err = 1;
                break;
            }

            bytes_received += ( size_t )rx;

            size_t bytes_used = 0;
            do
            {
                //hex_dump( buffer, bytes_received );

                bytes_used = 
                    this->handleReceive( buffer, bytes_received );
                if( bytes_used > bytes_received ) 
                {
                    int __bytes_used = ( int )bytes_used;
                    int __bytes_received = ( int )bytes_received;
                    WS_ERROR( "bytes_used( %d ) > bytes_received( %d )",
                            __bytes_used, __bytes_received );
                    bytes_used = bytes_received;
                }

                for( int i = 0; i + bytes_used < bytes_received; i++ )
                {
                    buffer[ i ] = buffer[ i + bytes_used ];
                }
                bytes_received -= bytes_used;
            } while( bytes_used > 0 );
        }
    }

    close( a_connectionFd );
    a_connected = false;
    a_connectionFd = -1;

    free( buffer );

    return err;
}

void* WsSocket::__run( void* arg )
{
    WsSocket* p_server = ( WsSocket* )arg;
    if( p_server == NULL ) return NULL;
    p_server->__run();
    return NULL;
}

void WsSocket::__run( void )
{
    int err = this->wsConnect();
    if( err )
    {
        WS_ERROR( "problem with connect function, port %d", a_port );
    }
}

size_t WsSocket::handleReceive( char* buf, size_t bytes )
{
    return ap_protocol->handleReceive( buf, bytes );
}

int WsSocket::sendBytes( const char* buf, size_t bytes )
{
    if( !a_connected ) return 1;

    ssize_t bytes_sent =
        send( a_connectionFd, buf, bytes, 0 );
    if( bytes_sent != ( ssize_t )bytes )
    {
        WS_ERROR( "was not able to send all of the data." );
        return 1;
    }

    return 0;
}

WsServerSocket::WsServerSocket( int port, size_t buffer_size, WsProtocol* p_protocol )
    : WsSocket( port, buffer_size, p_protocol )
{
}

WsServerSocket::~WsServerSocket( void )
{
}

int WsServerSocket::wsConnect( void )
{
    int listen_fd = 0;
    struct sockaddr_in server_addr;

    listen_fd = socket( AF_INET, SOCK_STREAM, 0 );
    if( listen_fd < 0 )
    {
        WS_ERROR( "unable to listen. (%d)", this->getPort() );
        this->halt();
        return 1;
    }

    memset( &server_addr, '0', sizeof( server_addr ) );

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl( INADDR_ANY );
    server_addr.sin_port = htons( this->getPort() );

    int err = bind( listen_fd, ( struct sockaddr* )&server_addr, 
                    sizeof( server_addr ) );
    if( err == -1 )
    {
        WS_ERROR( "error binding to listen fd (%d)", this->getPort() );
        this->halt();
        close( listen_fd );
        return 2;
    }

    err = listen( listen_fd, 10 );
    if( err == -1 )
    {
        WS_ERROR( "could not listen (%d)", this->getPort() );
        this->halt();
        close( listen_fd );
        return 3;
    }

    while( this->isRunning() )
    {
        WS_INFO( "Waiting for connection (%d)", this->getPort() );

        // blocks until a new connection is ready to accept.
        int fd = accept( listen_fd, ( struct sockaddr* )NULL, NULL );
        if( fd < 0 )
        {
            WS_ERROR( "Problem accepting connection (%d).", this->getPort() );
            usleep( 1000 * 1000 );
            continue;
        }

        WS_INFO( "Connection received. (%d)", this->getPort() );

        this->runConnection( fd );
    }

    return 0;
}

WsClientSocket::WsClientSocket( int port, size_t buffer_size, 
                                std::string host, WsProtocol* p_protocol )
    : WsSocket( port, buffer_size, p_protocol )
    , a_host( host )
{
    ws_trim( host );
}

WsClientSocket::~WsClientSocket( void )
{
}

int WsClientSocket::wsConnect( void )
{
    if( a_host.size() == 0 )
    {
        WS_ERROR( "specify a host to connect to (%d)", this->getPort() );
        this->halt();
        return 1;
    }

    struct addrinfo* p_addr_info;
    int err = getaddrinfo( a_host.c_str(), NULL, NULL, &p_addr_info );
    if( err )
    {
        WS_ERROR( "error with getaddrinfo (%d)", this->getPort() );
        this->halt();
        return err;
    }

    struct sockaddr* addr = p_addr_info->ai_addr;
    if( addr->sa_family != AF_INET )
    {
        WS_ERROR( "expected inet class sockaddr (%d)", this->getPort() );
        this->halt();
        return 2;
    }

    struct sockaddr_in* inet_addr = ( struct sockaddr_in* )addr;
    inet_addr->sin_family = AF_INET;
    inet_addr->sin_port = htons( this->getPort() );

    int sock_fd = socket( AF_INET, SOCK_STREAM, 0 );
    if( sock_fd < 0 )
    {
        WS_ERROR( "could not open socket. (%d)", this->getPort() );
        this->halt();
        return 3;
    }

    while( this->isRunning() )
    {
        WS_INFO( "connecting to %s:%d...", a_host.c_str(), this->getPort() );

        err = ::connect( sock_fd, addr, sizeof( struct sockaddr ) );
        if( err == -1 )
        {
            WS_ERROR( "error connecting socket (%d)", this->getPort() );
            usleep( 1000 * 1000 );
            continue;
        }

        WS_INFO( "Connection received. (%d)", this->getPort() );

        this->runConnection( sock_fd );
    }

    return 0;
}

