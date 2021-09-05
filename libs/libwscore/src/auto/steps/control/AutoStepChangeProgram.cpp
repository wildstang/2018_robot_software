#include "AutoStepChangeProgram.h"

#include "logger/WsLogger.h"
#include "auto/script/AutoStepFactory.h"
#include "core/RobotCore.h"
#include "auto/AutoManager.h"

AutoStepChangeProgram::AutoStepChangeProgram( std::string program_name, bool reversed )
    : AutoStep()
    , a_programName( program_name )
    , a_reversed( reversed )
{
}

AutoStepChangeProgram::AutoStepChangeProgram( AutoStepFactoryArgs* p_args )
    : AutoStep()
    , a_programName()
    , a_reversed( false )
{
    if( p_args )
    {
        p_args->getProgram( a_programName );
        p_args->optionalReversed( a_reversed );
    }
    else
    {
        WS_ERROR( "Did not receive proper arguments for step %s",
                  __PRETTY_FUNCTION__ );
    }
}

AutoStepChangeProgram::~AutoStepChangeProgram( void )
{
}

void AutoStepChangeProgram::initialize( void )
{
}

void AutoStepChangeProgram::update( void )
{
    RobotCore::getRobotCore()->getAutoManager()->changeProgram( a_programName, a_reversed );
}

std::string AutoStepChangeProgram::getClassName( void )
{
    return std::string( "AutoStepChangeProgram" );
}

AutoStepFactoryArgs AutoStepChangeProgram::getArguments( void )
{
    AutoStepFactoryArgs a;

    a.setProgram( a_programName );
    a.setReversed( a_reversed );

    return a;
}

