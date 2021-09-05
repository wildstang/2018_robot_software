#ifndef __OUTPUTSOURCE_H__
#define __OUTPUTSOURCE_H__

#include "io/OutputManager.h"

class Output;

#include "io/OutputManager.h"

#define CONNECT_OUTPUT( name, variable, type ) \
        { \
            Output* __output = \
                RobotCore::getRobotCore()->getOutputManager()->getOutput( name ); \
            WS_INFO( "connecting output %s to %s.%s", \
                     name, this->getName().c_str(), #variable ); \
            if( __output != NULL ) \
            { \
                OutputSource* __source = \
                    new ReferenceOutputSource< type >( &variable ); \
                __output->setOutputSource( __source ); \
            } \
            else \
            { \
                WS_ERROR( "output %s doesn't exist.", name ); \
            } \
        }

class OutputSource
{
    public:
        OutputSource( void );
        virtual ~OutputSource( void );

        virtual void pushToOutput( Output* p_output ) = 0;
};

template< typename T >
class ReferenceOutputSource : public OutputSource
{
    public:
        ReferenceOutputSource( T* p_value );
        virtual ~ReferenceOutputSource( void );

        virtual void pushToOutput( Output* p_output );

    protected:
        T* ap_value;
};

#define __OUTPUTSOURCE_H__TEMPLATE__
#include "OutputSource.cpp"
#undef __OUTPUTSOURCE_H__TEMPLATE__

#endif

