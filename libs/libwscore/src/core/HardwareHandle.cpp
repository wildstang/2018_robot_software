#include "HardwareHandle.h"

#include <sys/time.h>
#include <cstdlib>

#include "logger/WsLogger.h"

HardwareHandle::HardwareHandle( void )
{
    srand( time( NULL ) );
}

HardwareHandle::~HardwareHandle( void )
{
}

int HardwareHandle::initialize( void )
{
    WS_WARNING( "%s: no initialize function implemeneted in hardware",
                __PRETTY_FUNCTION__ );
    return 0;
}

double HardwareHandle::getFPGATimestamp( void )
{
    WS_ERROR( "%s: not implemented for this platform", __PRETTY_FUNCTION__ );
    return 0.0;
}

int HardwareHandle::addAutoProgram( AutoProgram* p_program )
{
    WS_ERROR( "%s: not implemented for this platform", __PRETTY_FUNCTION__ );
    return 1;
}

AutoProgram* HardwareHandle::getSelectedAutoProgram( void )
{
    WS_ERROR( "%s: not implemented for this platform", __PRETTY_FUNCTION__ );
    return NULL;
}

bool HardwareHandle::isAutoProgramLockedIn( void )
{
    WS_ERROR( "%s: not implemented for this platform", __PRETTY_FUNCTION__ );
    return false;
}

double HardwareHandle::getVoltage( void )
{
    WS_ERROR( "%s: not implemented for this platform", __PRETTY_FUNCTION__ );
    return 0.0;
}

double HardwareHandle::getTemperature( void )
{
    WS_ERROR( "%s: not implemented for this platform", __PRETTY_FUNCTION__ );
    return 0.0;
}

double HardwareHandle::getCurrent( int channel )
{
    WS_ERROR( "%s: not implemented for this platform", __PRETTY_FUNCTION__ );
    return 0.0;
}

double HardwareHandle::getTotalCurrent( void )
{
    WS_ERROR( "%s: not implemented for this platform", __PRETTY_FUNCTION__ );
    return 0.0;
}

double HardwareHandle::getTotalPower( void )
{
    WS_ERROR( "%s: not implemented for this platform", __PRETTY_FUNCTION__ );
    return 0.0;
}

double HardwareHandle::getTotalEnergy( void )
{
    WS_ERROR( "%s: not implemented for this platform", __PRETTY_FUNCTION__ );
    return 0.0;
}

std::string HardwareHandle::getGameData( void )
{
    WS_ERROR( "%s not implemented for this platform", __PRETTY_FUNCTION__ );
    return std::string( "" );
}

allianceColorT HardwareHandle::getAlliance( void )
{
    WS_ERROR( "%s not implemented for this platform", __PRETTY_FUNCTION__ );
    return ALLIANCE_COLOR_NONE;
}

void HardwareHandle::clearPdpStickyFaults( void )
{
    WS_ERROR( "%s: not implemented for this platform", __PRETTY_FUNCTION__ );
}

void HardwareHandle::clearPcmStickyFaults( void )
{
    WS_ERROR( "%s: not implemented for this platform", __PRETTY_FUNCTION__ );
}

