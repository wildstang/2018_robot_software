#ifndef __WSLOGGER_H__
#define __WSLOGGER_H__

#include <vector>
#include <set>
#include <string>
#include <cassert>
#include <cstdio>

#include "utils/ws_string.h"
#include "utils/ws_utils.h"

#ifndef WS_LOG_BUFF_SIZE
#define WS_LOG_BUFF_SIZE 2048
#endif

#if defined(TARGET_SIMULATION)
#define WS_LOG( level, ... ) \
    { std::string __log_ = ws_std_string_printf( "[%lu] ## %s ## %s ## %d ## ", \
    ws_milliseconds(), __PRETTY_FUNCTION__, __FILE__, __LINE__ ) + \
    ws_std_string_printf( __VA_ARGS__ ); \
    ws_printf( level, "%s", __log_.c_str() ); }
#elif defined(TARGET_FRC_CRIO_WPILIB)
#define WS_LOG( level, ... ) \
    { std::string __log_ = ws_std_string_printf( "[%llu] ## %s ## %s ## %d ## ", \
    ws_milliseconds(), __PRETTY_FUNCTION__, __FILE__, __LINE__ ) + \
    ws_std_string_printf( __VA_ARGS__ ); \
    ws_printf( level, "%s", __log_.c_str() ); }
#elif defined(TARGET_RASPI)
#define WS_LOG( level, ... ) \
    { std::string __log_ = ws_std_string_printf( "[%llu] ## %s ## %s ## %d ## ", \
    ws_milliseconds(), __PRETTY_FUNCTION__, __FILE__, __LINE__ ) + \
    ws_std_string_printf( __VA_ARGS__ ); \
    ws_printf( level, "%s", __log_.c_str() ); }
#else
#error "unsupported target"
#endif

#define WS_FUNC_HEAD \
    WS_LOG( LOG_LEVEL_TRACE, "entering" );

#define WS_FUNC_FOOT \
    WS_LOG( LOG_LEVEL_TRACE, "leaving" );

#define WS_TRACE( ... ) WS_LOG( LOG_LEVEL_TRACE, __VA_ARGS__ );

#define WS_DATA( ... ) WS_LOG( LOG_LEVEL_DATA, __VA_ARGS__ );

#define WS_INFO( ... ) WS_LOG( LOG_LEVEL_INFO, __VA_ARGS__ );

#define WS_WARNING( ... ) WS_LOG( LOG_LEVEL_WARNING, __VA_ARGS__ );

#define WS_ERROR( ... ) WS_LOG( LOG_LEVEL_ERROR, __VA_ARGS__ );

#define WS_CRITICAL( ... ) \
    { WS_LOG( LOG_LEVEL_CRITICAL, __VA_ARGS__ ); assert( false ); }

#define WS_STATE( ... ) WS_LOG( LOG_LEVEL_STATE, __VA_ARGS__ );

#define WS_STD_LOG_OUTPUT 1
#define WS_DEBUG_LOG_OUTPUT 2
#define WS_NET_LOG_OUTPUT 4
#define WS_STDERR_LOG_OUTPUT 8

typedef enum
{
    LOG_LEVEL_NONE = 0,

    LOG_LEVEL_TRACE,
    LOG_LEVEL_DATA,
    LOG_LEVEL_STATE,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_CRITICAL,

    LOG_LEVEL_NUM
} logLevelT;

typedef struct
{
    logLevelT level;
    std::string messege;
} initLogT;

std::string toString( logLevelT level );
logLevelT parseLogLevel( std::string str );

int ws_vprintf( logLevelT level, const char* fmt, va_list ap );
int ws_printf( logLevelT level, const char* fmt, ... )
    __attribute__(( format( printf, 2, 3 ) ));

class WsLogOutput
{
    public:
        WsLogOutput( void );
        virtual ~WsLogOutput( void );

        virtual int _output( const std::string& rc_str );

        static int output( logLevelT level, const std::string& rc_str );
        static void setLoggingEnabled( logLevelT level, bool enable );
        static bool getLoggingEnabled( logLevelT level );
        static inline void setInitBegin( void )
            { s_initComplete = false; }
        static inline void setInitComplete( void )
            { s_initComplete = true; }

        static inline bool initRun( void ) { return s_initRun; }
        static void init( unsigned int flags = 0 );
        static void shutdown( void );

        static inline std::vector< initLogT > getInitLog( void )
            { return s_initLogEntries; }

    private:
        static std::vector< WsLogOutput* > s_outputs;

        static bool s_loggingEnabled[ LOG_LEVEL_NUM ];
        static bool s_initRun;
        static bool s_initComplete;

        static std::vector< initLogT > s_initLogEntries;
};

inline void logging_init( unsigned int flags =
                          WS_STD_LOG_OUTPUT |
                          WS_NET_LOG_OUTPUT )
{
    if( !( WsLogOutput::initRun() ) )
    {
        WsLogOutput::init( flags );
    }
}

#endif
