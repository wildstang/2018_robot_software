#ifndef __WSDIGITALINPUT_H__
#define __WSDIGITALINPUT_H__

#include "io/inputs/InputStubs.h"

#include <string>

namespace frc
{
    class DigitalInput;
}

class WsDigitalInput : public DigitalInput
{
    public:
        // By giving the input number in the constructor we can make this generic
        // for all digital inputs
        WsDigitalInput( std::string name, int channel, bool pullup );
        WsDigitalInput( std::string name );
        virtual ~WsDigitalInput( void );
        
    protected:
        virtual bool readRawValue( void ) override;

        virtual int instantiate( void ) override;
        
    private:
        frc::DigitalInput* ap_input;
        bool a_pullup;
        int a_channel;
};

#endif

