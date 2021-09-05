#ifndef __INPUTTEMPLATE_H__
#define __INPUTTEMPLATE_H__

#include "io/Input.h"
#include "logger/Probe.h"

#include <string>

class InputListener;

bool __InputTemplate__compare( const std::vector< unsigned char >& rc_lhs,
                               const std::vector< unsigned char >& rc_rhs );
bool __InputTemplate__compare( const double& rc_lhs, const double& rc_rhs );
bool __InputTemplate__compare( const bool& rc_lhs, const bool& rc_rhs );
bool __InputTemplate__compare( const int& rc_lhs, const int& rc_rhs );

template< typename T >
class InputTemplate : public Input
{
    public:
        InputTemplate( std::string name, T default_value );
        virtual ~InputTemplate( void );

        T getValue( void );

    protected: 
        virtual T readRawValue( void ) = 0;

        virtual void readDataFromInput( void ) override;
        virtual void notifyListener( InputListener* p_listener ) override;

    private:
        T processInserts( T value );

        T a_currentValue;

        ProbeValue< T > a_valueProbe;
        ProbeValue< T > a_beforeInsertProbe;
};

#define __INPUTTEMPLATE_H__TEMPLATE__
#include "io/inputs/InputTemplate.cpp"
#undef __INPUTTEMPLATE_H__TEMPLATE__

#endif

