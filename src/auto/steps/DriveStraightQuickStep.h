#ifndef __DRIVESTRAIGHTQUICKSTEP_H__
#define __DRIVESTRAIGHTQUICKSTEP_H__

#include "auto/steps/AutoStep.h"
#include "utils/WsTimer.h"

class Drive;
class AutoStepFactoryArgs;

class DriveStraightQuickStep : public AutoStep
{
    public:
        DriveStraightQuickStep( double speed, int inchesToTravel, double timeout = -1.0 );
        DriveStraightQuickStep( AutoStepFactoryArgs* p_args );
        virtual ~DriveStraightQuickStep( void );

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
