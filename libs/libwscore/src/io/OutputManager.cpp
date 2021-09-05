#include "io/OutputManager.h"

#include "logger/WsLogger.h"
#include "io/Output.h"
#include "core/RobotCore.h"

OutputManager::OutputManager( RobotCore* p_robot )
    : WildstangModule( std::string( "outputs" ), p_robot )
    , a_outputs()
    , ap_robot( p_robot )
{
}

OutputManager::~OutputManager( void )
{
    std::map< std::string, Output* >::iterator i = a_outputs.begin();
    for(; i != a_outputs.end(); ++i )
    {
        Output* p_output = i->second;
        delete p_output;
    }

    a_outputs.clear();
}

void OutputManager::update( void )
{
    WS_FUNC_HEAD;

    // Iterate over all outputs and update each one
    std::map< std::string, Output* >::iterator i = a_outputs.begin();
    for(; i != a_outputs.end(); ++i )
    {
        //WS_TRACE( "Updating Output: %s", i->second->getName().c_str() );

        // Update the output - send value to output
        i->second->update();
    }

    WS_FUNC_FOOT;
}

void OutputManager::addOutput( Output* p_output )
{
    if( p_output == NULL )
    {
        WS_ERROR( "tried to add a null output!!" );
        return;
    }

    if( a_outputs.find( p_output->getName() ) == a_outputs.end() )
    {
        a_outputs[ p_output->getName() ] = p_output;
    }
    else
    {
        WS_WARNING( "output %s already found.", p_output->getName().c_str() );
    }
}

Output* OutputManager::getOutput( std::string name )
{
    Output* output = NULL;

    std::map< std::string, Output* >::iterator i = a_outputs.find( name );
    if( i == a_outputs.end() )
    {
        //throw new NoSuchElementException( "No input with name '" + p_name
        //                                  + "' in InputManager" );
    }
    else
    {
        output = i->second;
    }

    return output;
}

std::vector< std::string > OutputManager::listOutputs( void )
{
    std::vector< std::string > list;

    std::map< std::string, Output* >::iterator i = a_outputs.begin();
    for(; i != a_outputs.end(); ++i )
    {
        list.push_back( i->first );
    }

    return list;
}

void OutputManager::init( void )
{
    std::map< std::string, Output* >::iterator i = a_outputs.begin();
    for(; i != a_outputs.end(); ++i )
    {
        Output* p_output = i->second;
        WS_INFO( "instantiating output %s", p_output->getName().c_str() );
        p_output->instantiateOutput();
    }
}

