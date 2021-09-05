#include "WsPhoenixController.h"

#include "config/ConfigValueRef.h"

std::map< int, ctre::phoenix::motorcontrol::can::BaseMotorController* > 
    WsPhoenixController::s_phoenix;

WsPhoenixController::WsPhoenixController( std::string name,
                                          int channel,
                                          bool inverted,
                                          smartOutputCmdT default_value )
    : SmartOutput( name, default_value )
    , a_channel( channel )
    , a_followChannel( 0 )
    , a_inverted( inverted )
    , a_currentPeakLimit( 0.0 )
    , a_currentContinuousLimit( 0.0 )
    , a_rampSeconds( 0.0 )
    , a_peakCurrentDuration( 0 )
{
    CONFIG_VAR( "follow_channel", a_followChannel, int );
    CONFIG_VAR( "current_peak_limit", a_currentPeakLimit, double );
    CONFIG_VAR( "current_continuous_limit", a_currentContinuousLimit, double );
    CONFIG_VAR( "peak_current_duration", a_peakCurrentDuration, int );
    CONFIG_VAR( "output_ramp_seconds", a_rampSeconds, double );
}

WsPhoenixController::WsPhoenixController( std::string name )
    : SmartOutput( name, sc_nullCommand )
    , a_channel( -1 )
    , a_followChannel( 0 )
    , a_inverted( false )
    , a_currentPeakLimit( 0.0 )
    , a_currentContinuousLimit( 0.0 )
    , a_rampSeconds( 0.0 )
    , a_peakCurrentDuration( 0 )
{
    CONFIG_VAR( "channel", a_channel, int );
    CONFIG_VAR( "follow_channel", a_followChannel, int );
    CONFIG_VAR( "inverted", a_inverted, bool );
    CONFIG_VAR( "current_peak_limit", a_currentPeakLimit, double );
    CONFIG_VAR( "current_continuous_limit", a_currentContinuousLimit, double );
    CONFIG_VAR( "peak_current_duration", a_peakCurrentDuration, int );
    CONFIG_VAR( "output_ramp_seconds", a_rampSeconds, double );
}

WsPhoenixController::~WsPhoenixController( void )
{
}

int WsPhoenixController::configureOutputRamping( 
        ctre::phoenix::motorcontrol::can::BaseMotorController* p_motor )
{
    if( p_motor == NULL ) return 1;

    if( a_rampSeconds > 0.0 )
    {
        WS_INFO( "output ramping enabled for %s", this->getName().c_str() );
        p_motor->ConfigOpenloopRamp( a_rampSeconds, 50 );
        p_motor->ConfigClosedloopRamp( a_rampSeconds, 50 );
    }

    return 0;
}

