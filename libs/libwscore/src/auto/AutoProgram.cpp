#include "AutoProgram.h"

#include "auto/steps/AutoStep.h"
#include "auto/steps/control/AutoStepStopAutonomous.h"
#include "auto/AutoManager.h"
#include "logger/WsLogger.h"
#include "auto/steps/groups/AutoSerialStepGroup.h"
#include "logger/WsSmartDashboard.h"
#include "core/RobotCore.h"

#include "utils/ws_debug.h"
#include "utils/ws_string.h"

AutoProgram::AutoProgram( std::string program_name )
    : ac_programName( program_name )
    , ac_hasCode( false )
    , a_code()
    , ap_steps( NULL )
{
    WS_ASSERT( ws_is_well_formed_name( program_name.c_str() ) );
}

AutoProgram::AutoProgram( std::string program_name, std::string code )
    : ac_programName( program_name )
    , ac_hasCode( true )
    , a_code( code )
    , ap_steps( NULL )
{
    WS_ASSERT( ws_is_well_formed_name( program_name.c_str() ) );
}

AutoProgram::~AutoProgram( void )
{
    this->cleanup();
}

void AutoProgram::initialize( void )
{
    this->cleanup();

    ap_steps = new AutoSerialStepGroup();
    this->defineSteps();

    ap_steps->initialize();
}

void AutoProgram::cleanup( void )
{
    if( ap_steps != NULL )
    {
        delete ap_steps;
        ap_steps = NULL;
    }
}

std::string AutoProgram::getCode( void )
{
    if( ac_hasCode )
    {
        return a_code;
    }
    else
    {
        return this->generateCode();
    }
}

std::string AutoProgram::generateCode( void )
{
    std::string code;

    if( ap_steps == NULL )
    {
        ap_steps = new AutoSerialStepGroup();
        this->defineSteps();

        code = ap_steps->generateCode();

        this->cleanup();
    }
    else
    {
        code = ap_steps->generateCode();
    }

    return code;
}

void AutoProgram::defineSteps( void )
{
    if( ap_steps == NULL )
    {
        WS_ERROR( "auto program not initialized, cannot define steps." );
        return;
    }

    int err = ap_steps->parseCode( a_code );
    if( err )
    {
        WS_ERROR( "could not load the auto program %s", ac_programName.c_str() );
    }
}

void AutoProgram::addStep( AutoStep* p_step )
{
    if( ap_steps != NULL )
    {
        ap_steps->addStep( p_step );
    }
    else
    {
        WS_ERROR( "can't add steps to auto program when not initializing" );
        delete p_step;
    }
}

bool AutoProgram::checkSyntax( void )
{
    bool good_syntax = false;

    if( ap_steps == NULL )
    {
        ap_steps = new AutoSerialStepGroup();

        int err = ap_steps->parseCode( a_code );
        good_syntax = ( err == 0 );

        this->cleanup();
    }

    return good_syntax;
}

void AutoProgram::update( void )
{
    if( ap_steps )
    {
        ap_steps->update();
    }
}

bool AutoProgram::isFinished( void )
{
    bool finished = true;

    if( ap_steps )
    {
        finished = ap_steps->isFinished();
    }

    return finished;
}

