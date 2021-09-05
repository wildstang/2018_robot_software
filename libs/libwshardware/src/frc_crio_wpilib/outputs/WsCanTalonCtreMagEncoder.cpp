#include "WsCanTalonCtreMagEncoder.h"

#include "Talon.h"
#include "config/ConfigValueRef.h"
#include "io/InputManager.h"
#include "io/inputs/NullInputs.h"

WsCanTalonCtreMagEncoder::WsCanTalonCtreMagEncoder( std::string name,
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
                                                    smartOutputCmdT default_value )
    : WsCanTalon( name,
                  channel,
                  invert,
                  nominal_output_forward,
                  nominal_output_reverse,
                  peak_output_voltage_forward,
                  peak_output_voltage_reverse,
                  default_value )
    , a_direction( direction )
    , a_feedbackMs( feedback_ms )
    , a_sensorInputEnabled( sensor_input_enabled )
    , a_cycleCount( 0 )
    , a_stabilizationCycles( stabilization_cycles )
    , a_epsilon( epsilon )
    , a_resetSensorPrev( false )
    , ap_sensorInput( NULL )
    , ap_stabilizedInput( NULL )
    , ap_velocityInput( NULL )
    , a_errorProbe( "error", 0.0 )
    , a_cycleCountProbe( "cycle_count", 0.0 )
    , a_pidNegativeState( false )
{
    InputManager* p_im = RobotCore::getRobotCore()->getInputManager();

    ap_sensorInput = new NullAnalogInput( this->getName() + "_sensor" );
    ap_stabilizedInput = new NullDigitalInput( this->getName() + "_is_stabilized" );
    ap_velocityInput = new NullAnalogInput( this->getName() + "_velocity" );

    p_im->addInput( ap_sensorInput );
    p_im->addInput( ap_stabilizedInput );
    p_im->addInput( ap_velocityInput );

    this->addProbe( &a_errorProbe );
    this->addProbe( &a_cycleCountProbe );
}

WsCanTalonCtreMagEncoder::WsCanTalonCtreMagEncoder( std::string name )
    : WsCanTalon( name )
    , a_direction( false )
    , a_feedbackMs( 0 )
    , a_sensorInputEnabled( true )
    , a_cycleCount( 0 )
    , a_stabilizationCycles( 0 )
    , a_epsilon( 0.0 )
    , ap_sensorInput( NULL )
    , ap_stabilizedInput( NULL )
    , a_errorProbe( "error", 0.0 )
    , a_cycleCountProbe( "cycle_count", 0.0 )
    , a_pidNegativeState( false )
{
    InputManager* p_im = RobotCore::getRobotCore()->getInputManager();

    ap_sensorInput = new NullAnalogInput( this->getName() + "_sensor" );
    ap_stabilizedInput = new NullDigitalInput( this->getName() + "_is_stabilized" );
    ap_velocityInput = new NullAnalogInput( this->getName() + "_velocity" );

    p_im->addInput( ap_sensorInput );
    p_im->addInput( ap_stabilizedInput );
    p_im->addInput( ap_velocityInput );

    CONFIG_VAR( "sensor_direction", a_direction, bool );
    CONFIG_VAR( "feedback_ms", a_feedbackMs, int );
    CONFIG_VAR( "sensor_polling_enabled", a_sensorInputEnabled, bool );
    CONFIG_VAR( "stabilized_cycles", a_stabilizationCycles, int );
    CONFIG_VAR( "epsilon", a_epsilon, double );

    this->addProbe( &a_errorProbe );
    this->addProbe( &a_cycleCountProbe );
}

WsCanTalonCtreMagEncoder::~WsCanTalonCtreMagEncoder( void )
{
}

int WsCanTalonCtreMagEncoder::instantiate( void )
{
    int err = WsCanTalon::instantiate();

    if( !err )
    {
        ap_talon->ConfigSelectedFeedbackSensor(
            ctre::phoenix::motorcontrol::CTRE_MagEncoder_Relative, 0, 50 );

        ap_talon->SetStatusFramePeriod(
            ctre::phoenix::motorcontrol::Status_2_Feedback0, a_feedbackMs, 50 );
        ap_talon->SetSensorPhase( a_direction );

        ap_talon->ConfigVelocityMeasurementPeriod(
            ctre::phoenix::motorcontrol::Period_10Ms, 50 );
        ap_talon->ConfigVelocityMeasurementWindow( 4, 50 );

        ap_talon->SetSelectedSensorPosition( 0, 0, 50 );
    }

    return err;
}

void WsCanTalonCtreMagEncoder::sendDataToOutput( void )
{
    WsCanTalon::sendDataToOutput();

    smartOutputCmdT c = this->getValue();

    if( a_sensorInputEnabled )
    {
        double position = ( double )ap_talon->GetSelectedSensorPosition( 0 );
        double error = c.pos_setpoint - position;

        if( isBiDirectional() )
        {
            if( error < 0.0 )
            {
                if( !a_pidNegativeState )
                {
                    this->configProfileSlot( true );
                    a_pidNegativeState = true;
                }
            }
            else
            {
                if( a_pidNegativeState )
                {
                    this->configProfileSlot( false );
                    a_pidNegativeState = false;
                }
            }
        }
        else
        {
            if( a_pidNegativeState )
            {
                this->configProfileSlot( false );
                a_pidNegativeState = false;
            }
        }

        a_cycleCount = ( fabs( error ) < a_epsilon ) ?
            ( a_cycleCount + 1 ) : 0;

        a_errorProbe.update( error );
        a_cycleCountProbe.update( a_cycleCount );

        if( c.control_id != SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID )
        {
            a_cycleCount = 0;
            ap_stabilizedInput->setSlaveValue( false );
        }
        else
        {
            ap_stabilizedInput->setSlaveValue( a_cycleCount > a_stabilizationCycles );
        }

        ap_sensorInput->setSlaveValue( position );
        ap_velocityInput->setSlaveValue( ap_talon->GetSelectedSensorVelocity( 0 ) );
    }

    if( c.reset_sensor && !a_resetSensorPrev )
    {
        ap_talon->SetSelectedSensorPosition( 0, 0, 50 );
    }

    a_resetSensorPrev = c.reset_sensor;
}

