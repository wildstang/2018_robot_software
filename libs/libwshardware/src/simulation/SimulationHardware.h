#ifndef __SIMULATIONHARDWARE_H__
#define __SIMULATIONHARDWARE_H__

#include "core/HardwareHandle.h"

#include "SimulationSmartDashboard.h"
#include "SimulationIOFactory.h"

#include <map>

class SimulationHardware : public HardwareHandle
{
    public:
        SimulationHardware( void );
        virtual ~SimulationHardware( void );

        virtual int initialize( void );

        virtual double getFPGATimestamp( void );
        virtual double getVoltage( void );
        virtual double getTemperature( void );
        virtual double getCurrent( int channel );
        virtual double getTotalCurrent( void );
        virtual double getTotalPower( void );
        virtual double getTotalEnergy( void );

        virtual std::string getGameData( void );

        virtual int addAutoProgram( AutoProgram* p_program );
        virtual AutoProgram* getSelectedAutoProgram( void );
        virtual bool isAutoProgramLockedIn( void );
        virtual WsSmartDashboard* getSmartDashboard( void );
        virtual InputOutputFactory* getIOFactory( void );

    protected:
        SimulationSmartDashboard a_dashboard;
        SimulationIOFactory a_ioFactory;
        std::map< std::string, AutoProgram* > a_programs;
};

#endif

