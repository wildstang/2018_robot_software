#ifndef __DROPSTEP_H__
#define __DROPSTEP_H__

#include "auto/steps/AutoStep.h"
#include "utils/WsTimer.h"

class Claw;
class AutoStepFactoryArgs;

class DropStep : public AutoStep
{
    public:
        DropStep( int milliseconds = 500 );
        DropStep( AutoStepFactoryArgs* p_args );
        virtual ~DropStep( void );

        virtual void initialize( void );
        virtual void update( void );

        virtual std::string getClassName( void );
        virtual AutoStepFactoryArgs getArguments( void );

    private:
        Claw* ap_claw;
        double a_delay;
        bool a_hasStarted;

        WsTimer a_timer;
};

#endif

