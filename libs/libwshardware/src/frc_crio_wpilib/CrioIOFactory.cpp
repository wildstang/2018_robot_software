#include "CrioIOFactory.h"

#include "logger/WsLogger.h"

#include "inputs/RemoteAnalogInput.h"
#include "inputs/RemoteDigitalInput.h"
#include "inputs/WsAbsoluteEncoder.h"
#include "inputs/WsAnalogGyro.h"
#include "inputs/WsAnalogInput.h"
#include "inputs/WsCompassInput.h"
#include "inputs/WsDigitalInput.h"
#include "inputs/WsDPadButton.h"
#include "inputs/WsDSAnalogInput.h"
#include "inputs/WsDSDigitalInput.h"
#include "inputs/WsHallEffectInput.h"
#include "inputs/WsI2CInput.h"
#include "inputs/WsJoystickAxis.h"
#include "inputs/WsJoystickButton.h"
#include "inputs/WsLIDAR.h"
#include "inputs/WsNavxInput.h"

#include "outputs/RemoteAnalogOutput.h"
#include "outputs/RemoteDigitalOutput.h"
#include "outputs/WsCanTalon.h"
#include "outputs/WsCanTalonCtreMagEncoder.h"
#include "outputs/WsCanVictor.h"
#include "outputs/WsDigitalOutput.h"
#include "outputs/WsDoubleSolenoid.h"
#include "outputs/WsI2COutput.h"
#include "outputs/WsRelay.h"
#include "outputs/WsServo.h"
#include "outputs/WsSolenoid.h"
#include "outputs/WsTalon.h"
#include "outputs/WsVictor.h"

#include "io/outputs/WsSmartOutput.h"

CrioIOFactory::CrioIOFactory( void )
    : InputOutputFactory()
{
}

CrioIOFactory::~CrioIOFactory( void )
{
}

Input* CrioIOFactory::createInput( std::string name, inputTypeT type )
{
    Input* p_input = NULL;

    switch( type )
    {
        case INPUT_TYPE_REMOTE_ANALOG:
            p_input = new RemoteAnalogInput( name );
            break;
        case INPUT_TYPE_REMOTE_DIGITAL:
            p_input = new RemoteDigitalInput( name );
            break;
        case INPUT_TYPE_ABSOLUTE_ENCODER:
            p_input = new WsAbsoluteEncoder( name );
            break;
        case INPUT_TYPE_ANALOG_GYRO:
            p_input = new WsAnalogGyro( name );
            break;
        case INPUT_TYPE_ANALOG:
            p_input = new WsAnalogInput( name );
            break;
        case INPUT_TYPE_COPMASS:
            p_input = new WsCompassInput( name );
            break;
        case INPUT_TYPE_DIGITAL:
            p_input = new WsDigitalInput( name );
            break;
        case INPUT_TYPE_D_PAD_BUTTON:
            p_input = new WsDPadButton( name );
            break;
        case INPUT_TYPE_DRIVER_STATION_ANALOG:
            p_input = new WsDSAnalogInput( name );
            break;
        case INPUT_TYPE_DRIVER_STATION_DIGITAL:
            p_input = new WsDSDigitalInput( name );
            break;
        case INPUT_TYPE_HALL_EFFECT:
            p_input = new WsHallEffectInput( name );
            break;
        case INPUT_TYPE_I2C:
            p_input = new WsI2CInput( name );
            break;
        case INPUT_TYPE_JOYSTICK_AXIS:
            p_input = new WsJoystickAxis( name );
            break;
        case INPUT_TYPE_JOYSTICK_BUTTON:
            p_input = new WsJoystickButton( name );
            break;
        case INPUT_TYPE_LIDAR:
            p_input = new WsLIDAR( name );
            break;
        case INPUT_TYPE_NAVX_GYRO:
            p_input = new WsNavxInput( name );
            break;
        case INPUT_TYPE_NONE:
        case INPUT_TYPE_NUM:
        default:
            WS_ERROR( "invalid input type %d", ( int )type );
            break;
    }

    return p_input;
}

Output* CrioIOFactory::createOutput( std::string name, outputTypeT type )
{
    Output* p_output = NULL;

    switch( type )
    {
        case OUTPUT_TYPE_REMOTE_ANALOG:
            p_output = new RemoteAnalogOutput( name );
            break;
        case OUTPUT_TYPE_REMOTE_DIGITAL:
            p_output = new RemoteDigitalOutput( name );
            break;
        case OUTPUT_TYPE_CAN_TALON:
            p_output = new WsCanTalon( name );
            break;
        case OUTPUT_TYPE_CAN_TALON_CTRE_MAG_ENCODER:
            p_output = new WsCanTalonCtreMagEncoder( name );
            break;
        case OUTPUT_TYPE_CAN_VICTOR:
            p_output = new WsCanVictor( name );
            break;
        case OUTPUT_TYPE_DIGITAL_OUTPUT:
            p_output = new WsDigitalOutput( name );
            break;
        case OUTPUT_TYPE_DOUBLE_SOLENOID:
            p_output = new WsDoubleSolenoid( name );
            break;
        case OUTPUT_TYPE_I2C:
            p_output = new WsI2COutput( name );
            break;
        case OUTPUT_TYPE_RELAY:
            p_output = new WsRelay( name );
            break;
        case OUTPUT_TYPE_SERVO:
            p_output = new WsServo( name );
            break;
        case OUTPUT_TYPE_SOLENOID:
            p_output = new WsSolenoid( name );
            break;
        case OUTPUT_TYPE_TALON:
            p_output = new WsTalon( name );
            break;
        case OUTPUT_TYPE_VICTOR:
            p_output = new WsVictor( name );
            break;
        case OUTPUT_TYPE_NONE:
        case OUTPUT_TYPE_NUM:
        default:
            WS_ERROR( "invalid output type %d", ( int )type );
            break;
    }

    return p_output;
}
