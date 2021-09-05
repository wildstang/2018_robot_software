#include "InputSlot.h"

#include "core/RobotCore.h"
#include "io/InputManager.h"
#include "io/Input.h"
#include "logger/WsLogger.h"
#include "utils/ws_debug.h"

#ifndef __INPUTSLOT_H__TEMPLATE__

InputSlotManager::InputSlotManager( void )
    : a_slots()
    , a_destructionMode( false )
{
}

InputSlotManager::~InputSlotManager( void )
{
    a_destructionMode = true;

    std::set< InputSlot* >::iterator i = a_slots.begin();
    for(; i != a_slots.end(); ++i )
    {
        InputSlot* p_slot = *i;
        delete p_slot;
    }

    a_destructionMode = false;
    a_slots.clear();
}

void InputSlotManager::addSlot( InputSlot* p_slot )
{
    a_slots.insert( p_slot );
}

void InputSlotManager::deletingSlot( InputSlot* p_slot )
{
    if( !a_destructionMode ) a_slots.erase( p_slot );
}

InputSlot::InputSlot( Input* p_input )
    : InputListener()
    , ap_input( p_input )
{
    WS_ASSERT( p_input != NULL );
    p_input->addInputListener( this );
}

InputSlot::~InputSlot( void )
{
    RobotCore::getRobotCore()->getInputManager()->getSlotManager()->deletingSlot( this );
}

void InputSlot::inputUpdate( Input* p_source, double value )
{
    if( p_source == ap_input ) this->inputReceive( value );
}

void InputSlot::inputUpdate( Input* p_source, bool value )
{
    if( p_source == ap_input ) this->inputReceive( value );
}

void InputSlot::inputUpdate( Input* p_source, int value )
{
    if( p_source == ap_input ) this->inputReceive( value );
}

void InputSlot::inputUpdate( Input* p_source, std::vector< unsigned char > value )
{
    if( p_source == ap_input ) this->inputReceive( value );
}

void InputSlot::inputReceive( const double& rc_value )
{
    WS_ERROR( "Input slot for %s not implemented!", ap_input->getName().c_str() );
}

void InputSlot::inputReceive( const bool& rc_value )
{
    WS_ERROR( "Input slot for %s not implemented!", ap_input->getName().c_str() );
}

void InputSlot::inputReceive( const int& rc_value )
{
    WS_ERROR( "Input slot for %s not implemented!", ap_input->getName().c_str() );
}

void InputSlot::inputReceive( const std::vector< unsigned char >& rc_value )
{
    WS_ERROR( "Input slot for %s not implemented!", ap_input->getName().c_str() );
}

#else

template< typename S, typename T >
FunctionInputSlot< S, T >::FunctionInputSlot( S* p_subsystem, 
                                              int ( S::*p_function )( T value ),
                                              Input* p_input )
    : InputSlot( p_input )
    , ap_subsystem( p_subsystem )
    , ap_inputSlotFunction( p_function )
{
}

template< typename S, typename T >
FunctionInputSlot< S, T >::~FunctionInputSlot( void )
{
}

template< typename S, typename T >
void FunctionInputSlot< S, T >::inputReceive( const T& rc_value )
{
    ( ap_subsystem->*ap_inputSlotFunction )( rc_value );
}

#endif

