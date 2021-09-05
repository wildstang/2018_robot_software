#ifndef __DRIVEHEADINGPIDIO_H__
#define __DRIVEHEADINGPIDIO_H__

#include "pid/PidOutput.h"
#include "pid/PidInput.h"

#include "subsystems/Drive.h"
#include "robot/WsRobot.h"

class DriveHeadingPidIO : public PidOutput, public PidInput
{
    public:
        DriveHeadingPidIO( void );
        virtual ~DriveHeadingPidIO( void );

        virtual void pidWrite( double value );
        virtual double pidRead( void );

        void setGyroValue( double value );

    private:
        double a_gyroValue;
};

#endif

