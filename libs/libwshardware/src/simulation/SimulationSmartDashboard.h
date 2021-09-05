#ifndef __SIMULATIONSMARTDASHBOARD_H__
#define __SIMULATIONSMARTDASHBOARD_H__

#include "logger/WsSmartDashboard.h"

class SimulationSmartDashboard : public WsSmartDashboard
{
    public:
        SimulationSmartDashboard( void );
        virtual ~SimulationSmartDashboard( void );

        virtual int put( std::string key, int value );
        virtual int put( std::string key, double value );
        virtual int put( std::string key, bool value );
        virtual int put( std::string key, std::string value );
};

#endif

