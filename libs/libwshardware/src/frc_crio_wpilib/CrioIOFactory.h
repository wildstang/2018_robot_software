#ifndef __CRIOIOFACTORY_H__
#define __CRIOIOFACTORY_H__

#include "io/InputOutputFactory.h"

class CrioIOFactory : public InputOutputFactory
{
    public:
        CrioIOFactory( void );
        virtual ~CrioIOFactory( void );

        virtual Input* createInput( std::string name, inputTypeT type ) override;
        virtual Output* createOutput( std::string name, outputTypeT type ) override;
};

#endif

