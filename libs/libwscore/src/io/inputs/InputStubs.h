#ifndef __INPUTSTUBS_H__
#define __INPUTSTUBS_H__

#include "InputTemplate.h"

#include <vector>

class AnalogInput : public InputTemplate< double >
{
    public:
        AnalogInput( std::string name, double default_value );
        virtual ~AnalogInput( void );

    protected:
        virtual double readRawValue( void ) = 0;
};

class DigitalInput : public InputTemplate< bool >
{
    public:
        DigitalInput( std::string name, bool default_value );
        virtual ~DigitalInput( void );

    protected:
        virtual bool readRawValue( void ) = 0;
};

class DiscreteInput : public InputTemplate< int >
{
    public:
        DiscreteInput( std::string name, int default_value );
        virtual ~DiscreteInput( void );

    protected:
        virtual int readRawValue( void ) = 0;
};

class BinaryInput : public InputTemplate< std::vector< unsigned char > >
{
    public:
        BinaryInput( std::string name, std::vector< unsigned char > default_value );
        virtual ~BinaryInput( void );

    protected:
        virtual std::vector< unsigned char > readRawValue( void ) = 0;
};

#endif

