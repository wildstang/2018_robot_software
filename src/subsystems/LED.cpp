#include "LED.h"

#include "utils/ws_string.h"
#include "io/routing/OutputSource.h"
#include "io/routing/InputSlot.h"
#include "config/ConfigValueRef.h"

#include "robot/WsRobot.h"

#include "subsystems/Claw.h"

LED::LED( void )
    : Subsystem( std::string( "led" ) )
    , ap_claw( NULL )
    , a_state( 0 )
    , a_allianceColor( 0 )
    , a_wantCubeButton( false )
    , a_useRunningLights( false )
    , a_inTeleop( false )
    , a_ledTributeMode( false )
    , a_pinOne( 0 )
    , a_pinTwo( 0 )
    , a_pinThree( 0 )
    , a_timer( )
{
}

LED::~LED( void )
{
}

void LED::init( void )
{
    ap_claw = WsRobot::getInstance()->getClaw();
    CONFIG_VAR( "led_tribute_mode", a_ledTributeMode, bool );
    CONFIG_VAR( "use_running_lights", a_useRunningLights, bool );

    CONNECT_OUTPUT( "led_pin_one", a_pinOne, bool );
    CONNECT_OUTPUT( "led_pin_two", a_pinTwo, bool );
    CONNECT_OUTPUT( "led_pin_three", a_pinThree, bool );
    // set alliance color here

    CONNECT_INPUT( "want_cube_button", inputWantCube, LED, bool );
    allianceColorT color = WsRobot::getInstance()->getHardware()->getAlliance();
    a_allianceColor = 0;
    if( color == ALLIANCE_COLOR_RED )
    {
        a_allianceColor = 1;
    }
    if( color == ALLIANCE_COLOR_BLUE )
    {
        a_allianceColor = 2;
    }
    if( !a_useRunningLights ) { a_allianceColor = 0; }
}

void LED::resetState( void )
{
    a_timer.stop();
    a_timer.reset();
}

void LED::update( void )
{
    a_state = a_allianceColor;

    if( ap_claw->getState() == CLAW_STATE_CLOSED )
    {
        a_state = 3;
    }

    if( a_wantCubeButton || ( !a_timer.hasPeriodPassed( 1.0 ) && a_timer.isRunning() ) )
    {
        a_state = 5;
    }

    if( !a_inTeleop )
    {
        a_state = a_allianceColor;
    }
    if( a_state == 0 && a_ledTributeMode )
    {
        a_state = 4;
    }

    setOutputs( a_state );
}

void LED::selfTest( void )
{

}

int LED::inputWantCube( bool buttonState )
{
    a_wantCubeButton = buttonState;

    if( buttonState )
    {
        a_timer.stop();
        a_timer.reset();
        a_timer.start();
    }

    return 0;
}

void LED::setOutputs( int state )
{
    a_pinOne = state & 0x00000001;
    a_pinTwo = state & 0x00000002;
    a_pinThree = state & 0x00000004;
}

void LED::setTeleop( bool teleop )
{
    a_inTeleop = teleop;
}

