#ifndef __WSCANVICTOR_H__
#define __WSCANVICTOR_H__

#include "ctre/phoenix/MotorControl/CAN/VictorSPX.h"
#include "outputs/WsPhoenixController.h"

#include <map>

class NullAnalogInput;

class WsCanVictor : public WsPhoenixController
{
    public:
        WsCanVictor( std::string name,
                     int channel,
                     bool invert,
                     double nominal_output_forward,
                     double nominal_output_reverse,
                     double peak_output_voltage_forward,
                     double peak_output_voltage_reverse,
                     smartOutputCmdT default_value );
        WsCanVictor( std::string name );
        virtual ~WsCanVictor( void );

        virtual int instantiate( void ) override;

    protected:
        virtual void sendDataToOutput( void ) override;

        void configureOutputLevels( void );

    private:
        ctre::phoenix::motorcontrol::can::VictorSPX* ap_canVictor;
        smartOutputControlModeT a_controlId;

        double a_nominalOutputForward;
        double a_nominalOutputReverse;
        double a_peakOutputVoltageForward;
        double a_peakOutputVoltageReverse;

        NullAnalogInput* ap_voltageInput;
        NullAnalogInput* ap_currentInput;

        bool a_brakeModePrev;
};

#endif

