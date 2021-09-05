#include "WsStderrLogOutput.h"

#include <cstdio>

WsStderrLogOutput::WsStderrLogOutput( void )
    : WsLogOutput()
{
}

WsStderrLogOutput::~WsStderrLogOutput( void )
{
}

int WsStderrLogOutput::_output( const std::string& rc_str )
{
    fprintf( stderr, "%s\n", rc_str.c_str() );
    return 0;
}

