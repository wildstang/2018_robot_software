#ifndef __WSDEBUGLOGOUTPUT_H__
#define __WSDEBUGLOGOUTPUT_H__

#include "WsLogger.h"

#include <cstdio>

class WsDebugLogOutput : public WsLogOutput
{
    public:
        WsDebugLogOutput( void );
        virtual ~WsDebugLogOutput( void ); 

        virtual int _output( const std::string& rc_str );

        static void enableDebugLogging( void );
        static void disableDebugLogging( void );

    protected:
        FILE* ap_file;
        bool a_enabled;
        
        static bool s_debugEnabled;
};

#endif

