#ifndef __WSNETLOGOUTPUT_H__
#define __WSNETLOGOUTPUT_H__

#include "WsLogger.h"
#include "net/WsNetConsole.h"

class WsNetLogOutput : public WsLogOutput, public WsNetConsole
{
    public:
        WsNetLogOutput( void );
        virtual ~WsNetLogOutput( void );

        virtual int _output( const std::string& rc_str );

        int initNetLogOutput( void );

    protected:
        virtual int handleLine( const char* line );

        int handleSet( const std::vector< std::string >& rc_args );
        int handleShowInit( void );

    private:
        bool a_muted;
};

#endif

