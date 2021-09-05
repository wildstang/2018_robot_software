#include "ModButtonInput.h"
#include "io/InputManager.h"
#include "io/OutputManager.h"
#include "io/Input.h"

#include <string>


ModButtonInput::ModButtonInput( std::string name, std::string mod_name, std::string target_name, bool modState )
    : DigitalInput( name, false )
    , a_modName( mod_name )
    , a_targetName( target_name )
    , ap_modButton( NULL )
    , ap_targetButton( NULL )
    , a_modState( modState )
    , a_modButtonPressed( false )
    , a_targetButtonPressed( false )
{
    this->setEmissionEnable( false );
}

ModButtonInput::~ModButtonInput( void )
{
}

int ModButtonInput::instantiate( void )
{
    ap_modButton = 
        RobotCore::getRobotCore()->getInputManager()->getInput( a_modName );
    ap_targetButton = 
        RobotCore::getRobotCore()->getInputManager()->getInput( a_targetName );

    if( !ap_modButton )
    {
        WS_ERROR( "input %s does not exist", a_modName.c_str() );
        return 1;
    }

    if( !ap_targetButton )
    {
        WS_ERROR( "input %s does not exist", a_targetName.c_str() );
        return 1;
    }

    ap_modButton->addInputListener( this );
    ap_targetButton->addInputListener( this );

    ap_modButton->flush();
    ap_targetButton->flush();

    return 0;
}

void ModButtonInput::inputUpdate( Input* p_source, bool value )
{
    if( p_source == ap_modButton )
    {
        a_modButtonPressed = value;
    }
    else if( p_source == ap_targetButton )
    {
        a_targetButtonPressed = value;
    }
}

bool ModButtonInput::readRawValue( void )
{
    bool state = false;

    if( a_modButtonPressed != a_modState )
    {
        // Sets the value of the target to false when it can't be called
        // in case the mod button is lifted before the target; prevents
        // info overflow into other button.
        a_targetButtonPressed = false;
    }
    else
    {
        state = a_targetButtonPressed;
    }

    return state;
}

