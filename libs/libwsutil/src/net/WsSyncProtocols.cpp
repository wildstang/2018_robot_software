#include "WsSyncProtocols.h"

#include <cstring>

#include "logger/WsLogger.h"

WsSynchronizedProtocol::WsSynchronizedProtocol( int port, size_t buffer_size )
    : WsProtocol( port, buffer_size )
    , a_mutex( PTHREAD_MUTEX_INITIALIZER )
    , ap_buffer( NULL )
    , a_bytes( 0 )
    , a_bufferSize( buffer_size )
{
    pthread_mutex_init( &a_mutex, NULL );
    ap_buffer = ( char* )malloc( buffer_size * sizeof( char ) );
}

WsSynchronizedProtocol::~WsSynchronizedProtocol( void )
{
    free( ap_buffer );
    pthread_mutex_destroy( &a_mutex );
}

int WsSynchronizedProtocol::update( void )
{
    size_t bytes_used = 0;

    pthread_mutex_lock( &a_mutex );
    do
    {
        bytes_used = this->syncHandleReceive( ap_buffer, a_bytes );
        if( bytes_used >= a_bytes )
        {
            WS_ERROR( "bytes_used > a_bytes" );
            bytes_used = a_bytes;
        }

        for( int i = 0; i + bytes_used < a_bytes; i++ )
        {
            ap_buffer[ i ] = ap_buffer[ i + bytes_used ];
        }
        a_bytes -= bytes_used;
    } while( bytes_used > 0 );
    pthread_mutex_unlock( &a_mutex );

    return 0;
}

size_t WsSynchronizedProtocol::handleReceive( char* buf, size_t bytes )
{
    size_t ret_bytes = 0;

    pthread_mutex_lock( &a_mutex );
    if( bytes < ( a_bufferSize - a_bytes ) )
    {
        memcpy( ap_buffer + a_bytes, buf, bytes );
        ret_bytes = bytes;
        a_bytes += bytes;
    }
    else
    {
        memcpy( ap_buffer + a_bytes, buf, a_bufferSize - a_bytes );
        ret_bytes = a_bufferSize - a_bytes;
        a_bytes = a_bufferSize;
    }
    pthread_mutex_unlock( &a_mutex );

    return ret_bytes;
}

WsSynchronizedNetConsole::WsSynchronizedNetConsole( int port )
    : WsNetConsole( port )
    , a_mutex( PTHREAD_MUTEX_INITIALIZER )
    , a_cond( PTHREAD_COND_INITIALIZER )
    , a_line()
    , a_hasData( false )
{
    pthread_mutex_init( &a_mutex, NULL );
    pthread_cond_init( &a_cond, NULL );
}

WsSynchronizedNetConsole::~WsSynchronizedNetConsole( void )
{
    pthread_cond_destroy( &a_cond );
    pthread_mutex_destroy( &a_mutex );
}

int WsSynchronizedNetConsole::update( void )
{
    bool data = false;
    pthread_mutex_lock( &a_mutex );
    data = a_hasData;
    pthread_mutex_unlock( &a_mutex );

    int err = 0;
    if( data )
    {
        pthread_mutex_lock( &a_mutex );
        err = this->syncHandleLine( a_line.c_str() );
        a_hasData = false;
        a_ret = err;
        pthread_mutex_unlock( &a_mutex );

        pthread_cond_signal( &a_cond );
    }

    return err;
}

int WsSynchronizedNetConsole::handleLine( const char* line )
{
    int err = 0;

    pthread_mutex_lock( &a_mutex );
    a_line = std::string( line );
    a_hasData = true;

    pthread_cond_wait( &a_cond, &a_mutex );
    err = a_ret;
    pthread_mutex_unlock( &a_mutex );

    return err;
}

WsSynchronizedNetPacket::WsSynchronizedNetPacket( int port, 
                                                  size_t buffer_size )
    : WsNetPacket( port, buffer_size )
    , a_mutex( PTHREAD_MUTEX_INITIALIZER )
    , a_frames()
{
}

WsSynchronizedNetPacket::~WsSynchronizedNetPacket( void )
{
}

int WsSynchronizedNetPacket::update( void )
{
    int err = 0;

    pthread_mutex_lock( &a_mutex );
    while( !a_frames.empty() )
    {
        syncPacketFramingDataT entry = a_frames.front();
        a_frames.pop_front();

        int r = this->syncHandlePacket( &( entry.header ), entry.buffer );
        if( r ) err++;
    }
    pthread_mutex_unlock( &a_mutex );

    return err;
}

int WsSynchronizedNetPacket::handlePacket( packetFramingHeaderT* p_header,
                                           const char* buffer )
{
    pthread_mutex_lock( &a_mutex );
    syncPacketFramingDataT entry;
    entry.header = *p_header;
    char* buf = ( char* )malloc( p_header->message_size * sizeof( char ) );
    memcpy( buf, buffer, p_header->message_size );
    entry.buffer = buf;
    a_frames.push_back( entry );
    pthread_mutex_unlock( &a_mutex );

    return 0;
}

