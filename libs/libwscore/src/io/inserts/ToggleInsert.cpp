#include "ToggleInsert.h"

ToggleInsert::ToggleInsert( void )
    : WsIOInsert()
    , a_prev( false )
    , a_state( false )
{
}

ToggleInsert::~ToggleInsert( void )
{
}

bool ToggleInsert::process( bool value )
{
    if( value && !a_prev )
    {
        a_state = !a_state;
    }

    a_prev = value;

    return a_state;
}

std::string ToggleInsert::getName( void )
{
    return std::string( "toggle" );
}

