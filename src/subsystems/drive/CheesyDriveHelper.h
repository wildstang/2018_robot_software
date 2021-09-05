#ifndef __CHEESYDRIVEHELPER_H__
#define __CHEESYDRIVEHELPER_H__

#include "subsystems/drive/WsDrive.h"

/**
 * Helper class to implement "Cheesy Drive". "Cheesy Drive" simply means that
 * the "turning" stick controls the curvature of the robot's path rather than
 * its rate of heading change. This helps make the robot more controllable at
 * high speeds. Also handles the robot's quick turn functionality - "quick turn"
 * overrides constant-curvature turning for turn-in-place maneuvers.
 */
class CheesyDriveHelper
{
    public:
        CheesyDriveHelper( void );
        ~CheesyDriveHelper( void );

        wsDriveSignalT cheesyDrive( double throttle, double wheel, bool isQuickTurn );
        
        double handleDeadband( double val, double deadband );
        
        static const double sc_throttleDeadband;
        
    private:
        double a_quickStopAccumulator;
        wsDriveSignalT a_signal;
        
        static const double sc_wheelDeadband;
        static const double sc_turnSensitivity;
};

#endif

