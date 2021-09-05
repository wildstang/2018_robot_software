#include "WsSimulatedInput.h"

#include "WsSimulation.h"

#ifndef __WSSIMULATEDINPUT_H__TEMPLATE__

WsSimulatedInput::WsSimulatedInput( std::string name )
    : a_simulatedName( name )
{
    WsSimulation* p_sim = WsSimulation::getInstance();
    p_sim->addSimulatedInput( this );
}

WsSimulatedInput::~WsSimulatedInput( void )
{
}

int WsSimulatedInput::setSimulatedValueFromString( std::string value )
{
    int err = 0;

    err = setSimulatedValue( value );
    if( !err ) return 0;

    err = setSimulatedValue( atoi( value.c_str() ) );
    if( !err ) return 0;

    err = setSimulatedValue( atof( value.c_str() ) );
    if( !err ) return 0;

    bool bool_parse_value = false;
    err = parse( value, &bool_parse_value );
    if( !err )
    {
        err = setSimulatedValue( bool_parse_value );
    }
    if( !err ) return 0;

    std::vector< unsigned char > v;
    parse( value, &v );
    err = setSimulatedValue( v );
    if( !err ) return 0;

    return 1;
}

int WsSimulatedInput::setSimulatedValue( std::string value )
{
    return 1;
}

int WsSimulatedInput::setSimulatedValue( int value )
{
    return 1;
}

int WsSimulatedInput::setSimulatedValue( double value )
{
    return 1;
}

int WsSimulatedInput::setSimulatedValue( bool value )
{
    return 1;
}

int WsSimulatedInput::setSimulatedValue( std::vector< unsigned char > value )
{
    return 1;
}

#else

template< typename T >
WsSimulatedInputTemplate< T >::WsSimulatedInputTemplate( std::string name, T default_value )
    : InputTemplate< T >( name, default_value )
    , WsSimulatedInput( name )
    , a_simulatedValue( default_value )
{
}

template< typename T >
WsSimulatedInputTemplate< T >::~WsSimulatedInputTemplate( void )
{
}

template< typename T >
int WsSimulatedInputTemplate< T >::setSimulatedValue( T value )
{
    a_simulatedValue = value;
    return 0;
}

template< typename T >
T WsSimulatedInputTemplate< T >::readRawValue( void )
{
    return a_simulatedValue;
}

template< typename T >
int WsSimulatedInputTemplate< T >::instantiate( void )
{
    return 0;
}

#endif

