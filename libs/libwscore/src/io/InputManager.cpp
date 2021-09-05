#include "InputManager.h"

#include "logger/WsLogger.h"
#include "io/Input.h"
#include "core/RobotCore.h"

InputManager::InputManager( RobotCore* p_robot )
    : WildstangModule( std::string( "inputs" ), p_robot )
    , a_inputs()
    , ap_robot( p_robot )
    , a_slotManager()
{
}

InputManager::~InputManager( void )
{
    std::map< std::string, Input* >::iterator i = a_inputs.begin();
    for(; i != a_inputs.end(); ++i )
    {
        Input* p_input = i->second;
        delete p_input;
    }

    a_inputs.clear();
}

void InputManager::update( void )
{
    WS_FUNC_HEAD;

    // Iterate over all inputs and update each one
    std::map< std::string, Input* >::iterator i = a_inputs.begin();
    for(; i != a_inputs.end(); ++i )
    {
        i->second->update();
    }

    WS_FUNC_FOOT;
}

void InputManager::addInput( Input* p_input )
{
    if( p_input == NULL )
    {
        WS_ERROR( "tried to add a null input!!" );
        return;
    }

    if( a_inputs.find( p_input->getName() ) == a_inputs.end() )
    {
        a_inputs[ p_input->getName() ] = p_input;
    }
    else
    {
        WS_WARNING( "input %s already exists.",
                    p_input->getName().c_str() );
        delete p_input;
    }
}

Input* InputManager::getInput( std::string name )
{
    Input* result = NULL;

    std::map< std::string, Input* >::iterator i = a_inputs.find( name );
    if( i == a_inputs.end() )
    {
        WS_ERROR( "no input named %s", name.c_str() );
    }
    else
    {
        result = i->second;
    }

    return result;
}

int InputManager::becomeListener( std::string input, InputListener* p_listener )
{
    Input* p_input = this->getInput( input );
    if( p_input == NULL )
    {
        WS_ERROR( "was not able to listen for %s because it does not exist",
                  input.c_str() );
        return 1;
    }

    p_input->addInputListener( p_listener );

    return 0;
}

std::vector< std::string > InputManager::listInputs( void )
{
    std::vector< std::string > list;

    std::map< std::string, Input* >::iterator i = a_inputs.begin();
    for(; i != a_inputs.end(); ++i )
    {
        list.push_back( i->first );
    }

    return list;
}

void InputManager::init( void )
{
    std::map< std::string, Input* >::iterator i = a_inputs.begin();
    for(; i != a_inputs.end(); ++i )
    {
        Input* p_input = i->second;
        WS_INFO( "instantiating input: %s", p_input->getName().c_str() );
        p_input->instantiateInput();
    }
}

