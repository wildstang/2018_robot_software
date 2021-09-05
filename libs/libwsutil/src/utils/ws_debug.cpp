#include "ws_debug.h"

#include <execinfo.h>
#include <cstdio>

#include "utils/ws_string.h"

void ws_hex_dump( const char* buf, size_t len )
{
    const char* section = buf;
    size_t previous_pos = 0;

    for( size_t i = 0; i < len; i++ )
    {
        if( ( i % 16 ) == 0 )
        {
            // be explicit, everybody benefits.
            int addr = ( int )( i & 0x00000000FFFFFFFF );
            printf( "%08x: ", addr );
        }

        if( ( i % 4 ) == 0 ) putchar( ' ' );

        char x[ 3 ] = { 0, 0, 0 };
        ws_to_hex( x, buf[ i ] );

        printf( "%s", x );

        int last_run = ( ( i + 1 ) >= len ) ? 1 : 0;
        int last_char = ( ( i + 1 ) % 16 ) == 0;
        if( last_char || last_run )
        {
            printf( " |" );
            for(; previous_pos < ( i + 1 ); previous_pos++ )
            {
                char c = *( section++ );
                if( !isprint( c ) ) c = '.';
                putchar( c );
            }
            printf( "|\n" );
        }
    }
}

std::string ws_dbg_preview( std::deque< std::string > d, size_t n )
{
    std::string s;

    size_t j = 0;
    std::deque< std::string >::iterator i = d.begin();
    for(; ( j < n ) && ( i != d.end() ); ++i, ++j )
    {
        s.append( *i );
        s.append( ", " );
    }

    return s;
}

void ws_bt( void )
{
    void* bt[ 512 ];
    int s = backtrace( bt, 512 );

    char** symbols = backtrace_symbols( bt, s );

    for( int i = 0; i < s; i++ )
    {
        puts( symbols[ i ] );
    }
}

