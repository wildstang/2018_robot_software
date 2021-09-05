#ifndef __OUTPUTSTUBS_H__
#define __OUTPUTSTUBS_H__

#include "OutputTemplate.h"

class AnalogOutput : public OutputTemplate< double >
{
    public:
        AnalogOutput( std::string name, double default_value );
        virtual ~AnalogOutput( void );

    protected:
        virtual void sendDataToOutput( void ) = 0;
};

class SmartOutput : public OutputTemplate< smartOutputCmdT >
{
    public:
        SmartOutput( std::string name, smartOutputCmdT default_value );
        virtual ~SmartOutput( void );

    protected:
        virtual void sendDataToOutput( void ) = 0;
};

class DigitalOutput : public OutputTemplate< bool >
{
    public:
        DigitalOutput( std::string name, bool default_value );
        virtual ~DigitalOutput( void );

    protected:
        virtual void sendDataToOutput( void ) = 0;
};

class DiscreteOutput : public OutputTemplate< int >
{
    public:
        DiscreteOutput( std::string name, int default_value );
        virtual ~DiscreteOutput( void );

    protected:
        virtual void sendDataToOutput( void ) = 0;
};

class BinaryOutput : public OutputTemplate< std::vector< unsigned char > >
{
    public:
        BinaryOutput( std::string name, std::vector< unsigned char > default_value );
        virtual ~BinaryOutput( void );

    protected:
        virtual void sendDataToOutput( void ) = 0;
};

#endif

