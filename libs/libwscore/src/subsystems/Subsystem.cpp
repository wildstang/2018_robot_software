#include "Subsystem.h"

#include "core/RobotCore.h"
#include "subsystems/SubsystemManager.h"
#include "logger/WsLogger.h"

Subsystem::Subsystem( std::string name )
    : WildstangModule( name, RobotCore::getRobotCore()->getSubsystemManager() )
    , a_name( name )
{
    WS_INFO( "subsystem constructor: %s", name.c_str() );
}

Subsystem::~Subsystem( void )
{
    WS_INFO( "subsystem destructor: %s", this->getName().c_str() );
}

std::string Subsystem::getName( void )
{
    return a_name;
}

