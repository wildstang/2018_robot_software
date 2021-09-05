#ifndef __TURNPIDIO_H__
#define __TURNPIDIO_H__

#include "pid/PidOutput.h"
#include "pid/PidInput.h"

#include "subsystems/Drive.h"
#include "robot/WsRobot.h"

class TurnPidIO : public PidOutput, public PidInput
{
    public:
        TurnPidIO( void );
        virtual ~TurnPidIO( void );

        virtual void pidWrite( double value );
        virtual double pidRead( void );

        void setGyroValue( double value );

    private:
        double a_gyroValue;
};

#endif

