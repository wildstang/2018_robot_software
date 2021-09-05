#include "WsAutoStepFactory.h"

#include "auto/script/StepFactory.h"

#include "auto/steps/control/AutoStepStopAutonomous.h"
#include "auto/steps/control/AutoStepDelay.h"
#include "auto/steps/control/AutoStepChangeProgram.h"

#include "auto/steps/groups/AutoParallelStepGroup.h"
#include "auto/steps/groups/AutoParallelShortStepGroup.h"
#include "auto/steps/groups/AutoSerialStepGroup.h"

#include "auto/steps/ArmStep.h"
#include "auto/steps/LiftStep.h"
#include "auto/steps/ManipulatorStep.h"
#include "auto/steps/ClawStep.h"
#include "auto/steps/ArmFinishedStep.h"
#include "auto/steps/LiftFinishedStep.h"
#include "auto/steps/DriveStraightStep.h"
#include "auto/steps/DriveStraightQuickStep.h"
#include "auto/steps/DriveSetBrakeModeStep.h"
#include "auto/steps/DriveTurnStep.h"
#include "auto/steps/DriveQuickTurnStep.h"
#include "auto/steps/IntakeCubeStep.h"
#include "auto/steps/StartConfigStep.h"
#include "auto/steps/ScoreStep.h"

WsAutoStepFactory::WsAutoStepFactory( void )
{
}

WsAutoStepFactory::~WsAutoStepFactory( void )
{
}

int WsAutoStepFactory::createStepFactories( void )
{
    WS_AUTO_STEP( AutoStepDelay );
    WS_AUTO_STEP( AutoStepStopAutonomous );
    WS_AUTO_STEP( AutoStepChangeProgram );

    WS_AUTO_STEP( AutoParallelStepGroup );
    WS_AUTO_STEP( AutoParallelShortStepGroup );
    WS_AUTO_STEP( AutoSerialStepGroup );

    WS_AUTO_STEP( DriveStraightStep );
    WS_AUTO_STEP( DriveStraightQuickStep );
    WS_AUTO_STEP( DriveSetBrakeModeStep );
    WS_AUTO_STEP( DriveTurnStep );
    WS_AUTO_STEP( DriveQuickTurnStep );
    WS_AUTO_STEP( IntakeCubeStep );
    WS_AUTO_STEP( LiftStep );
    WS_AUTO_STEP( ManipulatorStep );
    WS_AUTO_STEP( ArmStep );
    WS_AUTO_STEP( ClawStep );
    WS_AUTO_STEP( LiftFinishedStep );
    WS_AUTO_STEP( ArmFinishedStep );

    WS_AUTO_STEP( StartConfigStep );
    WS_AUTO_STEP( ScoreStep );

    WS_AUTO_STEP_ALIAS( "DriveStep", DriveStraightQuickStep );
    WS_AUTO_STEP_ALIAS( "TurnStep", DriveQuickTurnStep );

    WS_DICTIONARY_ENUM( AUTO_LIFT_POS_INTAKE );
    WS_DICTIONARY_ENUM( AUTO_LIFT_POS_INTAKE_PLUS_ONE );
    WS_DICTIONARY_ENUM( AUTO_LIFT_POS_CARRY );
    WS_DICTIONARY_ENUM( AUTO_LIFT_POS_CARRY_PLUS_ONE );
    WS_DICTIONARY_ENUM( AUTO_LIFT_POS_SWITCH );
    WS_DICTIONARY_ENUM( AUTO_LIFT_POS_SWITCH_PLUS_ONE );
    WS_DICTIONARY_ENUM( AUTO_LIFT_POS_SCALE );
    WS_DICTIONARY_ENUM( AUTO_LIFT_POS_SCALE_PLUS_ONE );
    WS_DICTIONARY_ENUM( AUTO_LIFT_POS_SCALE_REVERSE );
    WS_DICTIONARY_ENUM( AUTO_LIFT_POS_SCALE_REVERSE_PLUS_ONE );
    WS_DICTIONARY_ENUM( AUTO_LIFT_POS_SHOOT_SCALE );

    WS_DICTIONARY_ENUM( AUTO_ARM_POS_INTAKE );
    WS_DICTIONARY_ENUM( AUTO_ARM_POS_INTAKE_PLUS_ONE );
    WS_DICTIONARY_ENUM( AUTO_ARM_POS_INTAKE_COMPLETE );
    WS_DICTIONARY_ENUM( AUTO_ARM_POS_SCORE_SWITCH );
    WS_DICTIONARY_ENUM( AUTO_ARM_POS_SCORE_SWITCH_PLUS_ONE );
    WS_DICTIONARY_ENUM( AUTO_ARM_POS_SCORE_SCALE );
    WS_DICTIONARY_ENUM( AUTO_ARM_POS_SCORE_SCALE_PLUS_ONE );
    WS_DICTIONARY_ENUM( AUTO_ARM_POS_SCORE_REVERSE );
    WS_DICTIONARY_ENUM( AUTO_ARM_POS_SHOOT_SCALE );
    WS_DICTIONARY_ENUM( AUTO_ARM_POS_CARRY );

    WS_DICTIONARY_ENUM( AUTO_INTAKE );
    WS_DICTIONARY_ENUM( AUTO_INTAKE_PLUS_ONE );
    WS_DICTIONARY_ENUM( AUTO_CARRY );
    WS_DICTIONARY_ENUM( AUTO_CARRY_PLUS_ONE );
    WS_DICTIONARY_ENUM( AUTO_SWITCH );
    WS_DICTIONARY_ENUM( AUTO_SWITCH_PLUS_ONE );
    WS_DICTIONARY_ENUM( AUTO_SCALE );
    WS_DICTIONARY_ENUM( AUTO_SCALE_PLUS_ONE );
    WS_DICTIONARY_ENUM( AUTO_SCALE_REVERSE );
    WS_DICTIONARY_ENUM( AUTO_SCALE_REVERSE_PLUS_ONE );
    WS_DICTIONARY_ENUM( AUTO_SHOOT_SCALE );
    WS_DICTIONARY_ENUM( AUTO_SHOOT_SCALE_REVERSE );

    WS_DICTIONARY_ENUM( AUTO_CLAW_POS_OPEN );
    WS_DICTIONARY_ENUM( AUTO_CLAW_POS_CLOSED );
    WS_DICTIONARY_ENUM( AUTO_CLAW_POS_RAGDOLL );

    WS_DICTIONARY_ENUM( AUTO_CLAW_DIRECTION_FWD );
    WS_DICTIONARY_ENUM( AUTO_CLAW_DIRECTION_REV );
    WS_DICTIONARY_ENUM( AUTO_CLAW_DIRECTION_REV_SLOWER );
    WS_DICTIONARY_ENUM( AUTO_CLAW_DIRECTION_REV_SLOWEST );
    WS_DICTIONARY_ENUM( AUTO_CLAW_DIRECTION_REV_BALLS_TO_WALL );
    WS_DICTIONARY_ENUM( AUTO_CLAW_DIRECTION_NEUTRAL );

    WS_DICTIONARY_ENUM( TARGET_TYPE_IN_RANGE );
    WS_DICTIONARY_ENUM( TARGET_TYPE_ON_TARGET );

    return 0;
}
