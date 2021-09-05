#include "AutoChooser.h"

#include <cstring>
#include <unistd.h>

#include "io/Input.h"
#include "io/InputManager.h"
#include "auto/AutoManager.h"
#include "utils/ws_utils.h"
#include "config/ConfigValueRef.h"

#include "robot/WsRobot.h"

AutoChooser::AutoChooser( void )
    : Subsystem( std::string( "auto_chooser" ) )
    , ac_delayTicks( 20 )
    , ac_maxDelay( 5.0 )
    , a_delayIndex( 0 )
    , a_programList()
    , a_rrIndex( 4 )
    , a_rlIndex( 2 )
    , a_llIndex( 3 )
    , a_lrIndex( 4 )
    , a_rrDefault()
    , a_rlDefault()
    , a_llDefault()
    , a_lrDefault()
    , a_modifyingElementIndex( AUTO_MOD_DELAY )
    , a_startPos( AUTO_START_LEFT )
    , a_infoUpdated( true )
    , a_initialized( false )
{
}

AutoChooser::~AutoChooser( void )
{
}

void AutoChooser::init( void )
{
    CONFIG_VAR( "auto_preg_rr_default", a_rrDefault, std::string );
    CONFIG_VAR( "auto_preg_rl_default", a_rlDefault, std::string );
    CONFIG_VAR( "auto_preg_lr_default", a_lrDefault, std::string );
    CONFIG_VAR( "auto_preg_ll_default", a_llDefault, std::string );

    CONNECT_INPUT( "next_field_auto_button", inputNextAutoButton, AutoChooser, bool );
    CONNECT_INPUT( "prev_field_auto_button", inputPrevAutoButton, AutoChooser, bool );
    CONNECT_INPUT( "prev_auto_button", inputNextFieldAutoButton, AutoChooser, bool );
    CONNECT_INPUT( "next_auto_button", inputPrevFieldAutoButton, AutoChooser, bool );
}

void AutoChooser::resetState( void )
{
#if 0
    a_rrDefault = this->getProgramAtIndex( a_rrIndex );
    a_rlDefault = this->getProgramAtIndex( a_rlIndex );
    a_llDefault = this->getProgramAtIndex( a_llIndex );
    a_lrDefault = this->getProgramAtIndex( a_lrIndex );

    WsRobot::getInstance()->saveConfigFile();

    sync();
#endif
}

void AutoChooser::startup( void )
{
#if 0
    a_programList = WsRobot::getInstance()->getAutoManager()->getListOfPrograms();
    std::map< std::string, unsigned int > index;
    ws_make_index( a_programList, index );

    if( a_programList.empty() )
    {
        a_programList.push_back( std::string( "no_programs" ) );
    }

    if( a_initialized )
    {
        std::map< std::string, unsigned int >::iterator i;

        i = index.find( rr_program );
        if( i != index.end() ) a_rrIndex = i->second;

        i = index.find( rl_program );
        if( i != index.end() ) a_rlIndex = i->second;

        i = index.find( ll_program );
        if( i != index.end() ) a_llIndex = i->second;

        i = index.find( lr_program );
        if( i != index.end() ) a_lrIndex = i->second;
    }
    else
    {
        std::map< std::string, unsigned int >::iterator i;

        i = index.find( a_rrDefault );
        if( i != index.end() ) a_rrIndex = i->second;

        i = index.find( a_rlDefault );
        if( i != index.end() ) a_rlIndex = i->second;

        i = index.find( a_llDefault );
        if( i != index.end() ) a_llIndex = i->second;

        i = index.find( a_lrDefault );
        if( i != index.end() ) a_lrIndex = i->second;
    }
#endif

    a_programList.push_back( "sleep" );
    a_programList.push_back( "drive" );
    a_programList.push_back( "three_cube" );
    a_programList.push_back( "one_and_one" );
    a_programList.push_back( "cross_over" );
    a_programList.push_back( "center_switch_left" );
    a_programList.push_back( "center_switch_right" );

    a_infoUpdated = true;
    a_initialized = true;
}

void AutoChooser::update( void )
{
    if( a_initialized && a_infoUpdated )
    {
        this->renderString();
        //printf( "%s", this->renderString().c_str() );
        a_infoUpdated = false;
    }
}

void AutoChooser::selfTest( void )
{
}

int AutoChooser::inputNextAutoButton( bool value )
{
    if( value )
    {
        switch( a_modifyingElementIndex )
        {
            case AUTO_MOD_RR:
                a_rrIndex = ( a_rrIndex + 1 ) % a_programList.size();
                break;
            case AUTO_MOD_RL:
                a_rlIndex = ( a_rlIndex + 1 ) % a_programList.size();
                break;
            case AUTO_MOD_LL:
                a_llIndex = ( a_llIndex + 1 ) % a_programList.size();
                break;
            case AUTO_MOD_LR:
                a_lrIndex = ( a_lrIndex + 1 ) % a_programList.size();
                break;
            case AUTO_MOD_POS:
                a_startPos = ( autoStartPosT )( ( ( int )a_startPos + 1 ) % 
                                                ( int )AUTO_START_NUM );
                break;
            case AUTO_MOD_DELAY:
                a_delayIndex = ( a_delayIndex + 1 ) % ac_delayTicks;
                break;
            default:
                a_modifyingElementIndex = AUTO_MOD_DELAY;
                break;
        }

        a_infoUpdated = true;
    }

    return 0;
}

int AutoChooser::inputPrevAutoButton( bool value )
{
    if( value )
    {
        switch( a_modifyingElementIndex )
        {
            case AUTO_MOD_RR:
                a_rrIndex = ( a_rrIndex +
                              ( a_programList.size() - 1 ) ) %
                            a_programList.size();
                break;
            case AUTO_MOD_RL:
                a_rlIndex = ( a_rlIndex +
                              ( a_programList.size() - 1 ) ) %
                            a_programList.size();
                break;
            case AUTO_MOD_LL:
                a_llIndex = ( a_llIndex +
                              ( a_programList.size() - 1 ) ) %
                            a_programList.size();
                break;
            case AUTO_MOD_LR:
                a_lrIndex = ( a_lrIndex +
                              ( a_programList.size() - 1 ) ) %
                            a_programList.size();
                break;
            case AUTO_MOD_POS:
                a_startPos = ( autoStartPosT )( ( ( int )a_startPos + 
                                                  ( int )AUTO_START_NUM - 1 ) % 
                                                ( int )AUTO_START_NUM );
                break;
            case AUTO_MOD_DELAY:
                a_delayIndex = ( a_delayIndex +
                                 ( ac_delayTicks - 1 ) ) %
                               ac_delayTicks;
                break;
            default:
                a_modifyingElementIndex = AUTO_MOD_DELAY;
                break;
        }

        a_infoUpdated = true;
    }

    return 0;
}

int AutoChooser::inputNextFieldAutoButton( bool value )
{
    if( value )
    {
        a_modifyingElementIndex =
            ( autoModT )( ( ( int ) a_modifyingElementIndex + 1 ) %
                          ( int ) AUTO_MOD_NUM );

        a_infoUpdated = true;
    }

    return 0;
}

int AutoChooser::inputPrevFieldAutoButton( bool value )
{
    if( value )
    {
        a_modifyingElementIndex =
            ( autoModT )( ( ( int ) a_modifyingElementIndex +
                            ( int )( AUTO_MOD_NUM - 1 ) ) %
                          ( int ) AUTO_MOD_NUM );

        a_infoUpdated = true;
    }

    return 0;
}

void AutoChooser::renderString( void )
{
    std::string rr_selected_program = 
        std::string( "RR: " ) + this->getProgramAtIndex( a_rrIndex );
    std::string rl_selected_program = 
        std::string( "RL: " ) + this->getProgramAtIndex( a_rlIndex );
    std::string ll_selected_program = 
        std::string( "LL: " ) + this->getProgramAtIndex( a_llIndex );
    std::string lr_selected_program = 
        std::string( "LR: " ) + this->getProgramAtIndex( a_lrIndex );
    std::string delay = ws_std_string_printf( "D: %.2f", this->getDelay() );
    std::string pos = std::string( "POS: " ) + toString( a_startPos );

    switch ( a_modifyingElementIndex )
    {
        default:
            a_modifyingElementIndex = AUTO_MOD_DELAY;
            // intentional fall through
        case AUTO_MOD_DELAY:
            delay = std::string( ">>>" ) + delay + "<<<";
            break;
        case AUTO_MOD_POS:
            pos = std::string( ">>>" ) + pos + "<<<";
            break;
        case AUTO_MOD_RR:
            rr_selected_program = std::string( ">>>" ) + rr_selected_program + "<<<";
            break;
        case AUTO_MOD_RL:
            rl_selected_program = std::string( ">>>" ) + rl_selected_program + "<<<";
            break;
        case AUTO_MOD_LL:
            ll_selected_program = std::string( ">>>" ) + ll_selected_program + "<<<";
            break;
        case AUTO_MOD_LR:
            lr_selected_program = std::string( ">>>" ) + lr_selected_program + "<<<";
            break;
    }

    printf( "\n" );
    printf( "%s\n", rr_selected_program.c_str() );
    printf( "%s\n", rl_selected_program.c_str() );
    printf( "%s\n", ll_selected_program.c_str() );
    printf( "%s\n", lr_selected_program.c_str() );
    printf( "%s\n", delay.c_str() );
    printf( "%s\n", pos.c_str() );
}

double AutoChooser::getDelay( void )
{
    double d = ( double )a_delayIndex;
    d /= ( double )ac_delayTicks;
    d *= ac_maxDelay;
    return d;
}

std::string AutoChooser::getProgramName( std::string letters )
{
    std::string program_name;
    ws_to_upper( letters );

    if( 0 == strncmp( letters.c_str(), "RR", 2 ) )
    {
        program_name = toString( a_startPos ) + "_" +
                       this->getProgramAtIndex( a_rrIndex );
    }
    else if( 0 == strncmp( letters.c_str(), "RL", 2 ) )
    {
        program_name = toString( a_startPos ) + "_" +
                       this->getProgramAtIndex( a_rlIndex );
    }
    else if( 0 == strncmp( letters.c_str(), "LL", 2 ) )
    {
        program_name = toString( a_startPos ) + "_" +
                       this->getProgramAtIndex( a_llIndex );
    }
    else if( 0 == strncmp( letters.c_str(), "LR", 2 ) )
    {
        program_name = toString( a_startPos ) + "_" +
                       this->getProgramAtIndex( a_lrIndex );
    }
    else
    {
        WS_ERROR( "wpilib is borked. big surprise." );
    }

    return program_name;
}

std::string AutoChooser::getProgramAtIndex( unsigned int index )
{
    std::string prog = std::string( "index_error" );

    if( index < a_programList.size() )
    {
        prog = a_programList[ index ];
    }

    return prog;
}

std::string toString( autoStartPosT pos )
{
    std::string str = std::string( "unknown" );

    switch( pos )
    {
        case AUTO_START_LEFT:
            str = std::string( "left" );
            break;
        case AUTO_START_CENTER:
            str = std::string( "center" );
            break;
        case AUTO_START_RIGHT:
            str = std::string( "right" );
            break;
        default:
            break;
    }

    return str;
}

