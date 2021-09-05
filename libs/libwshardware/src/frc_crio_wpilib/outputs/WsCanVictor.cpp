#include "WsCanVictor.h"

#include "config/ConfigValueRef.h"
#include "io/InputManager.h"
#include "io/inputs/NullInputs.h"
#include "outputs/WsCanTalon.h"

WsCanVictor::WsCanVictor( std::string name,
                          int channel,
                          bool invert,
                          double nominal_output_forward,
                          double nominal_output_reverse,
                          double peak_output_voltage_forward,
                          double peak_output_voltage_reverse,
                          smartOutputCmdT default_value )
    : WsPhoenixController( name, channel, invert, default_value )
    , ap_canVictor( NULL )
    , a_controlId( SMART_OUTPUT_CONTROL_MODE_ID_NONE )
    , a_nominalOutputForward( nominal_output_forward )
    , a_nominalOutputReverse( nominal_output_reverse )
    , a_peakOutputVoltageForward( peak_output_voltage_forward )
    , a_peakOutputVoltageReverse( peak_output_voltage_reverse )
    , ap_voltageInput( NULL )
    , ap_currentInput( NULL )
    , a_brakeModePrev( false )
{
    InputManager* p_im = RobotCore::getRobotCore()->getInputManager();

    ap_voltageInput = new NullAnalogInput( this->getName() + "_voltage" );
    ap_currentInput = new NullAnalogInput( this->getName() + "_current" );

    p_im->addInput( ap_voltageInput );
    p_im->addInput( ap_currentInput );
}

WsCanVictor::WsCanVictor( std::string name )
    : WsPhoenixController( name )
    , ap_canVictor( NULL )
    , a_controlId( SMART_OUTPUT_CONTROL_MODE_ID_NONE )
    , a_nominalOutputForward( 0.0 )
    , a_nominalOutputReverse( 0.0 )
    , a_peakOutputVoltageForward( 12.0 )
    , a_peakOutputVoltageReverse( -12.0 )
    , ap_voltageInput( NULL )
    , ap_currentInput( NULL )
    , a_brakeModePrev( false )
{
    InputManager* p_im = RobotCore::getRobotCore()->getInputManager();

    ap_voltageInput = new NullAnalogInput( this->getName() + "_voltage" );
    ap_currentInput = new NullAnalogInput( this->getName() + "_current" );

    p_im->addInput( ap_voltageInput );
    p_im->addInput( ap_currentInput );

    CONFIG_VAR( "nominal_output_forward", a_nominalOutputForward, double );
    CONFIG_VAR( "nominal_output_reverse", a_nominalOutputReverse, double );
    CONFIG_VAR( "peak_output_voltage_forward", a_peakOutputVoltageForward, double );
    CONFIG_VAR( "peak_output_voltage_forward", a_peakOutputVoltageReverse, double );
}

WsCanVictor::~WsCanVictor( void )
{
    if( ap_canVictor != NULL )
    {
        delete ap_canVictor;
        ap_canVictor = NULL;
    }
}

int WsCanVictor::instantiate( void )
{
    int err = 0;
    if( ap_canVictor != NULL ) delete ap_canVictor;

    if( this->getChannel() < 0 )
    {
        WS_ERROR( "Output %s not configured, not instantiating",
                  this->getName().c_str() );
        err++;
    }
    else
    {
        ap_canVictor = new ctre::phoenix::motorcontrol::can::VictorSPX( this->getChannel() );

        // TODO
        //this->configureOutputLevels();
        this->configureOutputRamping( ap_canVictor );

        ap_canVictor->SetInverted( this->getInverted() );

        s_phoenix[ this->getChannel() ] = ap_canVictor;
    }

    return err;
}

void WsCanVictor::sendDataToOutput( void )
{
    if( ap_canVictor == NULL )
    {
        WS_ERROR( "tried to update can victor %s when it's null. Check config?",
                  this->getName().c_str() );
        return;
    }

    smartOutputCmdT cmd = this->getValue();

    switch( cmd.control_id )
    {
        case SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE:
            ap_canVictor->Set( ctre::phoenix::motorcontrol::ControlMode::PercentOutput,
                               cmd.speed );
            break;
        case SMART_OUTPUT_CONTROL_MODE_ID_FOLLOW:
            if( a_controlId != SMART_OUTPUT_CONTROL_MODE_ID_FOLLOW )
            {
                int follow_id = cmd.follow_output_id;

                if( follow_id == 0 )
                {
                    follow_id = this->getFollowChannel();
                    WS_INFO( "cmd follow id 0, using configured %d.", follow_id );
                }

                std::map< int,
                    ctre::phoenix::motorcontrol::can::BaseMotorController* >::iterator i =
                    s_phoenix.find( follow_id );

                if( i != s_phoenix.end() )
                {
                    ctre::phoenix::motorcontrol::can::BaseMotorController* 
                        p_controller = i->second;

                    if( p_controller != NULL )
                    {
                        ap_canVictor->Follow( *p_controller );
                    }
                    else
                    {
                        WS_ERROR( "Phoenix controller id %s is null in map",
                                  follow_id );
                    }
                }
                else
                {
                    WS_ERROR( "WsCanVictor %s tried to follow %d but it does not exist",
                              this->getName().c_str(), follow_id );
                }
            }
            break;
        case SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID:
        case SMART_OUTPUT_CONTROL_MODE_ID_VELOCITY_PID:
        case SMART_OUTPUT_CONTROL_MODE_ID_NONE:
        case SMART_OUTPUT_CONTROL_MODE_ID_NUM:
        default:
            if( a_controlId != cmd.control_id )
            {
                WS_ERROR( "WsCanVictor %s in bad state", this->getName().c_str() );
            }
            break;
    }

    if( a_brakeModePrev != cmd.brake_mode )
    {
        ctre::phoenix::motorcontrol::NeutralMode brake =
            ctre::phoenix::motorcontrol::Coast;

        if( cmd.brake_mode )
        {
            brake = ctre::phoenix::motorcontrol::Brake;
        }

        ap_canVictor->SetNeutralMode( brake );

        a_brakeModePrev = cmd.brake_mode;
    }

#if 0
    ap_voltageInput->setSlaveValue( ap_canVictor->GetMotorOutputVoltage() );
    ap_currentInput->setSlaveValue( ap_canVictor->GetOutputCurrent() );
#endif

    a_controlId = cmd.control_id;
}

void WsCanVictor::configureOutputLevels( void )
{
    if( !ap_canVictor ) return;

    ap_canVictor->ConfigNominalOutputForward( a_nominalOutputForward, 50 );
    ap_canVictor->ConfigNominalOutputReverse( a_nominalOutputReverse, 50 );
    ap_canVictor->ConfigPeakOutputForward( a_peakOutputVoltageForward, 50 );
    ap_canVictor->ConfigPeakOutputReverse( a_peakOutputVoltageReverse, 50 );
}

