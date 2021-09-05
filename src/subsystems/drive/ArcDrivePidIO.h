#ifndef __ARCDRIVEPIDIO_H__
#define __ARCDRIVEPIDIO_H__

#include "pid/PidOutput.h"
#include "pid/PidInput.h"
#include "subsystems/Drive.h"
#include "robot/WsRobot.h"

class ArcDrivePidIO : public PidOutput, public PidInput
{
    public:
        ArcDrivePidIO( void );
        virtual ~ArcDrivePidIO( void );

        virtual void pidWrite( double value );
        virtual double pidRead( void );

        void setEncoderValue( double value );
        
    private:
        double a_encoderValue;
};

#endif