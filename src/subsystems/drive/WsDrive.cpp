#include "WsDrive.h"

#include "utils/ws_string.h"

const wsDriveSignalT ds_neutralSignal = { 0, 0, false };
const wsDriveSignalT ds_brakeMode = { 0, 0, true };

std::string toString( wsDriveSignalT signal )
{
    std::string str = std::string( "L: " ) + toString( signal.leftMotor ) +
                      std::string( ", R: " ) + toString( signal.rightMotor );
    return str;
}

std::string toString( wsDriveTypeT type )
{
    std::string str;

    switch( type )
    {
        case WS_DRIVE_TYPE_RAW:
            str = std::string( "raw" );
            break;
        case WS_DRIVE_TYPE_CHEESY:
            str = std::string( "cheesy" );
            break;
        case WS_DRIVE_TYPE_FULL_BRAKE:
            str = std::string( "full_brake" );
            break;
        case WS_DRIVE_TYPE_AUTO_DRIVE_STRAIGHT:
            str = std::string( "auto_drive_straight" );
            break;
        case WS_DRIVE_TYPE_AUTO_TURN:
            str = std::string( "auto_turn" );
            break;
        case WS_DRIVE_TYPE_AUTO_ARC:
            str = std::string( "auto_arc" );
            break;
        case WS_DRIVE_TYPE_NUM:
        case WS_DRIVE_TYPE_NONE:
        default:
            str = std::string( "none" );
            break;
    }

    return str;
}
