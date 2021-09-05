#ifndef __WSDPADBUTTON_H__
#define __WSDPADBUTTON_H__

#include "io/inputs/InputStubs.h"

namespace frc
{
    class Joystick;
}

class WsDPadButton : public DigitalInput
{
    public:
        WsDPadButton( std::string name, int port, int buttonIndex );
        WsDPadButton( std::string name );
        virtual ~WsDPadButton( void );
        
    protected:
        virtual bool readRawValue( void ) override;

        virtual int instantiate( void ) override;
        
    private:
        frc::Joystick* ap_joystick;
        int a_buttonIndex;
        int a_port;
};

#endif

