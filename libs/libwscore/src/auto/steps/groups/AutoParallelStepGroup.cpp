#include "AutoParallelStepGroup.h"

#include "auto/script/AutoStepFactory.h"
#include "logger/WsLogger.h"

AutoParallelStepGroup::AutoParallelStepGroup( void )
    : AutoStepGroup()
    , a_remaining()
{
}

AutoParallelStepGroup::AutoParallelStepGroup( AutoStepFactoryArgs* p_args )
    : AutoStepGroup()
    , a_remaining()
{
}

AutoParallelStepGroup::~AutoParallelStepGroup( void )
{
    a_remaining.clear();
}

void AutoParallelStepGroup::initialize( void )
{
    a_remaining.clear();

    std::vector< AutoStep* >::iterator i = a_steps.begin();
    for(; i != a_steps.end(); ++i )
    {
        AutoStep* p_step = *i;
        a_remaining.insert( p_step );
        p_step->initialize();
    }
}

void AutoParallelStepGroup::update( void )
{
    std::set< AutoStep* > still_running = a_remaining;

    std::set< AutoStep* >::iterator i = still_running.begin();
    for(; i != still_running.end(); ++i )
    {
        AutoStep* p_step = *i;
        p_step->update();

        if( p_step->isFinished() )
        {
            a_remaining.erase( p_step );
        }
    }

    if( a_remaining.empty() )
    {
        setFinished( true );
    }
}

std::string AutoParallelStepGroup::getClassName( void )
{
    return std::string( "AutoParallelStepGroup" );
}

