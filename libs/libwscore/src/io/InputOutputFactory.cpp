#include "InputOutputFactory.h"

InputOutputFactory::InputOutputFactory( void )
{
}

InputOutputFactory::~InputOutputFactory( void )
{
}

std::string toString( inputTypeT input )
{
    std::string str;

    switch( input )
    {
        case INPUT_TYPE_REMOTE_ANALOG:
            str = std::string( "remote_analog" );
            break;
        case INPUT_TYPE_REMOTE_DIGITAL:
            str = std::string( "remote_digital" );
            break;
        case INPUT_TYPE_ABSOLUTE_ENCODER:
            str = std::string( "absolute_encoder" );
            break;
        case INPUT_TYPE_ANALOG_GYRO:
            str = std::string( "analog_gyro" );
            break;
        case INPUT_TYPE_ANALOG:
            str = std::string( "analog" );
            break;
        case INPUT_TYPE_COPMASS:
            str = std::string( "copmass" );
            break;
        case INPUT_TYPE_DIGITAL:
            str = std::string( "digital" );
            break;
        case INPUT_TYPE_D_PAD_BUTTON:
            str = std::string( "d_pad_button" );
            break;
        case INPUT_TYPE_DRIVER_STATION_ANALOG:
            str = std::string( "driver_station_analog" );
            break;
        case INPUT_TYPE_DRIVER_STATION_DIGITAL:
            str = std::string( "driver_station_digital" );
            break;
        case INPUT_TYPE_HALL_EFFECT:
            str = std::string( "hall_effect" );
            break;
        case INPUT_TYPE_I2C:
            str = std::string( "i2c" );
            break;
        case INPUT_TYPE_JOYSTICK_AXIS:
            str = std::string( "joystick_axis" );
            break;
        case INPUT_TYPE_JOYSTICK_BUTTON:
            str = std::string( "joystick_button" );
            break;
        case INPUT_TYPE_LIDAR:
            str = std::string( "lidar" );
            break;
        case INPUT_TYPE_NAVX_GYRO:
            str = std::string( "navx_gyro" );
            break;
        case INPUT_TYPE_NUM:
        case INPUT_TYPE_NONE:
        default:
            str = std::string( "undefined" );
            break;
    }

    return str;
}

std::string toString( outputTypeT output )
{
    std::string str;

    switch( output )
    {
        case OUTPUT_TYPE_REMOTE_ANALOG:
            str = std::string( "remote_analog" );
            break;
        case OUTPUT_TYPE_REMOTE_DIGITAL:
            str = std::string( "remote_digital" );
            break;
        case OUTPUT_TYPE_CAN_TALON:
            str = std::string( "can_talon" );
            break;
        case OUTPUT_TYPE_CAN_TALON_CTRE_MAG_ENCODER:
            str = std::string( "can_talon_ctre_mag_encoder" );
            break;
        case OUTPUT_TYPE_CAN_VICTOR:
            str = std::string( "can_victor" );
            break;
        case OUTPUT_TYPE_DIGITAL_OUTPUT:
            str = std::string( "digital_output" );
            break;
        case OUTPUT_TYPE_DOUBLE_SOLENOID:
            str = std::string( "double_solenoid" );
            break;
        case OUTPUT_TYPE_I2C:
            str = std::string( "i2c" );
            break;
        case OUTPUT_TYPE_RELAY:
            str = std::string( "relay" );
            break;
        case OUTPUT_TYPE_SERVO:
            str = std::string( "servo" );
            break;
        case OUTPUT_TYPE_SOLENOID:
            str = std::string( "solenoid" );
            break;
        case OUTPUT_TYPE_TALON:
            str = std::string( "talon" );
            break;
        case OUTPUT_TYPE_VICTOR:
            str = std::string( "victor" );
            break;
        case OUTPUT_TYPE_NUM:
        case OUTPUT_TYPE_NONE:
        default:
            str = std::string( "undefined" );
            break;
    }

    return str;
}

