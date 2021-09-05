#ifndef __CLAWSTEP_H__
#define __CLAWSTEP_H__

#include "auto/steps/AutoStep.h"

class Claw;
class AutoStepFactoryArgs;

typedef enum
{
    AUTO_CLAW_POS_NONE = 0,

    AUTO_CLAW_POS_OPEN,
    AUTO_CLAW_POS_CLOSED,
    AUTO_CLAW_POS_RAGDOLL,

    AUTO_CLAW_POS_NUM
} clawAutoPositionT;

typedef enum
{
    AUTO_CLAW_DIRECTION_NONE = 0,

    AUTO_CLAW_DIRECTION_FWD,
    AUTO_CLAW_DIRECTION_REV,
    AUTO_CLAW_DIRECTION_REV_SLOWER,
    AUTO_CLAW_DIRECTION_REV_SLOWEST,
    AUTO_CLAW_DIRECTION_NEUTRAL,
    AUTO_CLAW_DIRECTION_REV_BALLS_TO_WALL,

    AUTO_CLAW_DIRECTION_NUM
} clawAutoDirectionT;

class ClawStep : public AutoStep
{
    public:
        ClawStep( clawAutoPositionT position, clawAutoDirectionT direction );
        ClawStep( AutoStepFactoryArgs* p_args );
        virtual ~ClawStep( void );

        virtual void initialize( void );
        virtual void update( void );
        virtual std::string getClassName( void );
        virtual AutoStepFactoryArgs getArguments( void );

    private:
        Claw* ap_claw;
        clawAutoPositionT a_position;
        clawAutoDirectionT a_direction;
};

#endif
