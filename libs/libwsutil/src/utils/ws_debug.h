#ifndef __WS_DEBUG_H__
#define __WS_DEBUG_H__

#include <cassert>
#include <cstdlib>
#include <deque>
#include <string>

#ifdef WS_DEBUG
#define WS_ASSERT( cond ) \
        { \
            if( !( cond ) ) \
            { \
                fprintf( stderr, "assertion failed: %s %s %s %d\n", \
                         #cond, __PRETTY_FUNCTION__, __FILE__, __LINE__ ); \
                ws_bt(); \
            } \
            assert( cond ); \
        }
#else
#define WS_ASSERT( cond ) assert( cond );
#endif

void ws_hex_dump( const char* buf, size_t len );
std::string ws_dbg_preview( std::deque< std::string > d, size_t n );

void ws_bt( void );

#endif

