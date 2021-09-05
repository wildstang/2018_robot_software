#include "WsIOInsert.h"

#include "logger/WsLogger.h"

WsIOInsert::WsIOInsert( void )
{
}

WsIOInsert::~WsIOInsert( void )
{
}

double WsIOInsert::process( double value )
{
    WS_WARNING( "Wrong insert data type, insert %s", this->getName().c_str() );
    return value;
}

int WsIOInsert::process( int value )
{
    WS_WARNING( "Wrong insert data type, insert %s", this->getName().c_str() );
    return value;
}

bool WsIOInsert::process( bool value )
{
    WS_WARNING( "Wrong insert data type, insert %s", this->getName().c_str() );
    return value;
}

std::vector< unsigned char > WsIOInsert::process( std::vector< unsigned char > value )
{
    WS_WARNING( "Wrong insert data type, insert %s", this->getName().c_str() );
    return value;
}

smartOutputCmdT WsIOInsert::process( smartOutputCmdT value )
{
    WS_WARNING( "Wrong insert data type, insert %s", this->getName().c_str() );
    return value;
}

std::string WsIOInsert::getName( void )
{
    WS_WARNING( "insert name not implemented." );
    return std::string( "blank" );
}

