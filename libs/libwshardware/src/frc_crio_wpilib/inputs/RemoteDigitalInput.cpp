#include "RemoteDigitalInput.h"

#include "networktables/NetworkTableInstance.h"

#include "config/ConfigValueRef.h"

RemoteDigitalInput::RemoteDigitalInput( std::string name, std::string network_table )
    : DigitalInput( name, false )
    , a_table()
    , a_tableName( network_table )
{
}

RemoteDigitalInput::RemoteDigitalInput( std::string name )
    : DigitalInput( name, false )
    , a_table()
    , a_tableName()
{
    CONFIG_VAR( "network_table", a_tableName, std::string );
}

RemoteDigitalInput::~RemoteDigitalInput( void )
{
}

int RemoteDigitalInput::instantiate( void )
{
    a_table = nt::NetworkTableInstance::GetDefault().GetTable( a_tableName );
    return 0;
}

bool RemoteDigitalInput::readRawValue( void )
{
    return a_table->GetBoolean( this->getName(), false );
}

