#ifndef __AUTOSTEPDELAY_H__
#define __AUTOSTEPDELAY_H__

#include "auto/steps/AutoStep.h"

#include "utils/WsTimer.h"

class AutoStepFactoryArgs;

class AutoStepDelay : public AutoStep
{
    public:        
        AutoStepDelay( int msDelay );
        AutoStepDelay( AutoStepFactoryArgs* p_args );
        virtual ~AutoStepDelay( void );
        
        virtual void initialize( void ) override;
        virtual void update( void ) override;
        
        virtual std::string getClassName( void ) override;
        virtual AutoStepFactoryArgs getArguments( void );

    protected:
        double a_delay;
        WsTimer a_timer;
};

#endif

