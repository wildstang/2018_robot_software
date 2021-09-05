#ifndef __INTAKECUBESTEP_H__
#define __INTAKECUBESTEP_H__

#include "auto/steps/AutoStep.h"

class Claw;
class AutoStepFactoryArgs;

class IntakeCubeStep : public AutoStep
{
    public:
        IntakeCubeStep( void );
        IntakeCubeStep( AutoStepFactoryArgs* p_args );
        virtual ~IntakeCubeStep( void );

        virtual void initialize( void );
        virtual void update( void );

        virtual std::string getClassName( void );
        virtual AutoStepFactoryArgs getArguments( void );

    private:
        Claw* ap_claw;
        bool a_hasStarted;
};

#endif

