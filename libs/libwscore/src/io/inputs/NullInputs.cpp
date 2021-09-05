#include "NullInputs.h"

#include "logger/WsLogger.h"

std::vector< unsigned char > NullBinaryInput::s_empty;

NullAnalogInput::NullAnalogInput( std::string name )
    : AnalogInput( name, 0.0 )
    , a_slaveValue( 0.0 )
{
    this->setEmissionEnable( false );
}

NullAnalogInput::NullAnalogInput( std::string name, double default_value )
    : AnalogInput( name, default_value )
    , a_slaveValue( default_value )
{
    this->setEmissionEnable( false );
}

NullAnalogInput::~NullAnalogInput( void )
{
}

void NullAnalogInput::setSlaveValue( double value )
{
    a_slaveValue = value;
}

int NullAnalogInput::instantiate( void )
{
    return 0;
}

double NullAnalogInput::readRawValue( void )
{
    return a_slaveValue;
}

NullDigitalInput::NullDigitalInput( std::string name )
    : DigitalInput( name, false )
    , a_slaveValue( false )
{
    this->setEmissionEnable( false );
}

NullDigitalInput::NullDigitalInput( std::string name, bool default_value )
    : DigitalInput( name, default_value )
    , a_slaveValue( default_value )
{
    this->setEmissionEnable( false );
}

NullDigitalInput::~NullDigitalInput( void )
{
}

void NullDigitalInput::setSlaveValue( bool value )
{
    a_slaveValue = value;
}

int NullDigitalInput::instantiate( void )
{
    return 0;
}

bool NullDigitalInput::readRawValue( void )
{
    return a_slaveValue;
}

NullDiscreteInput::NullDiscreteInput( std::string name )
    : DiscreteInput( name, 0 )
    , a_slaveValue( 0 )
{
    this->setEmissionEnable( false );
}

NullDiscreteInput::NullDiscreteInput( std::string name, int default_value )
    : DiscreteInput( name, default_value )
    , a_slaveValue( default_value )
{
    this->setEmissionEnable( false );
}

NullDiscreteInput::~NullDiscreteInput( void )
{
}

void NullDiscreteInput::setSlaveValue( int value )
{
    a_slaveValue = value;
}

int NullDiscreteInput::instantiate( void )
{
    return 0;
}

int NullDiscreteInput::readRawValue( void )
{
    return a_slaveValue;
}

NullBinaryInput::NullBinaryInput( std::string name )
    : BinaryInput( name, s_empty )
    , a_slaveValue( s_empty )
{
    this->setEmissionEnable( false );
}

NullBinaryInput::NullBinaryInput( std::string name, std::vector< unsigned char > default_value )
    : BinaryInput( name, default_value )
    , a_slaveValue( default_value )
{
    this->setEmissionEnable( false );
}

NullBinaryInput::~NullBinaryInput( void )
{
}

void NullBinaryInput::setSlaveValue( const std::vector< unsigned char >& rc_value )
{
    a_slaveValue = rc_value;
}

int NullBinaryInput::instantiate( void )
{
    return 0;
}

std::vector< unsigned char > NullBinaryInput::readRawValue( void )
{
    return a_slaveValue;
}

