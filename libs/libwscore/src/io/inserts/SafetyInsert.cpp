#include "SafetyInsert.h"

#include "utils/ws_utils.h"

SafetyInsert::SafetyInsert( void )
    : WsIOInsert()
    , a_currentValue( false )
    , a_lastValue( false )
    , a_timeActivated( 0 )
    , a_timeRequired( 2000 )
{
}

SafetyInsert::SafetyInsert( uint64_t timeRequired )
    : WsIOInsert()
    , a_currentValue( false )
    , a_lastValue( false )
    , a_timeActivated( 0 )
    , a_timeRequired( timeRequired )
{
}

SafetyInsert::~SafetyInsert( void )
{
}

bool SafetyInsert::process( bool value )
{
    bool returnValue = false;
    if( a_lastValue == false && value == true )
    {
        a_timeActivated = ws_milliseconds();
    }
    if( value == true )
    {
        if( ws_milliseconds() >= ( a_timeActivated + a_timeRequired ) )
        {
            returnValue = true;
        }
    }
    a_lastValue = value;
    return returnValue;
}

std::string SafetyInsert::getName( void )
{
    return std::string( "safety" );
}

