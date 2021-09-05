#ifndef __WSDSANALOGINPUT_H__
#define __WSDSANALOGINPUT_H__

#include "io/inputs/InputStubs.h"

class WsDSAnalogInput : public AnalogInput
{
    // By giving the input number in the constructor we can make this generic
    // for all analog inputs
    public:
        WsDSAnalogInput( std::string name, int channel );
        WsDSAnalogInput( std::string name );
        virtual ~WsDSAnalogInput( void );

    protected:
        virtual double readRawValue( void ) override;

        virtual int instantiate( void ) override;

    private:
        int a_channel;
};

#endif

