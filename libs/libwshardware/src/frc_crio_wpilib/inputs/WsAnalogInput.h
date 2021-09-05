#ifndef __WSANALOGINPUT_H__
#define __WSANALOGINPUT_H__

#include <string>

#include "io/inputs/InputStubs.h"

namespace frc
{
    class AnalogInput;
}

class WsAnalogInput : public AnalogInput
{
    public:
        // By giving the input number in the constructor we can make this generic
        // for all digital inputs
        WsAnalogInput( std::string name, int channel );
        WsAnalogInput( std::string name );
        virtual ~WsAnalogInput( void );
        
    protected:
        virtual double readRawValue( void ) override;

        virtual int instantiate( void ) override;

    private:
        frc::AnalogInput* ap_input;
        int a_channel;
};

#endif

