#include "InputStubs.h"

AnalogInput::AnalogInput( std::string name, double default_value )
    : InputTemplate< double >( name, default_value )
{
}

AnalogInput::~AnalogInput( void )
{
}

DigitalInput::DigitalInput( std::string name, bool default_value )
    : InputTemplate< bool >( name, default_value )
{
}

DigitalInput::~DigitalInput( void )
{
}

DiscreteInput::DiscreteInput( std::string name, int default_value )
    : InputTemplate< int >( name, default_value )
{
}

DiscreteInput::~DiscreteInput( void )
{
}

BinaryInput::BinaryInput( std::string name, std::vector< unsigned char > default_value )
    : InputTemplate< std::vector< unsigned char > >( name, default_value )
{
}

BinaryInput::~BinaryInput( void )
{
}

