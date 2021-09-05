#include "WsSmartOutput.h"

#include "config/ConfigValueRef.h"
#include "io/outputs/SmartOutputPidIO.h"
#include "io/InputManager.h"
#include "io/OutputManager.h"
#include "io/Input.h"
#include "io/inputs/NullInputs.h"

WsSmartOutput::WsSmartOutput( std::string name, 
                          std::string input_name, 
                          std::string output_name, 
                          smartOutputCmdT default_value )
    : SmartOutput( name, default_value )
    , a_controlId( SMART_OUTPUT_CONTROL_MODE_ID_NONE )
    , a_inputName( input_name )
    , a_outputName( output_name )
    , a_motorValue( 0.0 )
    , ap_motor( NULL )
    , ap_pidIO( new SmartOutputPidIO() )
    , ap_speedPidIO( new SmartOutputPidIO() )
    , ap_stabilizedInput( NULL )
    , a_pid( ap_pidIO, ap_pidIO, std::string( "pid" ), this )
    , a_speedPid( ap_speedPidIO, ap_speedPidIO, std::string( "speed_pid" ), this )
{
    ap_stabilizedInput = new NullDigitalInput( this->getName() + "_is_stabilized" );
    RobotCore::getRobotCore()->getInputManager()->addInput( ap_stabilizedInput );
}

WsSmartOutput::WsSmartOutput( std::string name )
    : SmartOutput( name, sc_nullCommand )
    , a_controlId( SMART_OUTPUT_CONTROL_MODE_ID_NONE )
    , a_inputName()
    , a_outputName()
    , a_motorValue( 0.0 )
    , ap_motor( NULL )
    , ap_pidIO( new SmartOutputPidIO() )
    , ap_speedPidIO( new SmartOutputPidIO() )
    , ap_stabilizedInput( NULL )
    , a_pid( ap_pidIO, ap_pidIO, std::string( "pid" ), this )
    , a_speedPid( ap_speedPidIO, ap_speedPidIO, std::string( "speed_pid" ), this )
{
    ap_stabilizedInput = new NullDigitalInput( this->getName() + "_is_stabilized" );
    RobotCore::getRobotCore()->getInputManager()->addInput( ap_stabilizedInput );

    CONFIG_VAR( "input_name", a_inputName, std::string );
    CONFIG_VAR( "output_name", a_outputName, std::string );
}

WsSmartOutput::~WsSmartOutput( void )
{
}

int WsSmartOutput::instantiate( void )
{
    Input* p_input =
        RobotCore::getRobotCore()->getInputManager()->getInput( a_inputName );
    if( !p_input )
    {
        WS_ERROR( "input %s does not exist", a_inputName.c_str() );
        return 1;
    }

    p_input->addInputListener( this );

    ap_motor =
        RobotCore::getRobotCore()->getOutputManager()->getOutput( a_outputName );
    if( !ap_motor )
    {
        WS_ERROR( "output %s does not exist", a_outputName.c_str() );
        return 1;
    }

    return 0;
}

void WsSmartOutput::sendDataToOutput( void )
{
    if( !this->isInstantiated() ) return;

    smartOutputCmdT cmd = this->getValue();
    switch( cmd.control_id )
    {
        case SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE:
            if( a_controlId != SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE )
            {
                a_pid.reset();
                a_speedPid.reset();
                ap_stabilizedInput->setSlaveValue( false );
            }

            a_motorValue = cmd.speed;   

            ap_stabilizedInput->setSlaveValue( false );
            break;

        case SMART_OUTPUT_CONTROL_MODE_ID_FOLLOW:
            if( a_controlId != SMART_OUTPUT_CONTROL_MODE_ID_FOLLOW )
            {
                WS_ERROR( "Follow mode not supported by smart output %s",
                          this->getName().c_str() );
                a_pid.reset();
                a_speedPid.reset();
                ap_stabilizedInput->setSlaveValue( false );
            }

            ap_stabilizedInput->setSlaveValue( false );
            break;

        case SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID:
            if( a_controlId != SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID )
            {
                a_pid.reset();
                ap_stabilizedInput->setSlaveValue( false );
            }
            a_pid.setSetPoint( cmd.pos_setpoint );
            a_pid.calcPid();

            ap_stabilizedInput->setSlaveValue( a_pid.isStabilized() );

            a_motorValue = ap_pidIO->getOutput();
            break;

        case SMART_OUTPUT_CONTROL_MODE_ID_VELOCITY_PID:
            if( a_controlId != SMART_OUTPUT_CONTROL_MODE_ID_VELOCITY_PID )
            {
                a_speedPid.reset();
                ap_stabilizedInput->setSlaveValue( false );
            }
            a_speedPid.setSetPoint( cmd.vel_setpoint );
            a_speedPid.calcPid();

            ap_stabilizedInput->setSlaveValue( a_speedPid.isStabilized() );

            a_motorValue = ap_speedPidIO->getOutput();
            break;

        case SMART_OUTPUT_CONTROL_MODE_ID_NONE:
        case SMART_OUTPUT_CONTROL_MODE_ID_NUM:
        default:
            if( a_controlId != cmd.control_id )
            {
                a_pid.reset();
                a_speedPid.reset();
                ap_stabilizedInput->setSlaveValue( false );
                WS_ERROR( "invalid control mode for smart output %s", 
                          this->getName().c_str() );
            }
            break;
    }

    ap_motor->setValue( a_motorValue );
    a_controlId = cmd.control_id;
}

void WsSmartOutput::inputUpdate( Input* p_source, double value )
{
    ap_pidIO->setInput( value );
    ap_speedPidIO->setInput( value );
}

