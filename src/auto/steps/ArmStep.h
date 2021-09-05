#ifndef __ARMSTEP_H__
#define __ARMSTEP_H__

#include "auto/steps/AutoStep.h"

class Arm;
class AutoStepFactoryArgs;

typedef enum
{
    AUTO_ARM_NONE = 0,

    AUTO_ARM_POS_INTAKE,
    AUTO_ARM_POS_INTAKE_PLUS_ONE,
    AUTO_ARM_POS_INTAKE_COMPLETE,
    AUTO_ARM_POS_SCORE_SWITCH,
    AUTO_ARM_POS_SCORE_SWITCH_PLUS_ONE,
    AUTO_ARM_POS_SCORE_SCALE,
    AUTO_ARM_POS_SCORE_SCALE_PLUS_ONE,
    AUTO_ARM_POS_SCORE_REVERSE,
    AUTO_ARM_POS_SHOOT_SCALE,
    AUTO_ARM_POS_SHOOT_SCALE_REVERSE,
    AUTO_ARM_POS_CARRY,

    AUTO_ARM_NUM
} autoArmT;

class ArmStep : public AutoStep
{
    public:
        ArmStep( autoArmT position );
        ArmStep( AutoStepFactoryArgs* p_args );
        virtual ~ArmStep( void );

        virtual void initialize( void );
        virtual void update( void );

        virtual std::string getClassName( void );
        virtual AutoStepFactoryArgs getArguments( void );

    private:
        Arm* ap_arm;
        autoArmT a_position;
};

#endif

