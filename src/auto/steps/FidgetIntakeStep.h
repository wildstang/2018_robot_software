#ifndef __FIDGETINTAKESTEP_H__
#define __FIDGETINTAKESTEP_H__

#include "auto/steps/AutoStep.h"
#include "utils/WsTimer.h"

class Claw;
class AutoStepFactoryArgs;

class FidgetIntakeStep : public AutoStep
{
    public:
        FidgetIntakeStep( int milliseconds = 800 );
        FidgetIntakeStep( AutoStepFactoryArgs* p_args );
        virtual ~FidgetIntakeStep( void );

        virtual void initialize( void );
        virtual void update( void );

        virtual std::string getClassName( void );
        virtual AutoStepFactoryArgs getArguments( void );

    private:
        Claw* ap_claw;
        double a_delay;
        bool a_fidgetOne;
        bool a_fidgetTwo;

        WsTimer a_timer;
};

#endif

