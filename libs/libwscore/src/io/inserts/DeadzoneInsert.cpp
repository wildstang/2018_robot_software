#include "DeadzoneInsert.h"

DeadzoneInsert::DeadzoneInsert( void )
    : WsIOInsert()
    , a_threshold( 0.05 )
{
}

DeadzoneInsert::DeadzoneInsert( double thresh )
    : WsIOInsert()
    , a_threshold( thresh )
{
    if( a_threshold < 0.0 )
    {
        a_threshold *= -1.0;
    }
}

DeadzoneInsert::~DeadzoneInsert( void )
{
}

double DeadzoneInsert::process( double value )
{
    if( value < 0.0 )
    {
        if( value > ( -1.0 * a_threshold ) )
            value = 0.0;
    }
    else
    {
        if( value < a_threshold )
            value = 0.0;
    }

    return value;
}

std::string DeadzoneInsert::getName( void )
{
    return std::string( "deadzone" );
}

