#ifndef __WSDIGITALOUTPUT_H__
#define __WSDIGITALOUTPUT_H__

#include "io/outputs/OutputStubs.h"

namespace frc
{
    class DigitalOutput;
}

class WsDigitalOutput : public DigitalOutput
{
        // By giving the output number in the constructor we can make this generic
        // for all digital outputs
    public:
        WsDigitalOutput( std::string name, int channel, bool default_value );
        WsDigitalOutput( std::string name );
        virtual ~WsDigitalOutput( void );

        virtual int instantiate( void );
        
    protected:
        virtual void sendDataToOutput( void );
        
    private:
        frc::DigitalOutput* ap_output;
        int a_channel;
};

#endif

