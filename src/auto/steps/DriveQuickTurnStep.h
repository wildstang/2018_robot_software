#ifndef __DRIVEQUICKTURNSTEP_H__
#define __DRIVEQUICKTURNSTEP_H__

#include "auto/steps/AutoStep.h"
#include "utils/WsTimer.h"

class Drive;
class AutoStepFactoryArgs;

class DriveQuickTurnStep : public AutoStep
{
    public:
        DriveQuickTurnStep( double angle, double speed, double timeout = -1.0 );
        DriveQuickTurnStep( AutoStepFactoryArgs* p_args );
        virtual ~DriveQuickTurnStep( void );

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
