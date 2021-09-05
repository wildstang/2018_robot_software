#include "SubsystemManager.h"

#include "logger/WsLogger.h"
#include "subsystems/Subsystem.h"
#include "core/RobotCore.h"

SubsystemManager::SubsystemManager( void )
    : WildstangModule( std::string( "subsystems" ), RobotCore::getRobotCore() )
    , a_subsystems()
{
}

SubsystemManager::~SubsystemManager( void )
{
    std::vector< Subsystem* >::iterator i = a_subsystems.begin();
    for(; i != a_subsystems.end(); ++i )
    {
        Subsystem* p_subsystem = *i;
        delete p_subsystem;
    }

    a_subsystems.clear();
}

void SubsystemManager::init( void )
{
    WS_FUNC_HEAD;
    
    std::vector< Subsystem* >::iterator i = a_subsystems.begin();
    for(; i != a_subsystems.end(); ++i )
    {
        Subsystem* p_sub = *i;

        WS_INFO( "Initializing Subsystem: %s", p_sub->getName().c_str() );
        
        // Init all subsystems.
        p_sub->init();
    }
    
    WS_FUNC_FOOT;
}

void SubsystemManager::update( void )
{
    WS_FUNC_HEAD;
    
    // Iterate over all outputs and update each one
    std::vector< Subsystem* >::iterator i = a_subsystems.begin();
    for(; i != a_subsystems.end(); ++i )
    {
        Subsystem* p_sub = *i;

        WS_TRACE( "Updating Subsystem: %s", p_sub->getName().c_str() );
        
        // Update the output - send value to output
        p_sub->update();
    }
    
    WS_FUNC_FOOT;
}

void SubsystemManager::addSubsystem( Subsystem* p_subsystem )
{
    if( p_subsystem == NULL )
    {
        WS_ERROR( "tried to register a null subsystem" );
        return;
    }

    a_subsystems.push_back( p_subsystem );
    WS_INFO( "registered subsystem %s", p_subsystem->getName().c_str() );
}

void SubsystemManager::selfTest( void )
{
    WS_FUNC_HEAD;
    
    std::vector< Subsystem* >::iterator i = a_subsystems.begin();
    for(; i != a_subsystems.end(); ++i )
    {
        Subsystem* p_s = *i;
        p_s->selfTest();
    }
    
    WS_FUNC_FOOT;
}

void SubsystemManager::resetState( void )
{
    WS_FUNC_HEAD;

    std::vector< Subsystem* >::iterator i = a_subsystems.begin();
    for(; i != a_subsystems.end(); ++i )
    {
        WS_INFO( "Resetting subsystem: %s", ( *i )->getName().c_str() );

        ( *i )->resetState();
    }

    WS_FUNC_FOOT;
}

