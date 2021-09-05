#ifndef __INPUTFACTORY_H__
#define __INPUTFACTORY_H__

#include <string>

class Input;
class Output;

typedef enum
{
    INPUT_TYPE_NONE = 0,

    INPUT_TYPE_REMOTE_ANALOG,
    INPUT_TYPE_REMOTE_DIGITAL,
    INPUT_TYPE_ABSOLUTE_ENCODER,
    INPUT_TYPE_ANALOG_GYRO,
    INPUT_TYPE_ANALOG,
    INPUT_TYPE_COPMASS,
    INPUT_TYPE_DIGITAL,
    INPUT_TYPE_D_PAD_BUTTON,
    INPUT_TYPE_DRIVER_STATION_ANALOG,
    INPUT_TYPE_DRIVER_STATION_DIGITAL,
    INPUT_TYPE_HALL_EFFECT,
    INPUT_TYPE_I2C,
    INPUT_TYPE_JOYSTICK_AXIS,
    INPUT_TYPE_JOYSTICK_BUTTON,
    INPUT_TYPE_LIDAR,
    INPUT_TYPE_NAVX_GYRO,

    INPUT_TYPE_NUM
} inputTypeT;

typedef enum
{
    OUTPUT_TYPE_NONE = 0,

    OUTPUT_TYPE_REMOTE_ANALOG,
    OUTPUT_TYPE_REMOTE_DIGITAL,
    OUTPUT_TYPE_CAN_TALON,
    OUTPUT_TYPE_CAN_TALON_CTRE_MAG_ENCODER,
    OUTPUT_TYPE_CAN_VICTOR,
    OUTPUT_TYPE_DIGITAL_OUTPUT,
    OUTPUT_TYPE_DOUBLE_SOLENOID,
    OUTPUT_TYPE_I2C,
    OUTPUT_TYPE_RELAY,
    OUTPUT_TYPE_SERVO,
    OUTPUT_TYPE_SOLENOID,
    OUTPUT_TYPE_TALON,
    OUTPUT_TYPE_VICTOR,

    OUTPUT_TYPE_NUM
} outputTypeT;

std::string toString( inputTypeT input );
std::string toString( outputTypeT output );

class InputOutputFactory
{
    public:
        InputOutputFactory( void );
        virtual ~InputOutputFactory( void );

        virtual Input* createInput( std::string name, inputTypeT type ) = 0;
        virtual Output* createOutput( std::string name, outputTypeT type ) = 0;
        inline Input* createInput( const char* name, inputTypeT type )
            { return this->createInput( std::string( name ), type ); }
        inline Output* createOutput( const char* name, outputTypeT type )
            { return this->createOutput( std::string( name ), type ); }
};

#endif
