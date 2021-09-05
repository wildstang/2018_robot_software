#ifndef __WSDOUBLESOLENOID_H__
#define __WSDOUBLESOLENOID_H__

#include "io/outputs/OutputStubs.h"

#include <string>

namespace frc
{
    class DoubleSolenoid;
}

typedef enum
{
    WS_DOUBLE_SOLENOID_STATE_NONE = 0,
    
    WS_DOUBLE_SOLENOID_STATE_OFF,
    WS_DOUBLE_SOLENOID_STATE_FORWARD,
    WS_DOUBLE_SOLENOID_STATE_REVERSE,

    WS_DOUBLE_SOLENOID_STATE_NUM
} wsDoubleSolenoidStateT;

class WsDoubleSolenoid : public DiscreteOutput
{
    public:
        WsDoubleSolenoid( std::string name, 
                          int module, 
                          int channel1, 
                          int channel2, 
                          wsDoubleSolenoidStateT default_value );
        WsDoubleSolenoid( std::string name );
        virtual ~WsDoubleSolenoid( void );

        virtual int instantiate( void );
        
    protected:
        virtual void sendDataToOutput( void );
        
    private:
        frc::DoubleSolenoid* ap_solenoid;
        int a_module;
        int a_firstChannel;
        int a_secondChannel;
};

#endif

