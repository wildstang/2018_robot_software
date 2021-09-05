#include "WsNetConsole.h"

#include <cstring>
#include <cstdio>

#include "logger/WsLogger.h"

#define WS_PRINTF_BUFF_SIZE 2048

WsNetConsole::WsNetConsole( int port )
    : WsProtocol( port, 2048 )
{
}

WsNetConsole::WsNetConsole( WsSocket* p_socket )
    : WsProtocol( p_socket )
{
}

WsNetConsole::~WsNetConsole( void )
{
}

size_t WsNetConsole::handleReceive( char* buf, size_t bytes )
{
    size_t bytes_used = 0;

    char* cr = buf;
    char* lf = buf;
    for(; ( *cr != '\r' ) && ( cr < ( buf + bytes ) ); cr++ );
    for(; ( *lf != '\n' ) && ( lf < ( buf + bytes ) ); lf++ );
    if( *cr != '\r' ) cr = NULL;
    if( *lf != '\n' ) lf = NULL;

    if( !( cr || lf ) ) return 0;
    if( cr && !lf ) lf = cr;
    if( lf && !cr ) cr = lf;

    bytes_used = 1 + lf - buf;

    if( bytes_used > bytes )
    {
        // Do not consume more bytes than are available.
        // should never happen.
        return 0;
    }

    *cr = '\0';

    int err = this->handleLine( buf );
    if( err )
    {
        net_printf( "** fail\n" );
    }
    else
    {
        net_printf( "** ok\n" );
    }

    return bytes_used;
}

int WsNetConsole::net_printf( const char* fmt, ... )
{
    va_list args;
    va_start( args, fmt );

    return net_vprintf( fmt, args );
}

int WsNetConsole::net_vprintf( const char* fmt, va_list args )
{
    char buff[ WS_PRINTF_BUFF_SIZE ];
    memset( buff, 0x00, sizeof( buff ) );
    vsnprintf( buff, WS_PRINTF_BUFF_SIZE - 1, fmt, args );
    return this->sendBytes( buff, ( size_t )strlen( buff ) );
}

