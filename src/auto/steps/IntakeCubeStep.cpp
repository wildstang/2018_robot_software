#include "IntakeCubeStep.h"

#include "subsystems/Claw.h"
#include "robot/WsRobot.h"
#include "logger/WsLogger.h"

#include "auto/script/AutoStepFactoryArgs.h"

IntakeCubeStep::IntakeCubeStep( void )
    : AutoStep()
    , ap_claw( NULL )
    , a_hasStarted( false )
{
}

IntakeCubeStep::IntakeCubeStep( AutoStepFactoryArgs* p_args )
    : AutoStep()
    , ap_claw( NULL )
    , a_hasStarted( false )
{
}

IntakeCubeStep::~IntakeCubeStep( void )
{
}

void IntakeCubeStep::initialize( void )
{
    ap_claw = WsRobot::getInstance()->getClaw();
}

void IntakeCubeStep::update( void )
{
    if( !a_hasStarted )
    {
        ap_claw->setIntakeDirectionForward();
        a_hasStarted = true;
    }

    if( ap_claw->clawHasCube() )
    {
        ap_claw->setIntakeDirectionNeutral();
        setFinished( true );
    }
}

std::string IntakeCubeStep::getClassName( void )
{
    return std::string( "IntakeCubeStep" );
}

AutoStepFactoryArgs IntakeCubeStep::getArguments( void )
{
    AutoStepFactoryArgs a;

    return a;
}

