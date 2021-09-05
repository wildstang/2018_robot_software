#ifndef __NULLINPUTS_H__
#define __NULLINPUTS_H__

#include "io/inputs/InputStubs.h"

class NullAnalogInput : public AnalogInput
{
    public:
        NullAnalogInput( std::string name );
        NullAnalogInput( std::string name, double default_value );
        virtual ~NullAnalogInput( void );

        void setSlaveValue( double value );

    protected:
        virtual double readRawValue( void );

        virtual int instantiate( void );

    private:
        double a_slaveValue;
};

class NullDigitalInput : public DigitalInput
{
    public:
        NullDigitalInput( std::string name );
        NullDigitalInput( std::string name, bool default_value );
        virtual ~NullDigitalInput( void );

        void setSlaveValue( bool value );

    protected:
        virtual bool readRawValue( void );

        virtual int instantiate( void );

    private:
        bool a_slaveValue;
};

class NullDiscreteInput : public DiscreteInput
{
    public:
        NullDiscreteInput( std::string name );
        NullDiscreteInput( std::string name, int default_value );
        virtual ~NullDiscreteInput( void );

        void setSlaveValue( int value );

    protected:
        virtual int readRawValue( void );

        virtual int instantiate( void );

    private:
        int a_slaveValue;
};

class NullBinaryInput : public BinaryInput
{
    public:
        NullBinaryInput( std::string name );
        NullBinaryInput( std::string name, std::vector< unsigned char > default_value );
        virtual ~NullBinaryInput( void );

        void setSlaveValue( const std::vector< unsigned char >& rc_value );

    protected:
        virtual std::vector< unsigned char > readRawValue( void );

        virtual int instantiate( void );

    private:
        static std::vector< unsigned char > s_empty;
        std::vector< unsigned char > a_slaveValue;
};

#endif

