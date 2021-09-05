#ifndef __WSDSDIGITALINPUT_H__
#define __WSDSDIGITALINPUT_H__

#include "io/inputs/InputStubs.h"

class WsDSDigitalInput : public DigitalInput
{
    // By giving the input number in the constructor we can make this generic
    // for all digital inputs
    public:
        WsDSDigitalInput( std::string name, int channel );
        WsDSDigitalInput( std::string name );
        virtual ~WsDSDigitalInput( void );

    protected:
        virtual bool readRawValue( void ) override;

        virtual int instantiate( void ) override;

    private:
        int a_channel;
};

#endif

