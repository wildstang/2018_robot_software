#include "NullOutputs.h"

#include "logger/WsLogger.h"

std::vector< unsigned char > NullBinaryOutput::s_empty;

NullAnalogOutput::NullAnalogOutput( std::string name )
    : AnalogOutput( name, 0.0 )
{
    this->setEmissionEnable( false );
}

NullAnalogOutput::NullAnalogOutput( std::string name, double default_value )
    : AnalogOutput( name, default_value )
{
    this->setEmissionEnable( false );
}

NullAnalogOutput::~NullAnalogOutput( void )
{
}

void NullAnalogOutput::sendDataToOutput( void )
{
}

int NullAnalogOutput::instantiate( void )
{
    return 0;
}

NullDigitalOutput::NullDigitalOutput( std::string name )
    : DigitalOutput( name, false )
{
    this->setEmissionEnable( false );
}

NullDigitalOutput::NullDigitalOutput( std::string name, bool default_value )
    : DigitalOutput( name, default_value )
{
    this->setEmissionEnable( false );
}

NullDigitalOutput::~NullDigitalOutput( void )
{
}

void NullDigitalOutput::sendDataToOutput( void )
{
}

int NullDigitalOutput::instantiate( void )
{
    return 0;
}

NullDiscreteOutput::NullDiscreteOutput( std::string name )
    : DiscreteOutput( name, 0 )
{
    this->setEmissionEnable( false );
}

NullDiscreteOutput::NullDiscreteOutput( std::string name, int default_value )
    : DiscreteOutput( name, default_value )
{
    this->setEmissionEnable( false );
}

NullDiscreteOutput::~NullDiscreteOutput( void )
{
}

void NullDiscreteOutput::sendDataToOutput( void )
{
}

int NullDiscreteOutput::instantiate( void )
{
    return 0;
}

NullBinaryOutput::NullBinaryOutput( std::string name )
    : BinaryOutput( name, s_empty )
{
    this->setEmissionEnable( false );
}

NullBinaryOutput::NullBinaryOutput( std::string name, std::vector< unsigned char > default_value )
    : BinaryOutput( name, default_value )
{
    this->setEmissionEnable( false );
}

NullBinaryOutput::~NullBinaryOutput( void )
{
}

void NullBinaryOutput::sendDataToOutput( void )
{
}

int NullBinaryOutput::instantiate( void )
{
    return 0;
}

const smartOutputCmdT NullSmartOutput::sc_empty =
    { SMART_OUTPUT_CONTROL_MODE_ID_NONE, 0.0, 0, 0.0, 0.0, false, false };

NullSmartOutput::NullSmartOutput( std::string name )
    : SmartOutput( name, sc_empty )
{
    this->setEmissionEnable( false );
}

NullSmartOutput::NullSmartOutput( std::string name, smartOutputCmdT default_value )
    : SmartOutput( name, default_value )
{
    this->setEmissionEnable( false );
}

NullSmartOutput::~NullSmartOutput( void )
{
}

void NullSmartOutput::sendDataToOutput( void )
{
}

int NullSmartOutput::instantiate( void )
{
    return 0;
}

