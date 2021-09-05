#include "SleeperProgram.h"

#include "auto/steps/control/AutoStepStopAutonomous.h"

SleeperProgram::SleeperProgram( void )
    : AutoProgram( std::string( "sleeper" ) )
{
}

SleeperProgram::~SleeperProgram( void )
{
}

void SleeperProgram::defineSteps( void )
{
    this->addStep( new AutoStepStopAutonomous() );
}

