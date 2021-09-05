#include "AutoParallelShortStepGroup.h"

#include "auto/script/AutoStepFactory.h"
#include "logger/WsLogger.h"

AutoParallelShortStepGroup::AutoParallelShortStepGroup( void )
    : AutoStepGroup()
{
}

AutoParallelShortStepGroup::AutoParallelShortStepGroup( AutoStepFactoryArgs* p_args )
    : AutoStepGroup()
{
}

AutoParallelShortStepGroup::~AutoParallelShortStepGroup( void )
{
}

void AutoParallelShortStepGroup::initialize( void )
{
    std::vector< AutoStep* >::iterator i = a_steps.begin();
    for(; i != a_steps.end(); ++i )
    {
        AutoStep* p_step = *i;
        p_step->initialize();
    }
}

void AutoParallelShortStepGroup::update( void )
{
    std::vector< AutoStep* >::iterator i = a_steps.begin();
    for(; i != a_steps.end(); ++i )
    {
        AutoStep* p_step = *i;
        p_step->update();

        if( p_step->isFinished() )
        {
            setFinished( true );
        }
    }
}

std::string AutoParallelShortStepGroup::getClassName( void )
{
    return std::string( "AutoParallelShortStepGroup" );
}

