#include "SimulationHardware.h"

#include "utils/ws_utils.h"
#include "auto/AutoProgram.h"
#include "logger/WsLogger.h"

#include "WsSimulation.h"

SimulationHardware::SimulationHardware( void )
    : HardwareHandle()
    , a_dashboard()
    , a_ioFactory()
    , a_programs()
{
}

SimulationHardware::~SimulationHardware( void )
{
    a_programs.clear();
}

int SimulationHardware::initialize( void )
{
    return 0;
}

double SimulationHardware::getFPGATimestamp( void )
{
    return ( ( double )ws_microseconds() / 1000000.0 );
}

int SimulationHardware::addAutoProgram( AutoProgram* p_program )
{
    if( p_program == NULL ) return 1;

    WS_INFO( "simulation: registered auto program %s",
             p_program->getProgramName().c_str() );
    a_programs[ p_program->getProgramName() ] = p_program;

    return 0;
}

AutoProgram* SimulationHardware::getSelectedAutoProgram( void )
{
    std::string name = WsSimulation::getInstance()->getAutoProgramName();
    std::map< std::string, AutoProgram* >::iterator i = a_programs.find( name );
    if( i != a_programs.end() )
    {
        return i->second;
    }
    else
    {
        WS_ERROR( "could not find the auto program specified '%s'",
                  name.c_str() );
        return NULL;
    }
}

std::string SimulationHardware::getGameData( void )
{
    return std::string( "RL" );
}

bool SimulationHardware::isAutoProgramLockedIn( void )
{
    return true;
}

WsSmartDashboard* SimulationHardware::getSmartDashboard( void )
{
    return &a_dashboard;
}

InputOutputFactory* SimulationHardware::getIOFactory( void )
{
    return &a_ioFactory;
}

double SimulationHardware::getVoltage( void )
{
    return 0.0;
}

double SimulationHardware::getTemperature( void )
{
    return 0.0;
}

double SimulationHardware::getCurrent( int channel )
{
    return 0.0;
}

double SimulationHardware::getTotalCurrent( void )
{
    return 0.0;
}

double SimulationHardware::getTotalPower( void )
{
    return 0.0;
}

double SimulationHardware::getTotalEnergy( void )
{
    return 0.0;
}

