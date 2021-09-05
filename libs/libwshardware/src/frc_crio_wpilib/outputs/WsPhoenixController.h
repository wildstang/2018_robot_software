#ifndef __WSPHOENIXCONTROLLER_H__
#define __WSPHOENIXCONTROLLER_H__

#include "io/outputs/OutputStubs.h"
#include "ctre/phoenix/MotorControl/CAN/BaseMotorController.h"
#include "ctre/phoenix/MotorControl/ControlMode.h"

#include <map>

class WsPhoenixController : public SmartOutput
{
    public:
        WsPhoenixController( std::string name,
                             int channel,
                             bool inverted,
                             smartOutputCmdT default_value );
        WsPhoenixController( std::string name );
        virtual ~WsPhoenixController( void );

    protected:
        inline int getChannel( void ) { return a_channel; }
        inline int getFollowChannel( void ) { return a_followChannel; }
        inline bool getInverted( void ) { return a_inverted; }
        inline double getCurrentPeakLimit( void )
            { return a_currentPeakLimit; }
        inline double getCurrentContinuousLimit( void )
            { return a_currentContinuousLimit; }
        inline double getOutputRampSeconds( void )
            { return a_rampSeconds; }
        inline int getPeakDuration( void )
            { return a_peakCurrentDuration; }

        static std::map< int, 
            ctre::phoenix::motorcontrol::can::BaseMotorController* > s_phoenix;

        int configureOutputRamping( 
            ctre::phoenix::motorcontrol::can::BaseMotorController* p_motor );

    private:
        int a_channel;
        int a_followChannel;
        bool a_inverted;
        double a_currentPeakLimit;
        double a_currentContinuousLimit;
        double a_rampSeconds;
        int a_peakCurrentDuration;
};

#endif

