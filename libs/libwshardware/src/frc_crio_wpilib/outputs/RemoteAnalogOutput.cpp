#include "RemoteAnalogOutput.h"

#include "networktables/NetworkTableInstance.h"

#include "config/ConfigValueRef.h"

RemoteAnalogOutput::RemoteAnalogOutput( std::string name, std::string table, double default_value )
    : AnalogOutput( name, default_value )
    , a_table()
    , a_tableName( table )
{
}

RemoteAnalogOutput::RemoteAnalogOutput( std::string name )
    : AnalogOutput( name, 0.0 )
    , a_table()
    , a_tableName()
{
    CONFIG_VAR( "network_table", a_tableName, std::string );
}

RemoteAnalogOutput::~RemoteAnalogOutput( void )
{
}

int RemoteAnalogOutput::instantiate( void )
{
    a_table = nt::NetworkTableInstance::GetDefault().GetTable( a_tableName );
    return 0;
}

void RemoteAnalogOutput::sendDataToOutput( void )
{
    a_table->PutNumber( this->getName(), this->getValue() );
}

