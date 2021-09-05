#ifndef __WSDRIVE_H__
#define __WSDRIVE_H__

#include <string>

#define BRAKE_MODE_ALLOWABLE_ERROR 20

//#define PATH_PROFILE_SLOT 0
#define BASE_LOCK_PROFILE_SLOT 1

/**
 * A drivetrain command consisting of the left, right motor settings and whether
 * the brake mode is enabled.
 */
typedef struct
{
    double leftMotor;
    double rightMotor;
    bool brakeMode;
} wsDriveSignalT;

typedef enum
{
    WS_DRIVE_TYPE_NONE = 0,

    WS_DRIVE_TYPE_RAW,
    WS_DRIVE_TYPE_CHEESY,
    WS_DRIVE_TYPE_FULL_BRAKE,
    WS_DRIVE_TYPE_AUTO_DRIVE_STRAIGHT,
    WS_DRIVE_TYPE_AUTO_TURN,
    WS_DRIVE_TYPE_AUTO_ARC,

    WS_DRIVE_TYPE_NUM
} wsDriveTypeT;

extern const wsDriveSignalT ds_neutralSignal;
extern const wsDriveSignalT ds_brakeMode;

std::string toString( wsDriveSignalT signal );
std::string toString( wsDriveTypeT type );

#endif
