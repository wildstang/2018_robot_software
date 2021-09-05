#ifndef __DRIVESTRAIGHTSTEP_H__
#define __DRIVESTRAIGHTSTEP_H__

#include "auto/steps/AutoStep.h"
#include "utils/WsTimer.h"

class Drive;
class AutoStepFactoryArgs;

class DriveStraightStep : public AutoStep
{
    public:
        DriveStraightStep( double speed, int inchesToTravel, double timeout = -1.0 );
        DriveStraightStep( AutoStepFactoryArgs* p_args );
        virtual ~DriveStraightStep( void );

        virtual void initialize( void );
        virtual void update( void );
        virtual std::string getClassName( void );
        virtual AutoStepFactoryArgs getArguments( void );

    private:
        Drive* ap_drive;

        double a_speed;
        int a_distanceToGo;
        double a_timeout;

        WsTimer a_timeoutTimer;
};

#endif

