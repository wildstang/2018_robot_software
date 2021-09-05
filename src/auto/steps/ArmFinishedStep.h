#ifndef __ARMFINISHEDSTEP_H__
#define __ARMFINISHEDSTEP_H__

#include "auto/steps/AutoStep.h"

class Arm;
class AutoStepFactoryArgs;

class ArmFinishedStep : public AutoStep
{
    public:
        ArmFinishedStep( void );
        ArmFinishedStep( AutoStepFactoryArgs* p_args );
        virtual ~ArmFinishedStep( void );

        virtual void initialize( void );
        virtual void update( void );

        virtual std::string getClassName( void );
        virtual AutoStepFactoryArgs getArguments( void );

    private:
        Arm* ap_arm;
};

#endif

