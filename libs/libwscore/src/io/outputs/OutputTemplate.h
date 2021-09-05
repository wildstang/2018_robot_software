#ifndef __OUTPUTTEMPLATE_H__
#define __OUTPUTTEMPLATE_H__

#include "io/Output.h"
#include "logger/Probe.h"

template< typename T >
class OutputTemplate : public Output
{
    public:
        OutputTemplate( std::string name, T default_value );
        virtual ~OutputTemplate( void );

        T getValue( void );
        virtual void setValue( T value ) override;

    protected:
        virtual void sendDataToOutput( void ) = 0;

    private:
        T processInserts( T value );

        T a_value;
        T a_rawValue;

        ProbeValue< T > a_valueProbe;
        ProbeValue< T > a_beforeInsertProbe;
};

#define __OUTPUTTEMPLATE_H__TEMPLATE__
#include "OutputTemplate.cpp"
#undef __OUTPUTTEMPLATE_H__TEMPLATE__

#endif

