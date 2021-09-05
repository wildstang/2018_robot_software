#ifndef __STARTCONFIGSTEP_H__
#define __STARTCONFIGSTEP_H__

#include "auto/steps/AutoStep.h"

class Claw;
class Arm;
class Lift;
class AutoStepFactoryArgs;

class StartConfigStep : public AutoStep
{
    public:
        StartConfigStep( void );
        StartConfigStep( AutoStepFactoryArgs* p_args );
        virtual ~StartConfigStep( void );

        virtual void initialize( void );
        virtual void update( void );

        virtual std::string getClassName( void );
        virtual AutoStepFactoryArgs getArguments( void );

    private:
        Claw* ap_claw;
        Arm* ap_arm;
        Lift* ap_lift;
};

#endif

