#ifndef __WSCANTALON_H__
#define __WSCANTALON_H__

#include "ctre/phoenix/MotorControl/CAN/TalonSRX.h"
#include "ctre/phoenix/MotorControl/ControlMode.h"
#include "outputs/WsPhoenixController.h"

#include <map>

class NullAnalogInput;

class WsCanTalon : public WsPhoenixController
{
    public:
        WsCanTalon( std::string name,
                    int channel,
                    bool invert,
                    double nominal_output_forward,
                    double nominal_output_reverse,
                    double peak_output_voltage_forward,
                    double peak_output_voltage_reverse,
                    smartOutputCmdT default_value );
        WsCanTalon( std::string name );
        virtual ~WsCanTalon( void );

        virtual int instantiate( void ) override;

    protected:
        virtual void sendDataToOutput( void ) override;

        void configureOutputLevels( void );
        void configureCurrentLimiting( void );
        void setPidConstants( void );

        inline bool isBiDirectional( void ) {return a_biDirectionalPid;}

        ctre::phoenix::motorcontrol::can::TalonSRX* ap_talon;

        double getP( bool negative );
        double getI( bool negative );
        double getD( bool negative );
        double getF( bool negative );
        int getProfileSlot( bool negative );
        int configProfileSlot( bool negative );

        std::string getConstantsMessage( bool negative );

    private:
        smartOutputControlModeT a_controlId;
        double a_p;
        double a_i;
        double a_d;
        double a_f;
        double a_pNeg;
        double a_iNeg;
        double a_dNeg;
        double a_fNeg;
        bool a_biDirectionalPid;

        double a_nominalOutputForward;
        double a_nominalOutputReverse;
        double a_peakOutputVoltageForward;
        double a_peakOutputVoltageReverse;

        NullAnalogInput* ap_voltageInput;
        NullAnalogInput* ap_currentInput;

        bool a_brakeModePrev;
};

#endif
