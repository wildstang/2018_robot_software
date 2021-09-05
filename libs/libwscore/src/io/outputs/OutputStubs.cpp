#include "OutputStubs.h"

AnalogOutput::AnalogOutput( std::string name, double default_value )
    : OutputTemplate< double >( name, default_value )
{
}

AnalogOutput::~AnalogOutput( void )
{
}

SmartOutput::SmartOutput( std::string name, smartOutputCmdT default_value )
    : OutputTemplate< smartOutputCmdT >( name, default_value )
{
}

SmartOutput::~SmartOutput( void )
{
}

DigitalOutput::DigitalOutput( std::string name, bool default_value )
    : OutputTemplate< bool >( name, default_value )
{
}

DigitalOutput::~DigitalOutput( void )
{
}

DiscreteOutput::DiscreteOutput( std::string name, int default_value )
    : OutputTemplate< int >( name, default_value )
{
}

DiscreteOutput::~DiscreteOutput( void )
{
}

BinaryOutput::BinaryOutput( std::string name, std::vector< unsigned char > default_value )
    : OutputTemplate< std::vector< unsigned char > >( name, default_value )
{
}

BinaryOutput::~BinaryOutput( void )
{
}

