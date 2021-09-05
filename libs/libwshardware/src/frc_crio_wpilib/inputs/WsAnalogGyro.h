#ifndef __WSANALOGGYRO_H__
#define __WSANALOGGYRO_H__

#include "io/inputs/InputStubs.h"

#include <stdint.h>

namespace frc
{
    class AnalogGyro;
}

class WsAnalogGyro : public AnalogInput
{
    public:
        WsAnalogGyro( std::string name, int channel, bool drift_comp );
        WsAnalogGyro( std::string name );
        virtual ~WsAnalogGyro( void );
        
        void calibrate( void );

    protected:
        virtual double readRawValue( void ) override;

        virtual int instantiate( void ) override;
        
    private:
        frc::AnalogGyro* ap_gyro;
        uint64_t a_startTime;
        double a_driftPerNano;
        bool a_driftComp;
        int a_channel;
        
        static const double sc_driftPerNanoFixed;
};

#endif

