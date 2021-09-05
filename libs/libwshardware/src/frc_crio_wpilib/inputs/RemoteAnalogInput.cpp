#include "RemoteAnalogInput.h"

#include "networktables/NetworkTableInstance.h"

#include "config/ConfigValueRef.h"

RemoteAnalogInput::RemoteAnalogInput( std::string name, std::string network_table )
    : AnalogInput( name, 0.0 )
    , a_table()
    , a_tableName( network_table )
{
}

RemoteAnalogInput::RemoteAnalogInput( std::string name )
    : AnalogInput( name, 0.0 )
    , a_table()
    , a_tableName()
{
    CONFIG_VAR( "network_table", a_tableName, std::string );
}

RemoteAnalogInput::~RemoteAnalogInput( void )
{
}

int RemoteAnalogInput::instantiate( void )
{
    a_table = nt::NetworkTableInstance::GetDefault().GetTable( a_tableName );
    return 0;
}

double RemoteAnalogInput::readRawValue( void )
{
    return a_table->GetNumber( this->getName(), 0 );
}

