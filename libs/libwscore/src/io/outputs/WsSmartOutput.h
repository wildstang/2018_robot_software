#ifndef __WSSMARTOUTPUT_H__
#define __WSSMARTOUTPUT_H__

#include "io/outputs/OutputStubs.h"
#include "io/routing/InputListener.h"
#include "pid/PidController.h"

class SmartOutputPidIO;
class NullDigitalInput;

class WsSmartOutput : public SmartOutput, public InputListener
{
    public:
        WsSmartOutput( std::string name, 
                     std::string input_name, 
                     std::string output_name, 
                     smartOutputCmdT default_value );
        WsSmartOutput( std::string name );
        virtual ~WsSmartOutput( void );

        virtual void inputUpdate( Input* p_source, double value );

    protected:
        virtual void sendDataToOutput( void );

        virtual int instantiate( void );

    private:
        smartOutputControlModeT a_controlId;

        std::string a_inputName;
        std::string a_outputName;
        double a_motorValue;
        Output* ap_motor;

        SmartOutputPidIO* ap_pidIO;
        SmartOutputPidIO* ap_speedPidIO;

        NullDigitalInput* ap_stabilizedInput;

        PidController a_pid;
        PidController a_speedPid;
};

#endif

