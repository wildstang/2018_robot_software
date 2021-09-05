#include "AutoStep.h"

#include "logger/WsLogger.h"

AutoStep::AutoStep( void )
    : InputListener()
    , a_finished( false )
    , a_depth( 0 )
{
}

AutoStep::~AutoStep( void )
{
}

bool AutoStep::isFinished( void )
{
    return a_finished;
}

void AutoStep::setFinished( bool is_finished )
{
    if( !a_finished && is_finished )
    {
        if( this->asGroup() )
        {
            WS_INFO( "group %s finished", this->getClassName().c_str() );
        }
        else
        {
            WS_INFO( "step %s finished", this->generateCode().c_str() );
        }
    }

    a_finished = is_finished;
}

std::string AutoStep::generateCode( void )
{
    std::string code;
    int spaces = this->getDepth() * AUTO_STEP_GROUP_TABSTOP;
    for(; spaces > 0; spaces-- ) code.push_back( ' ' );

    AutoStepFactoryArgs args = this->getArguments();
    args.setName( this->getClassName() );
    code.append( args.generateCode() );

    if( args.errorsOccurred() )
    {
        WS_ERROR( "problems building the code for the AutoStep %s",
                  this->getClassName().c_str() );
        code.clear();
    }

    return code;
}

AutoStepFactoryArgs AutoStep::getArguments( void )
{
    WS_WARNING( "%s not implemented in auto step %s",
                __PRETTY_FUNCTION__, this->getClassName().c_str() );

    AutoStepFactoryArgs a;
    a.setErrorFlag();
    return a;
}

AutoStepGroup* AutoStep::asGroup( void )
{
    return NULL;
}

