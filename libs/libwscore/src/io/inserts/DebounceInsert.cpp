#include "DebounceInsert.h"

DebounceInsert::DebounceInsert( void )
    : WsIOInsert()
    , a_currentValue( false )
    , a_cyclesOnCurrentValue( 0 )
    , a_lastValue( false )
    , a_debounceCycles( 5 )
{
}

DebounceInsert::DebounceInsert( int debounce_cycles )
    : WsIOInsert()
    , a_currentValue( false )
    , a_cyclesOnCurrentValue( 0 )
    , a_lastValue( false )
    , a_debounceCycles( debounce_cycles )
{
}

DebounceInsert::~DebounceInsert( void )
{
}

bool DebounceInsert::process( bool value )
{
    if( value != a_lastValue )
    {
        // The value has changed - reset the counter
        a_cyclesOnCurrentValue = 0;
        a_lastValue = value;
    }
    else
    {
        // Otherwise, the value has held for longer - increment counter
        a_cyclesOnCurrentValue++;
    }

    // If the value has held long enough, set the new value
    if( a_cyclesOnCurrentValue >= a_debounceCycles )
    {
        a_currentValue = value;
    }

    return a_currentValue;
}

std::string DebounceInsert::getName( void )
{
    return std::string( "debounce" );
}

