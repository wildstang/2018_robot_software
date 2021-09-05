#ifndef __WSJOYSTICKBUTTON_H__
#define __WSJOYSTICKBUTTON_H__

#include "io/inputs/InputStubs.h"

#include <string>

namespace frc
{
    class Joystick;
}

class WsJoystickButton : public DigitalInput
{
    public:
        WsJoystickButton( std::string name, int port, 
                          bool d_pad_button, int button_index );
        WsJoystickButton( std::string name );
        virtual ~WsJoystickButton( void );

    protected:
        virtual bool readRawValue( void ) override;

        virtual int instantiate( void ) override;

    private:
        frc::Joystick* ap_joystick;
        int a_buttonIndex;
        bool a_dPadButton;
        int a_port;
};

#endif

