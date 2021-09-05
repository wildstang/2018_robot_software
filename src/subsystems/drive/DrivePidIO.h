#ifndef __DRIVEPIDIO_H__
#define __DRIVEPIDIO_H__

#include "pid/PidOutput.h"
#include "pid/PidInput.h"
#include "subsystems/Drive.h"
#include "robot/WsRobot.h"

class DrivePidIO : public PidOutput, public PidInput
{
    public:
        DrivePidIO( void );
        virtual ~DrivePidIO( void );

        virtual void pidWrite( double value );
        virtual double pidRead( void );

        void setEncoderValue( double value );

    private:
        double a_encoderValue;
};

#endif

