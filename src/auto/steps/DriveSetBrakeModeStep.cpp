#include "DriveSetBrakeModeStep.h"

#include "subsystems/Drive.h"
#include "robot/WsRobot.h"
#include "logger/WsLogger.h"

#include "auto/script/AutoStepFactoryArgs.h"

DriveSetBrakeModeStep::DriveSetBrakeModeStep( bool brake )
    : AutoStep()
    , ap_drive( NULL )
    , a_brake( brake )
{
}

DriveSetBrakeModeStep::DriveSetBrakeModeStep( AutoStepFactoryArgs* p_args )
    : AutoStep()
    , ap_drive( NULL )
    , a_brake( false )
{
    if( p_args )
    {
        p_args->getState( this->a_brake );
    }
    else
    {
        WS_ERROR( "not enough arguments for %s",
                  __PRETTY_FUNCTION__ );
    }
}

DriveSetBrakeModeStep::~DriveSetBrakeModeStep( void )
{
}

void DriveSetBrakeModeStep::initialize( void )
{
    ap_drive = WsRobot::getInstance()->getDrive();
}

void DriveSetBrakeModeStep::update( void )
{
    ap_drive->setBrakeMode( a_brake );
    setFinished( true );
}

std::string DriveSetBrakeModeStep::getClassName( void )
{
    return std::string( "DriveSetBrakeModeStep" );
}

AutoStepFactoryArgs DriveSetBrakeModeStep::getArguments( void )
{
    AutoStepFactoryArgs a;

    a.setState( this->a_brake );

    return a;
}
