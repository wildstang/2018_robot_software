#include "WsCanTalon.h"

#include "Talon.h"
#include "config/ConfigValueRef.h"
#include "io/InputManager.h"
#include "io/inputs/NullInputs.h"

WsCanTalon::WsCanTalon( std::string name,
                        int channel,
                        bool invert,
                        double nominal_output_forward,
                        double nominal_output_reverse,
                        double peak_output_voltage_forward,
                        double peak_output_voltage_reverse,
                        smartOutputCmdT default_value )
    : WsPhoenixController( name, channel, invert, default_value )
    , ap_talon( NULL )
    , a_controlId( SMART_OUTPUT_CONTROL_MODE_ID_NONE )
    , a_p( 0.0 )
    , a_i( 0.0 )
    , a_d( 0.0 )
    , a_f( 0.0 )
    , a_pNeg( 0.0 )
    , a_iNeg( 0.0 )
    , a_dNeg( 0.0 )
    , a_fNeg( 0.0 )
    , a_biDirectionalPid( false )
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

    CONFIG_VAR( "p", a_p, double );
    CONFIG_VAR( "i", a_i, double );
    CONFIG_VAR( "d", a_d, double );
    CONFIG_VAR( "f", a_f, double );
    CONFIG_VAR( "p_neg", a_pNeg, double );
    CONFIG_VAR( "i_neg", a_iNeg, double );
    CONFIG_VAR( "d_neg", a_dNeg, double );
    CONFIG_VAR( "f_neg", a_fNeg, double );
    CONFIG_VAR( "bi_directional_pid", a_biDirectionalPid, bool );
}

WsCanTalon::WsCanTalon( std::string name )
    : WsPhoenixController( name )
    , ap_talon( NULL )
    , a_controlId( SMART_OUTPUT_CONTROL_MODE_ID_NONE )
    , a_p( 0.0 )
    , a_i( 0.0 )
    , a_d( 0.0 )
    , a_f( 0.0 )
    , a_pNeg( 0.0 )
    , a_iNeg( 0.0 )
    , a_dNeg( 0.0 )
    , a_fNeg( 0.0 )
    , a_biDirectionalPid( false )
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

    CONFIG_VAR( "p", a_p, double );
    CONFIG_VAR( "i", a_i, double );
    CONFIG_VAR( "d", a_d, double );
    CONFIG_VAR( "f", a_f, double );
    CONFIG_VAR( "p_neg", a_pNeg, double );
    CONFIG_VAR( "i_neg", a_iNeg, double );
    CONFIG_VAR( "d_neg", a_dNeg, double );
    CONFIG_VAR( "f_neg", a_fNeg, double );
    CONFIG_VAR( "bi_directional_pid", a_biDirectionalPid, bool );

    CONFIG_VAR( "nominal_output_forward", a_nominalOutputForward, double );
    CONFIG_VAR( "nominal_output_reverse", a_nominalOutputReverse, double );
    CONFIG_VAR( "peak_output_voltage_forward", a_peakOutputVoltageForward, double );
    CONFIG_VAR( "peak_output_voltage_forward", a_peakOutputVoltageReverse, double );
}

WsCanTalon::~WsCanTalon( void )
{
    if( ap_talon != NULL )
    {
        delete ap_talon;
        ap_talon = NULL;
    }
}

int WsCanTalon::instantiate( void )
{
    int err = 0;
    if( ap_talon != NULL ) delete ap_talon;

    if( this->getChannel() < 0 )
    {
        WS_ERROR( "Output %s not configured, not instantiating.",
                  this->getName().c_str() );
        err++;
    }
    else
    {
        ap_talon = new ctre::phoenix::motorcontrol::can::TalonSRX( this->getChannel() );

        this->setPidConstants();
        //this->configureOutputLevels();
        this->configureCurrentLimiting();

        this->configureOutputRamping( ap_talon );

        ap_talon->SetInverted( this->getInverted() );

        s_phoenix[ this->getChannel() ] = ap_talon;
    }

    return err;
}

void WsCanTalon::sendDataToOutput( void )
{
    if( ap_talon == NULL )
    {
        WS_WARNING( "Talon %s not instantiated, check configuration maybe?",
                    this->getName().c_str() );
        return;
    }

    smartOutputCmdT cmd = this->getValue();

    switch( cmd.control_id )
    {
        case SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE:
            ap_talon->Set( ctre::phoenix::motorcontrol::ControlMode::PercentOutput,
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

                    if( p_controller )
                    {
                        ap_talon->Follow( *p_controller );
                    }
                    else
                    {
                        WS_ERROR( "Phoenix controller id %d null in the map.",
                                  follow_id );
                    }
                }
                else
                {
                    WS_ERROR( "WsCanTalon %s tried to follow %d but it does not exist",
                              this->getName().c_str(), follow_id );
                }
            }
            break;
        case SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID:
            if( a_controlId != SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID )
            {
                this->setPidConstants();
            }
            ap_talon->Set( ctre::phoenix::motorcontrol::ControlMode::Position,
                           cmd.pos_setpoint );
            break;
        case SMART_OUTPUT_CONTROL_MODE_ID_VELOCITY_PID:
            if( a_controlId != SMART_OUTPUT_CONTROL_MODE_ID_VELOCITY_PID )
            {
                this->setPidConstants();
            }
            ap_talon->Set( ctre::phoenix::motorcontrol::ControlMode::Velocity,
                           cmd.vel_setpoint );
            break;
        case SMART_OUTPUT_CONTROL_MODE_ID_NONE:
        case SMART_OUTPUT_CONTROL_MODE_ID_NUM:
        default:
            if( a_controlId != cmd.control_id )
            {
                WS_ERROR( "WsCanTalon %s in bad state", this->getName().c_str() );
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

        ap_talon->SetNeutralMode( brake );

        a_brakeModePrev = cmd.brake_mode;
    }

#if 0
    ap_voltageInput->setSlaveValue( ap_talon->GetMotorOutputVoltage() );
    ap_currentInput->setSlaveValue( ap_talon->GetOutputCurrent() );
#endif

    a_controlId = cmd.control_id;
}

void WsCanTalon::setPidConstants( void )
{
    if( !ap_talon ) return;

    ap_talon->SelectProfileSlot( getProfileSlot( false ), 0 );
    ap_talon->Config_kP( getProfileSlot( false ), getP( false ), 50 );
    ap_talon->Config_kI( getProfileSlot( false ), getI( false ), 50 );
    ap_talon->Config_kD( getProfileSlot( false ), getD( false ), 50 );
    ap_talon->Config_kF( getProfileSlot( false ), getF( false ), 50 );
    ap_talon->Config_kP( getProfileSlot( true ), getP( true ), 50 );
    ap_talon->Config_kI( getProfileSlot( true ), getI( true ), 50 );
    ap_talon->Config_kD( getProfileSlot( true ), getD( true ), 50 );
    ap_talon->Config_kF( getProfileSlot( true ), getF( true ), 50 );
}

void WsCanTalon::configureOutputLevels( void )
{
    if( !ap_talon ) return;

    ap_talon->ConfigNominalOutputForward( a_nominalOutputForward, 50 );
    ap_talon->ConfigNominalOutputReverse( a_nominalOutputReverse, 50 );
    ap_talon->ConfigPeakOutputForward( a_peakOutputVoltageForward, 50 );
    ap_talon->ConfigPeakOutputReverse( a_peakOutputVoltageReverse, 50 );
}

void WsCanTalon::configureCurrentLimiting( void )
{
    if( !ap_talon ) return;

    double peak_limit = this->getCurrentPeakLimit();
    double cont_limit = this->getCurrentContinuousLimit();
    int peak_duration = this->getPeakDuration();

    bool current_limiting = false;

    if( peak_limit > 1.0 )
    {
        ap_talon->ConfigPeakCurrentLimit( peak_limit, 50 );
        current_limiting = true;
    }

    if( cont_limit > 1.0 )
    {
        ap_talon->ConfigContinuousCurrentLimit( cont_limit, 50 );
        current_limiting = true;
    }

    if( peak_duration > 0 )
    {
        ap_talon->ConfigPeakCurrentDuration( peak_duration, 50 );
        current_limiting = true;
    }

    ap_talon->EnableCurrentLimit( current_limiting );

    if( current_limiting )
    {
        WS_INFO( "current limiting enabled for %s", this->getName().c_str() );
    }
}

double WsCanTalon::getP( bool negative )
{
    return negative ? a_pNeg : a_p;
}

double WsCanTalon::getI( bool negative )
{
    return negative ? a_iNeg : a_i;
}

double WsCanTalon::getD( bool negative )
{
    return negative ? a_dNeg : a_d;
}

double WsCanTalon::getF( bool negative )
{
    return negative ? a_fNeg : a_f;
}

int WsCanTalon::getProfileSlot( bool negative )
{
    return negative ? 1 : 0;
}

std::string WsCanTalon::getConstantsMessage( bool negative )
{
    std::string msg = ws_std_string_printf( "slot %d, p %f, i %f, d %f, f %f",
                                            getProfileSlot( negative ),
                                            getP( negative ),
                                            getI( negative ),
                                            getD( negative ),
                                            getF( negative ) );
    return msg;
}

int WsCanTalon::configProfileSlot( bool negative )
{
    if( ap_talon != NULL )
    {
        ap_talon->SelectProfileSlot( this->getProfileSlot( negative ), 0 );
        WS_INFO( "%s: setting pid constants '%s'", 
                 this->getName().c_str(),
                 this->getConstantsMessage( negative ).c_str() );
    }

    return 0;
}

