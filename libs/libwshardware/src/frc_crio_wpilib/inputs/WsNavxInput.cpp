#include "inputs/WsNavxInput.h"

#include "I2C.h"

#include "core/RobotCore.h"
#include "io/InputManager.h"

WsNavxInput::WsNavxInput( std::string name )
    : AnalogInput( name, 0.0 )
    , a_navxGyro( { NULL } )
    , ap_gyroX( NULL )
    , ap_gyroY( NULL )
    , ap_gyroZ( NULL )
    , ap_accelX( NULL )
    , ap_accelY( NULL )
    , ap_accelZ( NULL )
    , ap_magX( NULL )
    , ap_magY( NULL )
    , ap_magZ( NULL )
    , ap_pitch( NULL )
    , ap_roll( NULL )
    , ap_yaw( NULL )
{
    InputManager* p_im = RobotCore::getRobotCore()->getInputManager();

    ap_gyroX = new NullAnalogInput( name + "_raw_x" );
    ap_gyroY = new NullAnalogInput( name + "_raw_y" );
    ap_gyroZ = new NullAnalogInput( name + "_raw_z" );
    ap_accelX = new NullAnalogInput( name + "_accel_x" );
    ap_accelY = new NullAnalogInput( name + "_accel_y" );
    ap_accelZ = new NullAnalogInput( name + "_accel_z" );
    ap_magX = new NullAnalogInput( name + "_mag_x" );
    ap_magY = new NullAnalogInput( name + "_mag_y" );
    ap_magZ = new NullAnalogInput( name + "_mag_z" );
    ap_pitch = new NullAnalogInput( name + "_pitch" );
    ap_roll = new NullAnalogInput( name + "_roll" );
    ap_yaw = new NullAnalogInput( name + "_yaw" );

    p_im->addInput( ap_gyroX );
    p_im->addInput( ap_gyroY );
    p_im->addInput( ap_gyroZ );
    p_im->addInput( ap_accelX );
    p_im->addInput( ap_accelY );
    p_im->addInput( ap_accelZ );
    p_im->addInput( ap_magX );
    p_im->addInput( ap_magY );
    p_im->addInput( ap_magZ );
    p_im->addInput( ap_pitch );
    p_im->addInput( ap_roll );
    p_im->addInput( ap_yaw );
}

WsNavxInput::~WsNavxInput( void )
{
    ws_navx_destroy( &a_navxGyro );
}

int WsNavxInput::instantiate( void )
{
    ws_navx_destroy( &a_navxGyro );
    ws_navx_init( &a_navxGyro, WS_NAVX_I2C_PORT_ONBOARD );

    return 0;
}

double WsNavxInput::readRawValue( void )
{
    ap_gyroX->setSlaveValue( ws_navx_get_raw_gyro_x( &a_navxGyro ) );
    ap_gyroY->setSlaveValue( ws_navx_get_raw_gyro_y( &a_navxGyro ) );
    ap_gyroZ->setSlaveValue( ws_navx_get_raw_gyro_z( &a_navxGyro ) );
    ap_accelZ->setSlaveValue( ws_navx_get_raw_accel_x( &a_navxGyro ) );
    ap_accelY->setSlaveValue( ws_navx_get_raw_accel_y( &a_navxGyro ) );
    ap_accelZ->setSlaveValue( ws_navx_get_raw_accel_z( &a_navxGyro ) );
    ap_magX->setSlaveValue( ws_navx_get_raw_mag_x( &a_navxGyro ) );
    ap_magY->setSlaveValue( ws_navx_get_raw_mag_y( &a_navxGyro ) );
    ap_magZ->setSlaveValue( ws_navx_get_raw_mag_z( &a_navxGyro ) );
    ap_pitch->setSlaveValue( ws_navx_get_pitch( &a_navxGyro ) );
    ap_roll->setSlaveValue( ws_navx_get_roll( &a_navxGyro ) );
    ap_yaw->setSlaveValue( ws_navx_get_yaw( &a_navxGyro ) );

    return ws_navx_get_angle( &a_navxGyro );
}

