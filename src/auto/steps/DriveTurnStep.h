#ifndef __DRIVETURNSTEP_H__
#define __DRIVETURNSTEP_H__

#include "auto/steps/AutoStep.h"
#include "utils/WsTimer.h"

class Drive;
class AutoStepFactoryArgs;

class DriveTurnStep : public AutoStep
{
    public:
        DriveTurnStep( double angle, double speed, double timeout = -1.0 );
        DriveTurnStep( AutoStepFactoryArgs* p_args );
        virtual ~DriveTurnStep( void );

        virtual void initialize( void );
        virtual void update( void );
        virtual std::string getClassName( void );
        virtual AutoStepFactoryArgs getArguments( void );

    private:
        Drive* ap_drive;

        double a_angle;
        double a_speed;
        double a_timeout;

        WsTimer a_timeoutTimer;
};

#endif
