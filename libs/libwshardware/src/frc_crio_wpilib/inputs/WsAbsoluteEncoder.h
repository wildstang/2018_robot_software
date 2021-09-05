#ifndef __WSABSOLUTEENCODER_H__
#define __WSABSOLUTEENCODER_H__

#include <string>

#include "inputs/WsAnalogInput.h"

class WsAbsoluteEncoder : public WsAnalogInput
{
    public:
        WsAbsoluteEncoder( std::string name, int channel, double max_voltage );
        WsAbsoluteEncoder( std::string name );
        virtual ~WsAbsoluteEncoder( void );

    protected:
        virtual double readRawValue( void ) override;

        virtual int instantiate( void ) override;
        
    private:
        double a_maxVoltage;
};

#endif

