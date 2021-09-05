#ifndef __MANIPULATORSTEP_H__
#define __MANIPULATORSTEP_H__

#include "auto/steps/AutoStep.h"

class Lift;
class Arm;
class AutoStepFactoryArgs;

typedef enum
{
    AUTO_NONE = 0,

    AUTO_INTAKE,
    AUTO_INTAKE_PLUS_ONE,
    AUTO_CARRY,
    AUTO_CARRY_PLUS_ONE,
    AUTO_SWITCH,
    AUTO_SWITCH_PLUS_ONE,
    AUTO_SCALE,
    AUTO_SCALE_PLUS_ONE,
    AUTO_SCALE_REVERSE,
    AUTO_SCALE_REVERSE_PLUS_ONE,
    AUTO_SHOOT_SCALE,
    AUTO_SHOOT_SCALE_REVERSE,

    AUTO_NUM
} autoManipulatorT;

class ManipulatorStep : public AutoStep
{
    public:
        ManipulatorStep( autoManipulatorT position );
        ManipulatorStep( AutoStepFactoryArgs* p_args );
        virtual ~ManipulatorStep( void );

        virtual void initialize( void );
        virtual void update( void );

        virtual std::string getClassName( void );
        virtual AutoStepFactoryArgs getArguments( void );

    private:
        Lift* ap_lift;
        Arm* ap_arm;
        autoManipulatorT a_position;
};

#endif

