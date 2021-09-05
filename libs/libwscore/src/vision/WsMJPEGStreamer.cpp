#include "WsMJPEGStreamer.h"

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

#include "utils/StopWatch.h"
#include "vision/SocketWorker.h"
#include "logger/WsLogger.h"

const int WsMJPEGStreamer::sc_FPS = 1000 / 10;
const int WsMJPEGStreamer::sc_PORT = 8887;

WsMJPEGStreamer::WsMJPEGStreamer( void )
    : Subsystem( std::string( "WsMJPEGStreamer" ) )
    , a_running( false )
    , ap_stopwatch( NULL )
    , a_thread( 0 )
    , a_mutex( PTHREAD_MUTEX_INITIALIZER )
    , a_socketWorkers()
    , a_pendingCleanup()
{
    pthread_mutex_init( &a_mutex, NULL );
}

WsMJPEGStreamer::~WsMJPEGStreamer( void )
{
    if( ap_stopwatch != NULL )
    {
        delete ap_stopwatch;
        ap_stopwatch = NULL;
    }

    pthread_mutex_lock( &a_mutex );
    a_running = false;
    pthread_mutex_unlock( &a_mutex );

    void* ret_val = NULL;
    pthread_join( a_thread, &ret_val );

    flushPendingCleanupQueue();

    std::set< SocketWorker* >::iterator i = a_socketWorkers.begin();
    for(; i != a_socketWorkers.end(); ++i )
    {
        SocketWorker* p_worker = *i;
        
        p_worker->stop();
        delete p_worker;
    }

    a_socketWorkers.clear();

    pthread_mutex_destroy( &a_mutex );
}

void WsMJPEGStreamer::init( void )
{
    ap_stopwatch = new StopWatch();
    
    if( !a_running )
    {
        a_running = true;

        int err = pthread_create( &a_thread, NULL, WsMJPEGStreamer::run, ( void* )this );

        if( err )
        {
            WS_ERROR( "Error initializing socket listener thread." );
        }
    }
}

void WsMJPEGStreamer::selfTest( void )
{
}

void WsMJPEGStreamer::update( void )
{
    flushPendingCleanupQueue();
}

void WsMJPEGStreamer::send( Mat* img )
{
    // SWH TODO
#if 0
    // Convert from mat to byte stream
    if( img != NULL && !img->empty() )
    {
        std::vector< unsigned char > bytes;
        Highgui.imencode( ".jpg", img, bytes );
        
        std::set< SocketWorker* >::iterator i = a_socketWorkers.begin();
        for(; i != a_socketWorkers.end(); ++i )
        {
            SocketWorker* p_worker = *i;
            p_worker->send( bytes );
        }
    }
#endif
}

int WsMJPEGStreamer::getPort( void )
{
    return sc_PORT;
}

void* WsMJPEGStreamer::run( void* arg )
{
    WsMJPEGStreamer* p_streamer = ( WsMJPEGStreamer* )arg;
    if( p_streamer == NULL ) return NULL;
    p_streamer->run();
    return NULL;
}

void WsMJPEGStreamer::run( void )
{
    int listen_fd = 0;
    int connection_fd = 0;
    struct sockaddr_in server_addr;

    listen_fd = socket( AF_INET, SOCK_STREAM, 0 );
    memset( &server_addr, '0', sizeof( server_addr ) );

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl( INADDR_ANY );
    server_addr.sin_port = htons( sc_PORT );

    bind( listen_fd, ( struct sockaddr* )&server_addr, sizeof( server_addr ) );

    listen( listen_fd, 10 );

    bool running = true;
    a_running = true;
    int counter = 0;

    while( running )
    {
        // wait for client connection
        WS_INFO( "Waiting for connection" );

        // blocks until a new connection is ready to accept.
        connection_fd = accept( listen_fd, ( struct sockaddr* )NULL, NULL );
        // TODO: set tcp no delay?

        WS_INFO( "Connection received." );
        //WS_INFO( "Connection received from %s",
        //         connection.getInetAddress().getHostName() );

        SocketWorker* sw = new SocketWorker( counter++, connection_fd, this );
        sw->go();

        a_socketWorkers.insert( sw );

        pthread_mutex_lock( &a_mutex );
        running = a_running;
        pthread_mutex_unlock( &a_mutex );
    }
}

void WsMJPEGStreamer::finish( SocketWorker* p_worker )
{
    pthread_mutex_lock( &a_mutex );
    a_pendingCleanup.push_back( p_worker );
    pthread_mutex_unlock( &a_mutex );
}

void WsMJPEGStreamer::flushPendingCleanupQueue( void )
{
    pthread_mutex_lock( &a_mutex );

    std::vector< SocketWorker* >::iterator i = a_pendingCleanup.begin();
    for(; i != a_pendingCleanup.end(); ++i )
    {
        SocketWorker* p_worker = *i;

        a_socketWorkers.erase( p_worker );
        p_worker->stop();
        delete p_worker;
    }

    a_pendingCleanup.clear();

    pthread_mutex_unlock( &a_mutex );
}

void WsMJPEGStreamer::resetState( void )
{
}

