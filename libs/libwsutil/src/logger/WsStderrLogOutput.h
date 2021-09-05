#ifndef __WSSTDERRLOGOUTPUT_H__
#define __WSSTDERRLOGOUTPUT_H__

#include "WsLogger.h"

class WsStderrLogOutput : public WsLogOutput
{
    public:
        WsStderrLogOutput( void );
        virtual ~WsStderrLogOutput( void ); 

        virtual int _output( const std::string& rc_str );
};

#endif

