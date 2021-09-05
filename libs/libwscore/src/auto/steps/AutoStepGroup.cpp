#include "AutoStepGroup.h"

#include "logger/WsLogger.h"
#include "utils/ws_debug.h"
#include "core/RobotCore.h"
#include "auto/AutoManager.h"
#include "auto/script/AutoStepFactory.h"
#include "auto/steps/control/AutoStepStopAutonomous.h"

AutoStepGroup::AutoStepGroup( void )
    : AutoStep()
    , a_steps()
{
}

AutoStepGroup::~AutoStepGroup( void )
{
    std::vector< AutoStep* >::iterator i = a_steps.begin();
    for(; i != a_steps.end(); ++i )
    {
        AutoStep* p_step = *i;
        delete p_step;
    }

    a_steps.clear();
}

void AutoStepGroup::addStep( AutoStep* p_step )
{
    if( p_step == NULL )
    {
        WS_WARNING( "tried to add null step." );
        return;
    }

    p_step->setDepth( this->getDepth() + 1 );

    a_steps.push_back( p_step );
}

std::string AutoStepGroup::generateCode( void )
{
    std::string code;

    std::string whitespace;
    int spaces = this->getDepth() * AUTO_STEP_GROUP_TABSTOP;
    for(; spaces > 0; spaces-- ) whitespace.push_back( ' ' );

    if( this->getDepth() > 0 )
    {
        code.append( whitespace + 
                     ws_std_string_printf( "@%s {\n", 
                                           this->getClassName().c_str() ) );
    }

    std::vector< AutoStep* >::iterator i = a_steps.begin();
    for(; i != a_steps.end(); ++i )
    {
        AutoStep* p_step = *i;
        code.append( p_step->generateCode() );
        code.append( "\n" );
    }

    if( this->getDepth() > 0 )
    {
        code.append( whitespace + "}\n" );
    }

    return code;
}

int AutoStepGroup::parseCode( const std::string& rc_code )
{
    std::string code = ws_strip_comments( rc_code );
    std::deque< std::string > tok = 
        ws_tokenize( rc_code.c_str(), " \n\r\t,", "@&(){};", "#", true );

    return this->parseCode( tok );
}

int AutoStepGroup::parseCode( std::deque< std::string >& r_tok )
{
    AutoStepFactory* p_factory = 
        RobotCore::getRobotCore()->getAutoManager()->getAutoStepFactory();

    std::vector< AutoStep* > steps;

    int err = 0;
    while( !r_tok.empty() )
    {
        AutoStep* p_step = NULL;

        std::string group_identifier = r_tok.front();
        if( group_identifier[ 0 ] == '}' )
        {
            r_tok.pop_front();
            break;
        }
        else if( group_identifier[ 0 ] == '@' )
        {
            // this is a group not a step.
            r_tok.pop_front();

            AutoStepGroup* p_group = p_factory->createGroup( r_tok );
            p_step = p_group;

            if( p_step != NULL )
            {
                err += p_group->parseCode( r_tok );
                if( err ) break;
            }
            else
            {
                WS_ERROR( "group create failure." );
                err++;
                break;
            }
        }
        else
        {
            p_step = p_factory->createStep( r_tok );
        }

        if( p_step == NULL )
        {
            WS_ERROR( "Error parsing auto step %s", 
                      ws_dbg_preview( r_tok, 5 ).c_str() );
            err++;
            break;
        }

        steps.push_back( p_step );
    }

    if( err )
    {
        this->addStep( new AutoStepStopAutonomous() );
    }
    else
    {
        std::vector< AutoStep* >::iterator j = steps.begin();
        for(; j != steps.end(); ++j )
        {
            this->addStep( *j );
        }
    }

    return err;
}

AutoStepGroup* AutoStepGroup::asGroup( void )
{
    return this;
}

