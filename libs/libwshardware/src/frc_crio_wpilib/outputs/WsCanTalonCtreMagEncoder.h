#ifndef __WSCANTALONCTREMAGENCODER_H__
#define __WSCANTALONCTREMAGENCODER_H__

#include "io/outputs/OutputStubs.h"
#include "ctre/phoenix/MotorControl/CAN/TalonSRX.h"
#include "ctre/phoenix/MotorControl/ControlMode.h"
#include "outputs/WsCanTalon.h"

#include "logger/Probe.h"

class NullAnalogInput;
class NullDigitalInput;

class WsCanTalonCtreMagEncoder : public WsCanTalon
{
    public:
        WsCanTalonCtreMagEncoder( std::string name,
                                  int channel,
                                  bool invert,
                                  double nominal_output_forward,
                                  double nominal_output_reverse,
                                  double peak_output_voltage_forward,
                                  double peak_output_voltage_reverse,
                                  bool direction,
                                  int feedback_ms,
                                  int stabilization_cycles,
                                  double epsilon,
                                  bool sensor_input_enabled,
                                  smartOutputCmdT default_value );
        WsCanTalonCtreMagEncoder( std::string name );
        virtual ~WsCanTalonCtreMagEncoder( void );

        virtual int instantiate( void ) override;

    protected:
        virtual void sendDataToOutput( void ) override;

        bool a_direction;
        int a_feedbackMs;
        bool a_sensorInputEnabled;
        int a_cycleCount;
        int a_stabilizationCycles;
        double a_epsilon;

        bool a_resetSensorPrev;

        NullAnalogInput* ap_sensorInput;
        NullDigitalInput* ap_stabilizedInput;
        NullAnalogInput* ap_velocityInput;

        ProbeValue< double > a_errorProbe;
        ProbeValue< int > a_cycleCountProbe;

    private:
        bool a_pidNegativeState;
};

#endif
