#include "RemoteDigitalOutput.h"

#include "networktables/NetworkTableInstance.h"

#include "config/ConfigValueRef.h"

RemoteDigitalOutput::RemoteDigitalOutput( std::string name, std::string table, bool default_value )
    : DigitalOutput( name, default_value )
    , a_table()
    , a_tableName( table )
{
}

RemoteDigitalOutput::RemoteDigitalOutput( std::string name )
    : DigitalOutput( name, false )
    , a_table()
    , a_tableName()
{
    CONFIG_VAR( "network_table", a_tableName, std::string );
}

RemoteDigitalOutput::~RemoteDigitalOutput( void )
{
}

int RemoteDigitalOutput::instantiate( void )
{
    a_table = nt::NetworkTableInstance::GetDefault().GetTable( a_tableName );
    return 0;
}

void RemoteDigitalOutput::sendDataToOutput( void )
{
    a_table->PutBoolean( getName(), getValue() );
}

