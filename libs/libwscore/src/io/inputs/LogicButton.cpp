#include "LogicButton.h"
#include "io/InputManager.h"
#include "io/OutputManager.h"
#include "io/Input.h"

#include <string>

LogicButton::LogicButton( std::string name, std::string first_button, 
                          bool first_invert, std::string second_button, 
                          bool second_invert, logicOperatorT logic )
    : DigitalInput( name, false )
    , a_firstName( first_button )
    , a_secondName( second_button )
    , ap_firstButton( NULL )
    , ap_secondButton( NULL )
    , a_firstButtonState( false )
    , a_secondButtonState( false )
    , a_firstInvert( first_invert )
    , a_secondInvert( second_invert )
    , a_logic( logic )
{
    this->setEmissionEnable( false );
}

LogicButton::~LogicButton( void )
{
}

int LogicButton::instantiate( void )
{
    ap_firstButton = 
        RobotCore::getRobotCore()->getInputManager()->getInput( a_firstName );
    ap_secondButton = 
        RobotCore::getRobotCore()->getInputManager()->getInput( a_secondName );

    if( !ap_firstButton )
    {
        WS_ERROR( "input %s does not exist", a_firstName.c_str() );
        return 1;
    }

    if( !ap_secondButton )
    {
        WS_ERROR( "input %s does not exist", a_secondName.c_str() );
        return 1;
    }

    ap_firstButton->addInputListener( this );
    ap_secondButton->addInputListener( this );

    ap_firstButton->flush();
    ap_secondButton->flush();

    return 0;
}


void LogicButton::inputUpdate( Input* p_source, bool value )
{
    if( p_source == ap_firstButton )
    {
        a_firstButtonState = a_firstInvert ^ value;
    }

    if( p_source == ap_secondButton )
    {
        a_secondButtonState = a_secondInvert ^ value;
    }
}

bool LogicButton::readRawValue( void )
{
    bool value = false;

    switch( a_logic )
    {
        case LOGIC_OPERATOR_AND:
            value = a_firstButtonState && a_secondButtonState;
            break;
        case LOGIC_OPERATOR_OR:
            value = a_firstButtonState || a_secondButtonState;
            break;
        case LOGIC_OPERATOR_XOR:
            value = a_firstButtonState ^ a_secondButtonState;
            break;
        case LOGIC_OPERATOR_NUM:
        case LOGIC_OPERATOR_NONE:
        default:
            break;
    }

    return value;
}

