#include "StartConfigStep.h"

#include "subsystems/Claw.h"
#include "subsystems/Arm.h"
#include "subsystems/Lift.h"
#include "robot/WsRobot.h"
#include "logger/WsLogger.h"

#include "auto/script/AutoStepFactoryArgs.h"

StartConfigStep::StartConfigStep( void )
    : AutoStep()
    , ap_claw( NULL )
    , ap_arm( NULL )
    , ap_lift( NULL )
{
}

StartConfigStep::StartConfigStep( AutoStepFactoryArgs* p_args )
    : AutoStep()
    , ap_claw( NULL )
    , ap_arm( NULL )
    , ap_lift( NULL )
{
}

StartConfigStep::~StartConfigStep( void )
{
}

void StartConfigStep::initialize( void )
{
    ap_claw = WsRobot::getInstance()->getClaw();
    ap_arm = WsRobot::getInstance()->getArm();
    ap_lift = WsRobot::getInstance()->getLift();
}

void StartConfigStep::update( void )
{
    ap_claw->setIntakeDirectionNeutral();
    ap_claw->setClawPositionClosed();
    ap_lift->setCarryPosition();
    ap_arm->setCarryPosition();
    setFinished( true );
}

std::string StartConfigStep::getClassName( void )
{
    return std::string( "StartConfigStep" );
}

AutoStepFactoryArgs StartConfigStep::getArguments( void )
{
    AutoStepFactoryArgs a;

    return a;
}

