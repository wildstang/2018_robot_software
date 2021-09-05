#ifndef __WSJOYSTICKAXIS_H__
#define __WSJOYSTICKAXIS_H__

#include "io/inputs/InputStubs.h"

#include <string>

namespace frc
{
    class Joystick;
}

class WsJoystickAxis : public AnalogInput
{
    public:
        WsJoystickAxis( std::string name, int port, int axisIndex );
        WsJoystickAxis( std::string name );
        virtual ~WsJoystickAxis( void );
        
    protected:
        virtual double readRawValue( void ) override;

        virtual int instantiate( void ) override;
        
    private:
        frc::Joystick* ap_joystick;
        int a_axisIndex;
        int a_port;
};

#endif

