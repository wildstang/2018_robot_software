#undef NAMESPACED_WPILIB

#include "ws_navx.h"

#include "I2C.h"
#include "AHRS.h"

int ws_navx_init( wsNavxT* p_navx, wsNavxI2CPortT port )
{
    I2C::Port p = I2C::Port::kOnboard;
    switch( port )
    {
        case WS_NAVX_I2C_PORT_ONBOARD:
            p = I2C::Port::kOnboard;
            break;
        case WS_NAVX_I2C_PORT_MXP:
            p = I2C::Port::kMXP;
            break;
        case WS_NAVX_I2C_PORT_NONE:
        case WS_NAVX_I2C_PORT_NUM:
        default:
            // TODO: errors
            break;
    }

    p_navx->p_darwin_awards_for_code = new AHRS( p );
    return 0;
}

int ws_navx_destroy( wsNavxT* p_navx )
{
    if( p_navx->p_darwin_awards_for_code )
        delete p_navx->p_darwin_awards_for_code;
    return 0;
}

double ws_navx_get_raw_gyro_x( wsNavxT* p_navx )
{
    return p_navx->p_darwin_awards_for_code->GetRawGyroX();
}

double ws_navx_get_raw_gyro_y( wsNavxT* p_navx )
{
    return p_navx->p_darwin_awards_for_code->GetRawGyroY();
}

double ws_navx_get_raw_gyro_z( wsNavxT* p_navx )
{
    return p_navx->p_darwin_awards_for_code->GetRawGyroZ();
}

double ws_navx_get_raw_accel_x( wsNavxT* p_navx )
{
    return p_navx->p_darwin_awards_for_code->GetRawAccelX();
}

double ws_navx_get_raw_accel_y( wsNavxT* p_navx )
{
    return p_navx->p_darwin_awards_for_code->GetRawAccelY();
}

double ws_navx_get_raw_accel_z( wsNavxT* p_navx )
{
    return p_navx->p_darwin_awards_for_code->GetRawAccelZ();
}

double ws_navx_get_raw_mag_x( wsNavxT* p_navx )
{
    return p_navx->p_darwin_awards_for_code->GetRawMagX();
}

double ws_navx_get_raw_mag_y( wsNavxT* p_navx )
{
    return p_navx->p_darwin_awards_for_code->GetRawMagY();
}

double ws_navx_get_raw_mag_z( wsNavxT* p_navx )
{
    return p_navx->p_darwin_awards_for_code->GetRawMagZ();
}

double ws_navx_get_pitch( wsNavxT* p_navx )
{
    return p_navx->p_darwin_awards_for_code->GetPitch();
}

double ws_navx_get_roll( wsNavxT* p_navx )
{
    return p_navx->p_darwin_awards_for_code->GetRoll();
}

double ws_navx_get_yaw( wsNavxT* p_navx )
{
    return p_navx->p_darwin_awards_for_code->GetYaw();
}

double ws_navx_get_angle( wsNavxT* p_navx )
{
    return p_navx->p_darwin_awards_for_code->GetAngle();
}

