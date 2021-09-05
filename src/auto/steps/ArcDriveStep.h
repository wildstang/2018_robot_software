#ifndef __ARCDRIVESTEP_H__
#define __ARCDRIVESTEP_H__

#include "auto/steps/AutoStep.h"

#include "utils/WsTimer.h"

class Drive;
class AutoStepFactoryArgs;

class ArcDriveStep : public AutoStep
{
    public:
        ArcDriveStep( double radius, double angle, double speed, double timeout );
        ArcDriveStep( AutoStepFactoryArgs* p_args );
        virtual ~ArcDriveStep( void );

        virtual void initialize( void );
        virtual void update( void );
        virtual std::string getClassName( void );
        virtual AutoStepFactoryArgs getArguments( void );

    private:
        Drive* ap_drive;

        double a_radius;
        double a_angle;
        double a_speed;
        double a_timeout;

        WsTimer a_timeoutTimer;
};

#endif
