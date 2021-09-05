#include "WsLogger.h"

#include "logger/WsDebugLogOutput.h"
#include "logger/WsNetLogOutput.h"
#include "logger/WsStderrLogOutput.h"
#include "utils/ws_string.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdlib>
#include <cstdarg>

bool WsLogOutput::s_loggingEnabled[ LOG_LEVEL_NUM ];
bool WsLogOutput::s_initRun = false;
bool WsLogOutput::s_initComplete = true;
std::vector< WsLogOutput* > WsLogOutput::s_outputs;
std::vector< initLogT > WsLogOutput::s_initLogEntries;

void WsLogOutput::init( unsigned int flags )
{
    if( s_initRun )
    {
        return;
    }

    memset( s_loggingEnabled, true, sizeof( s_loggingEnabled ) );
    setLoggingEnabled( LOG_LEVEL_TRACE, false );
    setLoggingEnabled( LOG_LEVEL_STATE, false );

    //
    // INSERT LOG OUTPUTS BELOW THIS LINE
    //

    if( flags & WS_STD_LOG_OUTPUT )
    {
        s_outputs.push_back( new WsLogOutput() );
    }

    if( flags & WS_NET_LOG_OUTPUT )
    {
        WsNetLogOutput* p_net = new WsNetLogOutput();
        p_net->initNetLogOutput();
        s_outputs.push_back( p_net );
    }

    if( flags & WS_DEBUG_LOG_OUTPUT )
    {
        s_outputs.push_back( new WsDebugLogOutput() );
    }

    if( flags & WS_STDERR_LOG_OUTPUT )
    {
        s_outputs.push_back( new WsStderrLogOutput() );
    }

    //
    // INSERT LOG OUTPUTS ABOVE THIS LINE
    //

    WsLogOutput::s_initRun = true;

    WS_INFO( "logging initialized" );
}

WsLogOutput::WsLogOutput( void )
{
}

WsLogOutput::~WsLogOutput( void )
{
}

int WsLogOutput::_output( const std::string& rc_str )
{
    printf( "%s\n", rc_str.c_str() );
    return 0;
}

int ws_vprintf( logLevelT level, const char* fmt, va_list ap )
{
    char buff[ WS_LOG_BUFF_SIZE ];
    memset( buff, 0x00, WS_LOG_BUFF_SIZE * sizeof( char ) );

    vsnprintf( buff, WS_LOG_BUFF_SIZE - 1, fmt, ap );

    std::string str = std::string( buff );

    return WsLogOutput::output( level, str );
}

int ws_printf( logLevelT level, const char* fmt, ... )
{
    va_list args;
    va_start( args, fmt );

    return ws_vprintf( level, fmt, args );
}

void WsLogOutput::shutdown( void )
{
    std::vector< WsLogOutput* > v = s_outputs;
    s_outputs.clear();
    s_initRun = false;

    std::vector< WsLogOutput* >::iterator i = v.begin();

    for( ; i != v.end(); ++i )
    {
        WsLogOutput* p_output = *i;
        delete p_output;
    }
}

int WsLogOutput::output( logLevelT level, const std::string& rc_str )
{
    logging_init();

    std::string output = toString( level ) + ":: " + rc_str;

    if( !s_initComplete )
    {
        initLogT lineLog = { .level=level, .messege=output };
        s_initLogEntries.push_back( lineLog );
    }

    if( !s_loggingEnabled[ level ] ) return 0;

    int err = 0;
    if( !s_initRun )
    {
        puts( output.c_str() );
    }
    else
    {
        std::vector< WsLogOutput* >::iterator i = s_outputs.begin();
        for(; i != s_outputs.end(); ++i )
        {
            WsLogOutput* p_output = *i;
            err += p_output->_output( output ) ? 1 : 0;
        }
    }

    return err;
}

void WsLogOutput::setLoggingEnabled( logLevelT level, bool enable )
{
    s_loggingEnabled[ level ] = enable;
}

bool WsLogOutput::getLoggingEnabled( logLevelT level )
{
    return s_loggingEnabled [ level ];
}

std::string toString( logLevelT level )
{
    std::string str;

    switch( level )
    {
        case LOG_LEVEL_TRACE:
            str = std::string( "LOG_TRACE" );
            break;
        case LOG_LEVEL_DATA:
            str = std::string( "LOG_DATA" );
            break;
        case LOG_LEVEL_STATE:
            str = std::string( "LOG_STATE" );
            break;
        case LOG_LEVEL_INFO:
            str = std::string( "LOG_INFO" );
            break;
        case LOG_LEVEL_WARNING:
            str = std::string( "LOG_WARNING" );
            break;
        case LOG_LEVEL_ERROR:
            str = std::string( "LOG_ERROR" );
            break;
        case LOG_LEVEL_CRITICAL:
            str = std::string( "LOG_CRITICAL" );
            break;
        case LOG_LEVEL_NONE:
        case LOG_LEVEL_NUM:
        default:
            str = std::string( "LOG_INVALID" );
            break;
    }

    return str;
}

logLevelT parseLogLevel( std::string str )
{
    logLevelT level = LOG_LEVEL_NONE;
    ws_to_lower( str );

    if( 0 == strncmp( str.c_str(), "trace", 5 ) )
    {
        level = LOG_LEVEL_TRACE;
    }
    else if( 0 == strncmp( str.c_str(), "data", 4 ) )
    {
        level = LOG_LEVEL_DATA;
    }
    else if( 0 == strncmp( str.c_str(), "state", 5 ) )
    {
        level = LOG_LEVEL_STATE;
    }
    else if( 0 == strncmp( str.c_str(), "info", 4 ) )
    {
        level = LOG_LEVEL_INFO;
    }
    else if( 0 == strncmp( str.c_str(), "warning", 7 ) )
    {
        level = LOG_LEVEL_WARNING;
    }
    else if( 0 == strncmp( str.c_str(), "error", 5 ) )
    {
        level = LOG_LEVEL_ERROR;
    }
    else if( 0 == strncmp( str.c_str(), "critical", 8 ) )
    {
        level = LOG_LEVEL_CRITICAL;
    }

    return level;
}
