#ifndef __LIFTFINISHEDSTEP_H__
#define __LIFTFINISHEDSTEP_H__

#include "auto/steps/AutoStep.h"

class Lift;
class AutoStepFactoryArgs;

typedef enum
{
    TARGET_TYPE_NONE = 0,

    TARGET_TYPE_ON_TARGET,
    TARGET_TYPE_IN_RANGE,

    TARGET_TYPE_NUM
} targetTypeT;

class LiftFinishedStep : public AutoStep
{
    public:
        LiftFinishedStep( targetTypeT rangeType );
        LiftFinishedStep( AutoStepFactoryArgs* p_args );
        virtual ~LiftFinishedStep( void );

        virtual void initialize( void );
        virtual void update( void );

        virtual std::string getClassName( void );
        virtual AutoStepFactoryArgs getArguments( void );

    private:
        Lift* ap_lift;

        targetTypeT a_rangeType;
};

#endif
