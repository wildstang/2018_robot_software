#ifndef __SIMULATIONIOFACTORY_H__
#define __SIMULATIONIOFACTORY_H__

#include "io/InputOutputFactory.h"

class SimulationIOFactory : public InputOutputFactory
{
    public:
        SimulationIOFactory( void );
        virtual ~SimulationIOFactory( void );

        virtual Input* createInput( std::string name, inputTypeT type );
        virtual Output* createOutput( std::string name, outputTypeT type );
};

#endif

