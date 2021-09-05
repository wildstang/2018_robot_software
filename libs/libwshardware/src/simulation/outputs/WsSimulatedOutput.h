#ifndef __WSSIMULATEDOUTPUT_H__
#define __WSSIMULATEDOUTPUT_H__

#include "io/outputs/OutputTemplate.h"

class WsSimulatedOutput
{
    public:
        WsSimulatedOutput( std::string name );
        virtual ~WsSimulatedOutput( void );

        inline void setEnabled( bool enabled ) { a_simulatorEnabled = enabled; }
        inline std::string getSimulatedName( void )
            { return a_simulatedName; }

    protected:
        std::string a_simulatedName;
        bool a_simulatorEnabled;
};

template< typename T >
class WsSimulatedOutputTemplate : public OutputTemplate< T >, public WsSimulatedOutput
{
    public:
        WsSimulatedOutputTemplate( std::string name, T default_value );
        virtual ~WsSimulatedOutputTemplate( void );

    protected:
        virtual void sendDataToOutput( void );

        virtual int instantiate( void );
};

#define __WSSIMULATEDOUTPUT_H__TEMPLATE__
#include "WsSimulatedOutput.cpp"
#undef __WSSIMULATEDOUTPUT_H__TEMPLATE__

#endif

