#ifndef __PROBE_H__
#define __PROBE_H__

#include <string>

class Probe
{
    public:
        Probe( std::string name );
        virtual ~Probe( void );

        virtual std::string get( void ) = 0;

        inline std::string getName( void ) { return ac_name; }

    protected:
        const std::string ac_name;
};

template< typename T >
class ProbeValue : public Probe
{
    public:
        ProbeValue( std::string name, T default_value );
        virtual ~ProbeValue( void );

        virtual std::string get( void );

        void update( T value );

    protected:
        T a_value;
};

#define __PROBE_H__TEMPLATE__
#include "Probe.cpp"
#undef __PROBE_H__TEMPLATE__

#endif

