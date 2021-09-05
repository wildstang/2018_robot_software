#include "WsSimulatedOutput.h"

#include "WsSimulation.h"

#ifndef __WSSIMULATEDOUTPUT_H__TEMPLATE__

WsSimulatedOutput::WsSimulatedOutput( std::string name )
    : a_simulatedName( name )
    , a_simulatorEnabled( true )
{
    WsSimulation* p_sim = WsSimulation::getInstance();
    p_sim->addSimulatedOutput( this );
}

WsSimulatedOutput::~WsSimulatedOutput( void )
{
}

#else

template< typename T >
WsSimulatedOutputTemplate< T >::WsSimulatedOutputTemplate( std::string name, T default_value )
    : OutputTemplate< T >( name, default_value )
    , WsSimulatedOutput( name )
{
}

template< typename T >
WsSimulatedOutputTemplate< T >::~WsSimulatedOutputTemplate( void )
{
}

template< typename T >
void WsSimulatedOutputTemplate< T >::sendDataToOutput( void )
{
    std::string s = toString( this->getValue() );
    if( a_simulatorEnabled ) 
        printf( "@@ Output %s: %s\n", this->getName().c_str(), s.c_str() );
}

template< typename T >
int WsSimulatedOutputTemplate< T >::instantiate( void )
{
    return 0;
}

#endif

