#include "Input.h"

#include "logger/WsLogger.h"
#include "io/routing/InputListener.h"
#include "io/InputManager.h"
#include "io/inserts/WsIOInsert.h"

#include "utils/ws_string.h"
#include "utils/ws_debug.h"

Input::Input( InputManager* p_manager, std::string name )
    : WildstangModule( name, p_manager )
    , a_inserts()
    , a_valueChanged( true )
    , a_listeners()
    , a_name( name )
    , ap_inputManager( p_manager )
    , a_enabled( true )
    , a_instantiated( false )
    , a_instantiationWarningCount( 0 )
{
    WS_ASSERT( ws_is_well_formed_name( name.c_str() ) );
}

Input::~Input( void )
{
    this->clearInserts();
    a_listeners.clear();
}

void Input::addInputListener( InputListener* p_listener )
{
    if( p_listener == NULL )
    {
        WS_ERROR( "tried to add null input listener to %s",
                  this->getName().c_str() );
        return;
    }

    WS_FUNC_HEAD;
    
    //
    // worth the setup inefficiency for the gains in periodic
    // efficiency
    //

    bool found = false;
    std::vector< InputListener* >::iterator i = a_listeners.begin();
    for(; i != a_listeners.end(); ++i )
    {
        if( *i == p_listener )
        {
            found = true;
            break;
        }
    }

    if( !found )
    {
        a_listeners.push_back( p_listener );
    }
    
    WS_FUNC_FOOT;
}

void Input::notifyListeners( void )
{
    //WS_FUNC_HEAD;
    
    // If the value changed, notify listeners
    if( hasValueChanged() )
    {
        WS_TRACE( "Input %s: value has changed - notifying listeners",
            getName().c_str() );
        
        std::vector< InputListener* >::iterator i = a_listeners.begin();
        for(; i != a_listeners.end(); ++i )
        {
            InputListener* p_listener = *i;
            
            this->notifyListener( p_listener );
        }
    }
    //else
    //{
        //WS_TRACE( "Input value (%s) has not changed - not notifying",
        //    getName().c_str() );
    //}
    
    //WS_FUNC_FOOT;
}

void Input::update( void )
{
    if( !isInstantiated() )
    {
        if( a_instantiationWarningCount == 0 )
        {
            WS_WARNING( "tried to update input %s before instantiated",
                        this->getName().c_str() );
        }

        a_instantiationWarningCount = 
            ( a_instantiationWarningCount + 1 ) % 1000;

        return;
    }

    //WS_FUNC_HEAD;
    
    if( a_enabled )
    {
        // Read the raw input state
        //WS_TRACE( "Input %s: Reading data from input", getName().c_str() );

        readDataFromInput();

        // Notify any listeners
        notifyListeners();
    }
    else
    {
        //WS_TRACE( "Input %s: Disabled.", getName().c_str() );
    }
    
    //WS_FUNC_FOOT;
}

Input* Input::addInsert( WsIOInsert* p_insert )
{
    if( p_insert != NULL )
    {
        a_inserts.push_back( p_insert );
    }
    else
    {
        WS_ERROR( "tried to add a null insert to %s",
                  this->getName().c_str() );
    }

    return this;
}

void Input::clearInserts( void )
{
    std::vector< WsIOInsert* >::iterator i = a_inserts.begin();
    for(; i != a_inserts.end(); ++i )
    {
        WsIOInsert* p_insert = *i;
        delete p_insert;
    }

    a_inserts.clear();
}

void Input::instantiateInput( void )
{
    this->instantiate();
    this->notifyListeners();
    a_instantiated = true;
    WS_INFO( "%s instantiated.", this->getName().c_str() );
}

void Input::flush( void )
{
    a_valueChanged = true;
    this->notifyListeners();
}

