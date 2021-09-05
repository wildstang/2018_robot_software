#ifndef __LIFTSTEP_H__
#define __LIFTSTEP_H__

#include "auto/steps/AutoStep.h"

class Lift;
class AutoStepFactoryArgs;

typedef enum
{
    AUTO_LIFT_NONE = 0,

    AUTO_LIFT_POS_INTAKE,
    AUTO_LIFT_POS_INTAKE_PLUS_ONE,
    AUTO_LIFT_POS_CARRY,
    AUTO_LIFT_POS_CARRY_PLUS_ONE,
    AUTO_LIFT_POS_SWITCH,
    AUTO_LIFT_POS_SWITCH_PLUS_ONE,
    AUTO_LIFT_POS_SCALE,
    AUTO_LIFT_POS_SCALE_PLUS_ONE,
    AUTO_LIFT_POS_SCALE_REVERSE,
    AUTO_LIFT_POS_SCALE_REVERSE_PLUS_ONE,
    AUTO_LIFT_POS_SHOOT_SCALE,
    AUTO_LIFT_POS_SHOOT_SCALE_REVERSE,

    AUTO_LIFT_NUM
} autoLiftT;

class LiftStep : public AutoStep
{
    public:
        LiftStep( autoLiftT position );
        LiftStep( AutoStepFactoryArgs* p_args );
        virtual ~LiftStep( void );

        virtual void initialize( void );
        virtual void update( void );

        virtual std::string getClassName( void );
        virtual AutoStepFactoryArgs getArguments( void );

    private:
        Lift* ap_lift;
        autoLiftT a_position;
};

#endif

