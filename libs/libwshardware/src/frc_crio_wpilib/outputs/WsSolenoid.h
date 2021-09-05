#ifndef __WSSOLENOID_H__
#define __WSSOLENOID_H__

#include "io/outputs/OutputStubs.h"

#include <string>

namespace frc
{
    class Solenoid;
}

class WsSolenoid : public DigitalOutput
{
    public:
        WsSolenoid( std::string name, int module,
                    int channel, bool default_value );
        WsSolenoid( std::string name, int module, int channel );
        WsSolenoid( std::string name );
        virtual ~WsSolenoid( void );

        virtual int instantiate( void );

    protected:
        virtual void sendDataToOutput( void );

    private:
        frc::Solenoid* ap_solenoid;
        int a_module;
        int a_channel;
};

#endif

