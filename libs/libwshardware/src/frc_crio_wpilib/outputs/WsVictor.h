#ifndef __WSVICTOR_H__
#define __WSVICTOR_H__

#include "io/outputs/OutputStubs.h"

namespace frc
{
    class Victor;
}

/**
 *
 * @author Nathan
 */
class WsVictor : public AnalogOutput
{
    // By giving the victor1 number in the constructor we can make this generic
    // for all digital victor1s
    public:
        WsVictor( std::string name, int channel, double default_value );
        WsVictor( std::string name );
        virtual ~WsVictor( void );

        virtual int instantiate( void );

    protected:
        virtual void sendDataToOutput( void );

    private:
        frc::Victor* ap_victor;
        int a_channel;
};

#endif

