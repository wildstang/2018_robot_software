#include "WsCompassInput.h"

WsCompassInput::WsCompassInput( std::string name, frc::I2C::Port port, int address )
    : WsI2CInput( name, port, address )
    , PIDSource()
{
}

WsCompassInput::WsCompassInput( std::string name )
    : WsI2CInput( name )
    , PIDSource()
{
}

WsCompassInput::~WsCompassInput( void )
{
}

void WsCompassInput::SetPIDSourceType( frc::PIDSourceType pidSource )
{
    // Ignore - can't change type
}

frc::PIDSourceType WsCompassInput::GetPIDSourceType( void ) const
{
    return frc::PIDSourceType::kDisplacement;
}

double WsCompassInput::PIDGet( void )
{
    unsigned char x = 0;

    // Expecting the compass to return a value between 0-180.  need to multiply by 2 for the angle
    // in range 0-360

    std::vector< unsigned char > v = this->getValue();
    if( v.size() == 0 )
    {
        WS_ERROR( "no data to get from binary input." );
    }
    else
    {
        x = v[ 0 ];
    }

    return ( ( double )x * 2.0 );
}

