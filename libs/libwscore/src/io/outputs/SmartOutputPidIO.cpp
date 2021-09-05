#include "SmartOutputPidIO.h"

SmartOutputPidIO::SmartOutputPidIO( void )
    : PidInput()
    , PidOutput()
    , a_output( 0.0 )
    , a_input( 0.0 )
{
}

SmartOutputPidIO::~SmartOutputPidIO( void )
{
}

double SmartOutputPidIO::pidRead( void )
{
    return a_input;
}

void SmartOutputPidIO::pidWrite( double output )
{
    a_output = output;
}

void SmartOutputPidIO::setInput( double value )
{
    a_input = value;
}

double SmartOutputPidIO::getOutput( void )
{
    return a_output;    
}

