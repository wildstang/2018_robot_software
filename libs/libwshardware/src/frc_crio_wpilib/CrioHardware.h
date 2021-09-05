#ifndef __CRIOHARDWARE_H__
#define __CRIOHARDWARE_H__

#include "core/HardwareHandle.h"

#include "SmartDashboard/SendableChooser.h"
#include "SmartDashboard/SmartDashboard.h"

#include "WsCrioSmartDashboard.h"
#include "CrioIOFactory.h"

namespace frc
{
    class PowerDistributionPanel;
    class Compressor;
}

class CrioHardware : public HardwareHandle
{
    public:
        CrioHardware( void );
        virtual ~CrioHardware( void );

        virtual int initialize( void ) override;

        virtual double getFPGATimestamp( void ) override;
        virtual double getVoltage( void ) override;
        virtual double getTemperature( void ) override;
        virtual double getCurrent( int channel ) override;
        virtual double getTotalCurrent( void ) override;
        virtual double getTotalPower( void ) override;
        virtual double getTotalEnergy( void ) override;

        virtual void clearPdpStickyFaults( void ) override;
        virtual void clearPcmStickyFaults( void ) override;

        virtual std::string getGameData( void ) override;
        virtual allianceColorT getAlliance( void ) override;

        virtual int addAutoProgram( AutoProgram* p_program ) override;
        virtual AutoProgram* getSelectedAutoProgram( void ) override;
        virtual bool isAutoProgramLockedIn( void ) override;
        virtual WsSmartDashboard* getSmartDashboard( void ) override;
        virtual InputOutputFactory* getIOFactory( void ) override;

    protected:
        frc::SendableChooser< AutoProgram* > a_chooser;
        frc::SendableChooser< bool > a_lockinChooser;
        WsCrioSmartDashboard a_dashboard;
        CrioIOFactory a_ioFactory;
        frc::PowerDistributionPanel* ap_pdp;
        frc::Compressor* ap_compressor;
};

#endif
