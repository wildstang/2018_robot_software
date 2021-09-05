#include "ArmFinishedStep.h"

#include "robot/WsRobot.h"
#include "subsystems/Arm.h"

ArmFinishedStep::ArmFinishedStep( void )
    : AutoStep()
    , ap_arm( NULL )
{
}

ArmFinishedStep::ArmFinishedStep( AutoStepFactoryArgs* p_args )
{
}

ArmFinishedStep::~ArmFinishedStep( void )
{
}

void ArmFinishedStep::initialize( void )
{
    ap_arm = WsRobot::getInstance()->getArm();
}

void ArmFinishedStep::update( void )
{
    if( ap_arm->isStabilized() )
    {
        setFinished( true );
    }
}

std::string ArmFinishedStep::getClassName( void )
{
    return std::string( "ArmFinishedStep" );
}

AutoStepFactoryArgs ArmFinishedStep::getArguments( void )
{
    AutoStepFactoryArgs a;

    return a;
}

