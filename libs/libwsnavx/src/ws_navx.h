#ifndef __WS_NAVX_H__
#define __WS_NAVX_H__

//
// Note this library was designed to bridge the gap between modules
// that assume namespacing for wpilib and those that don't.
//
// You should only include this file from modules that assume namespacing
// of wpilib, and do not include any files from this file.
//

class AHRS;

typedef struct
{
    AHRS* p_darwin_awards_for_code;
} wsNavxT;

typedef enum
{
    WS_NAVX_I2C_PORT_NONE = 0,

    WS_NAVX_I2C_PORT_ONBOARD,
    WS_NAVX_I2C_PORT_MXP,

    WS_NAVX_I2C_PORT_NUM
} wsNavxI2CPortT;

int ws_navx_init( wsNavxT* p_navx, wsNavxI2CPortT port );
int ws_navx_destroy( wsNavxT* p_navx );

double ws_navx_get_raw_gyro_x( wsNavxT* p_navx );
double ws_navx_get_raw_gyro_y( wsNavxT* p_navx );
double ws_navx_get_raw_gyro_z( wsNavxT* p_navx );
double ws_navx_get_raw_accel_x( wsNavxT* p_navx );
double ws_navx_get_raw_accel_y( wsNavxT* p_navx );
double ws_navx_get_raw_accel_z( wsNavxT* p_navx );
double ws_navx_get_raw_mag_x( wsNavxT* p_navx );
double ws_navx_get_raw_mag_y( wsNavxT* p_navx );
double ws_navx_get_raw_mag_z( wsNavxT* p_navx );
double ws_navx_get_pitch( wsNavxT* p_navx );
double ws_navx_get_roll( wsNavxT* p_navx );
double ws_navx_get_yaw( wsNavxT* p_navx );
double ws_navx_get_angle( wsNavxT* p_navx );

#endif

