#include "WsDebugLogOutput.h"

#include "logger/WsLogger.h"

#include <unistd.h>

bool WsDebugLogOutput::s_debugEnabled = false;

#ifdef TARGET_SIMULATION
const char* g_debug_file = "/home/sam/debug.log";
#else
const char* g_debug_file = "/home/lvuser/debug.log";
#endif

WsDebugLogOutput::WsDebugLogOutput( void )
    : WsLogOutput()
    , ap_file( NULL )
    , a_enabled( true )
{
    if( s_debugEnabled )
    {
        ap_file = fopen( g_debug_file, "w" );
        if( ap_file == NULL )
        {
            WS_ERROR( "could not open file %s", g_debug_file );
            a_enabled = false;
        }
    }
}

WsDebugLogOutput::~WsDebugLogOutput( void )
{
    if( ap_file != NULL )
    {
        fclose( ap_file );
        ap_file = NULL;
    }
}

int WsDebugLogOutput::_output( const std::string& rc_str )
{
    if( !a_enabled ) return 0;

    if( ( ap_file == NULL ) && s_debugEnabled )
    {
        ap_file = fopen( g_debug_file, "w" );
        if( ap_file == NULL )
        {
            WS_ERROR( "could not open file %s", g_debug_file );
            a_enabled = false;
        }
    }

    if( ( ap_file != NULL ) && !s_debugEnabled )
    {
        fclose( ap_file );
        ap_file = NULL;
        sync();
    }

    std::string str;
    str.assign( rc_str );
    str.append( "\n" );

    if( ap_file != NULL )
    {
        fwrite( str.c_str(), 1, str.length(), ap_file );
    }

    return 0;
}

void WsDebugLogOutput::enableDebugLogging( void )
{
    s_debugEnabled = true;
}

void WsDebugLogOutput::disableDebugLogging( void )
{
    s_debugEnabled = false;
}

