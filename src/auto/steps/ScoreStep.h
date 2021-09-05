#ifndef __SCORESTEP_H__
#define __SCORESTEP_H__

#include "auto/steps/AutoStep.h"
#include "utils/WsTimer.h"

class Claw;
class AutoStepFactoryArgs;

class ScoreStep : public AutoStep
{
    public:
        ScoreStep( int milliseconds = 350 );
        ScoreStep( AutoStepFactoryArgs* p_args );
        virtual ~ScoreStep( void );

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

