#ifndef __WSTALON_H__
#define __WSTALON_H__

#include "io/outputs/OutputStubs.h"

namespace frc
{
    class Talon;
}

/**
 *
 * @author Nathan
 */
class WsTalon : public AnalogOutput
{
    // By giving the victor1 number in the constructor we can make this generic
    // for all digital victor1s
    public:
        WsTalon( std::string name, int channel, double default_value );
        WsTalon( std::string name );
        virtual ~WsTalon( void );

        virtual int instantiate( void );

    protected:
        virtual void sendDataToOutput( void );

    private:
        frc::Talon* ap_talon;
        int a_channel;
};

#endif

