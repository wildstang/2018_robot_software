#ifndef __NULLOUTPUTS_H__
#define __NULLOUTPUTS_H__

#include "OutputStubs.h"

class NullAnalogOutput : public AnalogOutput
{
    public:
        NullAnalogOutput( std::string name );
        NullAnalogOutput( std::string name, double default_value );
        virtual ~NullAnalogOutput( void );

    protected:
        virtual void sendDataToOutput( void );

        virtual int instantiate( void );
};

class NullDigitalOutput : public DigitalOutput
{
    public:
        NullDigitalOutput( std::string name );
        NullDigitalOutput( std::string name, bool default_value );
        virtual ~NullDigitalOutput( void );

    protected:
        virtual void sendDataToOutput( void );

        virtual int instantiate( void );
};

class NullDiscreteOutput : public DiscreteOutput
{
    public:
        NullDiscreteOutput( std::string name );
        NullDiscreteOutput( std::string name, int default_value );
        virtual ~NullDiscreteOutput( void );

    protected:
        virtual void sendDataToOutput( void );

        virtual int instantiate( void );
};

class NullBinaryOutput : public BinaryOutput
{
    public:
        NullBinaryOutput( std::string name );
        NullBinaryOutput( std::string name, std::vector< unsigned char > default_value );
        virtual ~NullBinaryOutput( void );

    protected:
        virtual void sendDataToOutput( void );

        virtual int instantiate( void );

    private:
        static std::vector< unsigned char > s_empty;
};

class NullSmartOutput : public SmartOutput
{
    public:
        NullSmartOutput( std::string name );
        NullSmartOutput( std::string name, smartOutputCmdT default_value );
        virtual ~NullSmartOutput( void );

    protected:
        virtual void sendDataToOutput( void );

        virtual int instantiate( void );

    private:
        static const smartOutputCmdT sc_empty;
};

#endif

