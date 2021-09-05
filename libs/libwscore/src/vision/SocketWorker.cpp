#include "SocketWorker.h"

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

#include "vision/WsMJPEGStreamer.h"
#include "logger/WsLogger.h"

const int SocketWorker::sc_SEND_BUFF_SIZE = 8000;

SocketWorker::SocketWorker( int counter, int socket_fd, WsMJPEGStreamer* ims )
    : a_fps( WsMJPEGStreamer::sc_FPS )
    , ap_ims( ims )
    , a_counter( counter )
    , a_sockFd( socket_fd )
    , a_running( false )
    , a_latestImage()
    , a_thread( 0 )
    , a_mutex( PTHREAD_MUTEX_INITIALIZER )
{
    pthread_mutex_init( &a_mutex, NULL );
}

SocketWorker::~SocketWorker( void )
{
}

void SocketWorker::writeHeader( void )
{
    const char* str = "HTTP/1.1 200 OK\r\n"
                      "Connection: close\r\n"
                      "Max-Age: 0\r\n"
                      "Expires: 0\r\n"
                      "Cache-Control: no-store, no-cache, must-revalidate, no-transform, pre-check=0, post-check=0, max-age=0\r\n"
                      "Pragma: no-cache\r\n"
                      "Content-Type: multipart/x-mixed-replace; boundary=--"
                      "~EOF~\r\n\r\n--~EOF~\r\n";
    ::send( a_sockFd, str, strlen( str ), 0 );
}

void SocketWorker::go( void )
{
    int err = pthread_create( &a_thread, NULL, &SocketWorker::entry, ( void* )this );
    if( err ) WS_ERROR( "error initializing pthread" );
}

void SocketWorker::stop( void )
{
    pthread_mutex_lock( &a_mutex );
    a_running = false;
    pthread_mutex_unlock( &a_mutex );

    pthread_join( a_thread, NULL );

    close( a_sockFd );

    pthread_mutex_destroy( &a_mutex );
}

void* SocketWorker::entry( void* arg )
{
    SocketWorker* p_worker = ( SocketWorker* )arg;
    p_worker->run();
    return NULL;
}

void SocketWorker::run( void )
{
    bool running = true;
    a_running = true;

    writeHeader();

    while( running )
    {
        pthread_mutex_lock( &a_mutex );
        {
            const unsigned char* const imageBytes = &( a_latestImage[ 0 ] );

            std::string str = 
                ws_std_string_printf( "Content-type: image/jpeg\r\n"
                                   "Content-Length: %d\r\n\r\n",
                                   a_latestImage.size() );

            ::send( a_sockFd, str.c_str(), str.length(), 0 );
            ::send( a_sockFd, imageBytes, a_latestImage.size(), 0 );

            const char* send_str = "\r\n--~EOF~\r\n";
            ::send( a_sockFd, send_str, strlen( send_str ), 0 );
        }
        pthread_mutex_unlock( &a_mutex );

        usleep( 1000000 / a_fps );

        pthread_mutex_lock( &a_mutex );
        running = a_running;
        pthread_mutex_unlock( &a_mutex );
    }

    close_connection();
}

void SocketWorker::close_connection( void )
{
    // safe to call from within your pthread if you like.
    ap_ims->finish( this );
    WS_INFO( "Client #%d is disconnected.", a_counter );
}

void SocketWorker::send( std::vector< unsigned char >& r_v )
{
    pthread_mutex_lock( &a_mutex );
    a_latestImage = r_v;
    pthread_mutex_unlock( &a_mutex );
}

