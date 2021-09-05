#ifndef __WS_UTILS_H__
#define __WS_UTILS_H__

#include <cstdint>
#include <cstdlib>
#include <ctype.h>

#include <vector>
#include <string>
#include <map>

#define PI 3.1415926535897

uint64_t ws_milliseconds( void );
uint64_t ws_microseconds( void );
uint64_t ws_seconds( void );

std::string ws_format_seconds( uint64_t s );

//
// note this function makes no attempt to handle duplicates.
//
int ws_make_index( const std::vector< std::string > rc_arr,
                   std::map< std::string, unsigned int > r_idx );

inline double ws_min( double lhs, double rhs )
    { return ( lhs < rhs ) ? lhs : rhs; }
inline double ws_max( double lhs, double rhs )
    { return ( lhs < rhs ) ? rhs : lhs; }
inline int ws_min( int lhs, int rhs )
    { return ( lhs < rhs ) ? lhs : rhs; }
inline int ws_max( int lhs, int rhs )
    { return ( lhs < rhs ) ? rhs : lhs; }
inline int ws_dabs( int v )
    { return ( v < 0 ) ? ( -1 * v ) : v; }

#endif

