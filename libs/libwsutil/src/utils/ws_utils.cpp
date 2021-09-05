#include "ws_utils.h"

#include <sys/time.h>

#include "ws_string.h"

uint64_t ws_milliseconds( void )
{
    return ws_microseconds() / 1000;
}

uint64_t ws_microseconds( void )
{
    struct timeval tv;
    gettimeofday( &tv, NULL );

    uint64_t usec = ( tv.tv_sec * 1000000 ) +
                      tv.tv_usec;

    return usec;
}

uint64_t ws_seconds( void )
{
    struct timeval tv;
    gettimeofday( &tv, NULL );

    uint64_t sec = tv.tv_sec;
    return sec;
}

int ws_make_index( const std::vector< std::string > rc_arr,
                   std::map< std::string, unsigned int > r_idx )
{
    r_idx.clear();

    for( unsigned int i = 0; i < rc_arr.size(); ++i )
    {
        r_idx[ rc_arr[ i ] ] = i;
    }

    return 0;
}

std::string ws_format_seconds( uint64_t s )
{
    uint64_t m = s / 60;
    s = s % 60;

    uint64_t h = m / 60;
    m = m % 60;

    uint64_t d = h / 24;
    h = h % 24;

    uint64_t y = d / 365;
    d = d % 365;

    return ws_std_string_printf( "%s yrs %s day %s hrs %s min %s sec",
                                 toString( y ).c_str(), toString( d ).c_str(),
                                 toString( h ).c_str(), toString( m ).c_str(),
                                 toString( s ).c_str() );
}

