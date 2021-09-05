#include "WsNetLogOutput.h"

#include "utils/ws_string.h"

#include <cstring>

WsNetLogOutput::WsNetLogOutput( void )
    : WsLogOutput()
    , WsNetConsole( 1111 )
    , a_muted( false )
{
}

WsNetLogOutput::~WsNetLogOutput( void )
{
}

int WsNetLogOutput::initNetLogOutput( void )
{
    return this->startSocket();
}

int WsNetLogOutput::_output( const std::string& rc_str )
{
    if( !a_muted )
    {
        std::string str;
        str.assign( rc_str );
        str.append( "\n" );
        sendBytes( str.c_str(), str.length() );
    }

    // do not return any error. Don't care if it isn't connected.
    return 0;
}

int WsNetLogOutput::handleLine( const char* line )
{
    std::vector< std::string > tok = ws_strtok_sep( line, " \n\r" );
    if( tok.size() <= 0 ) return 0;

    std::string command = tok[ 0 ];

    int err = 0;
    if( 0 == strncmp( command.c_str(), "set", 3 ) )
    {
        err = this->handleSet( tok );
    }
    else if( 0 == strncmp( command.c_str(), "print_init", 10 ) )
    {
        err = this->handleShowInit();
    }
    else if( 0 == strncmp( command.c_str(), "mute", 4 ) )
    {
        a_muted = true;
    }
    else if( 0 == strncmp( command.c_str(), "unmute", 6 ) )
    {
        a_muted = false;
    }
    else if( 0 == strncmp( command.c_str(), "help", 4 ) )
    {
        net_printf( ">> help, set, print_init, mute, unmute\n" );
    }
    else
    {
        WS_ERROR( "command %s not recognized", command.c_str() );
        err++;
    }

    return err;
}

int WsNetLogOutput::handleSet( const std::vector< std::string >& rc_args )
{
    if( rc_args.size() != 3 )
    {
        WS_ERROR( "expected: set LEVEL on/off" );
        return 1;
    }

    logLevelT level = parseLogLevel( rc_args[ 1 ] );
    std::string str_active = rc_args[ 2 ];
    ws_to_lower( str_active );
    bool active = false;

    if( level == LOG_LEVEL_NONE )
    {
        WS_ERROR( "specify a valid log level" );
        return 1;
    }

    int err = parse( str_active, &active );
    if( err )
    {
        WS_ERROR( "specify a valid state" );
        return 1;
    }

    WsLogOutput::setLoggingEnabled( level, active );

    return 0;
}

int WsNetLogOutput::handleShowInit( void )
{
    std::vector< initLogT > v = WsLogOutput::getInitLog();
    std::vector< initLogT >::iterator i = v.begin();
    for(; i != v.end(); ++i )
    {
        if( WsLogOutput::getLoggingEnabled( ( *i ).level ) )
        {
            std::string line = ( *i ).messege;
            net_printf( "%s\n", line.c_str() );
        }
    }

    return 0;
}

