#ifndef __WSNAVXINPUT_H__
#define __WSNAVXINPUT_H__

#include "io/inputs/InputStubs.h"
#include "io/inputs/NullInputs.h"

#include "ws_navx.h"

#include <stdint.h>

class WsNavxInput : public AnalogInput
{
    public:
        WsNavxInput( std::string name );
        virtual ~WsNavxInput( void );

    protected:
        virtual double readRawValue( void ) override;

        virtual int instantiate( void ) override;

    private:
        wsNavxT a_navxGyro;
        NullAnalogInput* ap_gyroX;
        NullAnalogInput* ap_gyroY;
        NullAnalogInput* ap_gyroZ;
        NullAnalogInput* ap_accelX;
        NullAnalogInput* ap_accelY;
        NullAnalogInput* ap_accelZ;
        NullAnalogInput* ap_magX;
        NullAnalogInput* ap_magY;
        NullAnalogInput* ap_magZ;
        NullAnalogInput* ap_pitch;
        NullAnalogInput* ap_roll;
        NullAnalogInput* ap_yaw;
};

#endif
