#include "SimulationIOFactory.h"

#include "logger/WsLogger.h"

#include "inputs/WsSimulatedInput.h"
#include "outputs/WsSimulatedOutput.h"

#include "io/outputs/WsSmartOutput.h"
#include "io/inputs/NullInputs.h"
#include "io/InputManager.h"

SimulationIOFactory::SimulationIOFactory( void )
{
}

SimulationIOFactory::~SimulationIOFactory( void )
{
}

Input* SimulationIOFactory::createInput( std::string name, inputTypeT type )
{
    Input* p_input = NULL;

    std::vector< unsigned char > v;

    InputManager* p_im = RobotCore::getRobotCore()->getInputManager();

    switch( type )
    {
        case INPUT_TYPE_REMOTE_ANALOG:
        case INPUT_TYPE_ABSOLUTE_ENCODER:
        case INPUT_TYPE_ANALOG_GYRO:
        case INPUT_TYPE_ANALOG:
        case INPUT_TYPE_DRIVER_STATION_ANALOG:
        case INPUT_TYPE_JOYSTICK_AXIS:
            p_input = new WsSimulatedInputTemplate< double >( name, 0.0 );
            break;
        case INPUT_TYPE_REMOTE_DIGITAL:
        case INPUT_TYPE_DIGITAL:
        case INPUT_TYPE_D_PAD_BUTTON:
        case INPUT_TYPE_DRIVER_STATION_DIGITAL:
        case INPUT_TYPE_JOYSTICK_BUTTON:
            p_input = new WsSimulatedInputTemplate< bool >( name, false );
            break;
        case INPUT_TYPE_COPMASS:
        case INPUT_TYPE_I2C:
            p_input = new WsSimulatedInputTemplate< std::vector< unsigned char > >(
                name, v );
            break;
        case INPUT_TYPE_HALL_EFFECT:
        case INPUT_TYPE_LIDAR:
            p_input = new WsSimulatedInputTemplate< int >( name, 0 );
            break;
        case INPUT_TYPE_NAVX_GYRO:
            p_input = new WsSimulatedInputTemplate< double >( name, 0.0 );
            p_im->addInput( new WsSimulatedInputTemplate< double >( name + "_rot_x", 0.0 ) );
            p_im->addInput( new WsSimulatedInputTemplate< double >( name + "_rot_y", 0.0 ) );
            p_im->addInput( new WsSimulatedInputTemplate< double >( name + "_accel_x", 0.0 ) );
            p_im->addInput( new WsSimulatedInputTemplate< double >( name + "_accel_y", 0.0 ) );
            p_im->addInput( new WsSimulatedInputTemplate< double >( name + "_accel_z", 0.0 ) );
            p_im->addInput( new WsSimulatedInputTemplate< double >( name + "_mag_x", 0.0 ) );
            p_im->addInput( new WsSimulatedInputTemplate< double >( name + "_mag_y", 0.0 ) );
            p_im->addInput( new WsSimulatedInputTemplate< double >( name + "_mag_z", 0.0 ) );
            break;
        case INPUT_TYPE_NONE:
        case INPUT_TYPE_NUM:
        default:
            WS_ERROR( "unknown input type %d", ( int )type );
            break;
    }

    return p_input;
}

Output* SimulationIOFactory::createOutput( std::string name, outputTypeT type )
{
    Output* p_output = NULL;

    smartOutputCmdT cmd = { SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE,
                            0.0, 0, 0.0, 0.0 };
    std::vector< unsigned char > v;

    InputManager* p_im = RobotCore::getRobotCore()->getInputManager();

	switch( type )
	{
        case OUTPUT_TYPE_REMOTE_ANALOG:
        case OUTPUT_TYPE_TALON:
        case OUTPUT_TYPE_SERVO:
        case OUTPUT_TYPE_VICTOR:
            p_output = new WsSimulatedOutputTemplate< double >( name, 0.0 );
            break;
        case OUTPUT_TYPE_SOLENOID:
        case OUTPUT_TYPE_DIGITAL_OUTPUT:
        case OUTPUT_TYPE_REMOTE_DIGITAL:
        case OUTPUT_TYPE_RELAY:
            p_output = new WsSimulatedOutputTemplate< bool >( name, false );
            break;
        case OUTPUT_TYPE_CAN_TALON:
        case OUTPUT_TYPE_CAN_VICTOR:
            p_output = new WsSimulatedOutputTemplate< smartOutputCmdT >( name, cmd );
            p_im->addInput( new NullAnalogInput( name + "_voltage", 0.0 ) );
            p_im->addInput( new NullAnalogInput( name + "_current", 0.0 ) );
            break;
        case OUTPUT_TYPE_CAN_TALON_CTRE_MAG_ENCODER:
            p_output = new WsSimulatedOutputTemplate< smartOutputCmdT >( name, cmd );
            p_im->addInput( new NullAnalogInput( name + "_sensor", 0.0 ) );
            p_im->addInput( new NullAnalogInput( name + "_voltage", 0.0 ) );
            p_im->addInput( new NullAnalogInput( name + "_current", 0.0 ) );
            p_im->addInput( new NullAnalogInput( name + "_is_stabilized", 0.0 ) );
            p_im->addInput( new NullAnalogInput( name + "_velocity", 0.0 ) );
            break;
        case OUTPUT_TYPE_DOUBLE_SOLENOID:
            p_output = new WsSimulatedOutputTemplate< int >( name, 0 );
            break;
        case OUTPUT_TYPE_I2C:
            p_output = new WsSimulatedOutputTemplate< std::vector< unsigned char > >( 
                name, v );
            break;
        case OUTPUT_TYPE_NUM:
        case OUTPUT_TYPE_NONE:
        default:
            WS_ERROR( "unknown output type %d", ( int )type );
            break;
    }

    return p_output;
}

