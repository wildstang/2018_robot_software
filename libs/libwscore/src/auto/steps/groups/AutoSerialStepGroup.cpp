#include "AutoSerialStepGroup.h"

#include "logger/WsSmartDashboard.h"
#include "logger/WsLogger.h"
#include "auto/script/AutoStepFactory.h"

AutoSerialStepGroup::AutoSerialStepGroup( void )
    : AutoStepGroup()
    , a_currentStep( 0 )
    , a_finishedPreviousStep( false )
{
}

AutoSerialStepGroup::AutoSerialStepGroup( AutoStepFactoryArgs* p_args )
    : AutoStepGroup()
    , a_currentStep( 0 )
    , a_finishedPreviousStep( false )
{
}

AutoSerialStepGroup::~AutoSerialStepGroup( void )
{
}

void AutoSerialStepGroup::initialize( void )
{
    a_finishedPreviousStep = false;
    a_currentStep = 0;

    if( !a_steps.empty() )
    {
        a_steps[ a_currentStep ]->initialize();
    }
    else
    {
        WS_ERROR( "empty steps." );
    }
}

void AutoSerialStepGroup::update( void )
{
    if( isFinished() )
    {
        return;
    }

    if( a_finishedPreviousStep )
    {
        a_finishedPreviousStep = false;
        a_currentStep++;

        if( a_currentStep >= a_steps.size() )
        {
            // We have reached the end of our list of a_steps, we're finished
            setFinished( true );
            return;
        }
        else
        {
            a_steps[ a_currentStep ]->initialize();
        }
    }

    AutoStep* p_step = a_steps[ a_currentStep ];
    SD_PUT( "current auto step", p_step->getClassName() );
    p_step->update();

    if( p_step->isFinished() )
    {
        a_finishedPreviousStep = true;
    }
}

std::string AutoSerialStepGroup::getClassName( void )
{
    return std::string( "AutoSerialStepGroup" );
}

AutoStep* AutoSerialStepGroup::getCurrentStep( void )
{
    return a_steps[ a_currentStep ];
}

AutoStep* AutoSerialStepGroup::getNextStep( void )
{
    if( a_currentStep + 1 < a_steps.size() )
    {
        return a_steps[ a_currentStep + 1 ];
    }
    else
    {
        return NULL;
    }
}

