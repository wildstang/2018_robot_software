#ifndef __WSSERVO_H__
#define __WSSERVO_H__

#include "io/outputs/OutputStubs.h"

namespace frc
{
    class Servo;
}

class WsServo : public AnalogOutput
{
    public:
        WsServo( std::string name, int channel, double default_value );
        WsServo( std::string name );
        virtual ~WsServo( void );

        virtual int instantiate( void );

    protected:
        virtual void sendDataToOutput( void );

    private:
        frc::Servo* ap_servo;
        int a_channel;
};

#endif

