#include "InvertInsert.h"

InvertInsert::InvertInsert( void )
    : WsIOInsert()
{
}

InvertInsert::~InvertInsert( void )
{
}

double InvertInsert::process( double value )
{
    return value * -1.0;
}

int InvertInsert::process( int value )
{
    return value * -1;
}

bool InvertInsert::process( bool value )
{
    return !value;
}

std::string InvertInsert::getName( void )
{
    return std::string( "invert" );
}

