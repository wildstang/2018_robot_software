#ifndef __ARCANGLEPIDIO_H__
#define __ARCANGLEPIDIO_H__

#include "pid/PidOutput.h"
#include "pid/PidInput.h"
#include "subsystems/Drive.h"
#include "robot/WsRobot.h"

class ArcAnglePidIO : public PidOutput, public PidInput
{
    public:
        ArcAnglePidIO( void );
        virtual ~ArcAnglePidIO( void );

        virtual void pidWrite( double value );
        virtual double pidRead( void );

        void setEncoderValue( double value );
        void setGyroAngle( double value );
        void setRadius( double value );

    private:
        double a_encoderValue;
        double a_gyroAngle;
        double a_radius;
};

#endif