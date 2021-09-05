#include "WsLIDAR.h"

#include "LidarSensor.h"
#include "crio_utils.h"

#include "config/ConfigValueRef.h"

WsLIDAR::WsLIDAR( std::string name, enum frc::I2C::Port port, int address )
    : DiscreteInput( name, 0 )
    , ap_lidar( NULL )
    , a_port( port )
    , a_portStr()
    , ac_factoryConstructor( false )
    , a_address( address )
{
}

WsLIDAR::WsLIDAR( std::string name )
    : DiscreteInput( name, 0 )
    , ap_lidar( NULL )
    , a_port( frc::I2C::kOnboard )
    , a_portStr()
    , ac_factoryConstructor( true )
    , a_address( 0 )
{
    CONFIG_VAR( "port", a_portStr, std::string );
    CONFIG_VAR( "address", a_address, int );
}

WsLIDAR::~WsLIDAR( void )
{
    if( ap_lidar != NULL )
    {
        delete ap_lidar;
        ap_lidar = NULL;
    }
}

int WsLIDAR::instantiate( void )
{
    if( ac_factoryConstructor )
    {
        a_port = parse_i2c_port( a_portStr );
    }

    if( ap_lidar != NULL ) delete ap_lidar;
    ap_lidar = new LidarSensor( a_port, a_address );
    ap_lidar->start();
    return 0;
}

int WsLIDAR::readRawValue( void )
{
    return ap_lidar->getSmoothedDistance();
}

