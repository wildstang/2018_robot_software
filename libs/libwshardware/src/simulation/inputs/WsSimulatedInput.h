#ifndef __WSSIMULATEDINPUT_H__
#define __WSSIMULATEDINPUT_H__

#include "io/inputs/InputTemplate.h"

class WsSimulatedInput
{
    public:
        WsSimulatedInput( std::string name );
        virtual ~WsSimulatedInput( void );

        int setSimulatedValueFromString( std::string value );

        virtual int setSimulatedValue( std::string value );
        virtual int setSimulatedValue( int value );
        virtual int setSimulatedValue( double value );
        virtual int setSimulatedValue( bool value );
        virtual int setSimulatedValue( std::vector< unsigned char > value );

        inline std::string getSimulatedName( void ) { return a_simulatedName; }

    protected:
        std::string a_simulatedName;
};

template< typename T >
class WsSimulatedInputTemplate : public InputTemplate< T >,
                                 public WsSimulatedInput
{
    public:
        WsSimulatedInputTemplate( std::string name, T default_value );
        virtual ~WsSimulatedInputTemplate( void );

        virtual int setSimulatedValue( T value ) override;

    protected:
        virtual T readRawValue( void ) override;

        virtual int instantiate( void ) override;

    private:
        T a_simulatedValue;
};

#define __WSSIMULATEDINPUT_H__TEMPLATE__
#include "WsSimulatedInput.cpp"
#undef __WSSIMULATEDINPUT_H__TEMPLATE__

#endif

