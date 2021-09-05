#include "CrioHardware.h"

#include "Timer.h"
#include "auto/AutoProgram.h"

#include "PowerDistributionPanel.h"
#include "Compressor.h"
#include "DriverStation.h"

#include "CameraServer.h"

CrioHardware::CrioHardware( void )
    : HardwareHandle()
    , a_chooser()
    , a_lockinChooser()
    , a_ioFactory()
    , ap_pdp( NULL )
{
    a_lockinChooser.AddDefault( "Unlocked", false );
    a_lockinChooser.AddObject( "Locked", true );

    ap_pdp = new frc::PowerDistributionPanel();
    ap_compressor = new frc::Compressor( 16 );
}

CrioHardware::~CrioHardware( void )
{
    delete ap_pdp;
    ap_pdp = NULL;

    delete ap_compressor;
    ap_compressor = NULL;
}

int CrioHardware::initialize( void )
{
    frc::SmartDashboard::PutData( "Auto Selector", &a_chooser );
    frc::SmartDashboard::PutData( "Lock-in Auto Program", &a_lockinChooser );
    frc::CameraServer::GetInstance()->StartAutomaticCapture();

    return 0;
}

double CrioHardware::getFPGATimestamp( void )
{
    return frc::Timer::GetFPGATimestamp();
}

int CrioHardware::addAutoProgram( AutoProgram* p_program )
{
    if( p_program == NULL ) return 1;
    a_chooser.AddObject( p_program->getProgramName(), p_program );
    return 0;
}

AutoProgram* CrioHardware::getSelectedAutoProgram( void )
{
    return ( AutoProgram* )a_chooser.GetSelected();
}

bool CrioHardware::isAutoProgramLockedIn( void )
{
    return a_lockinChooser.GetSelected();
}

WsSmartDashboard* CrioHardware::getSmartDashboard( void )
{
    return &a_dashboard;
}

InputOutputFactory* CrioHardware::getIOFactory( void )
{
    return &a_ioFactory;
}

double CrioHardware::getVoltage( void )
{
    //return ap_pdp->GetVoltage();
    return 0.0;
}

double CrioHardware::getTemperature( void )
{
    //return ap_pdp->GetTemperature();
    return 0.0;
}

double CrioHardware::getCurrent( int channel )
{
    return ap_pdp->GetCurrent( channel );
    return 0.0;
}

double CrioHardware::getTotalCurrent( void )
{
    //return ap_pdp->GetTotalCurrent();
    return 0.0;
}

double CrioHardware::getTotalPower( void )
{
    //return ap_pdp->GetTotalPower();
    return 0.0;
}

double CrioHardware::getTotalEnergy( void )
{
    //return ap_pdp->GetTotalEnergy();
    return 0.0;
}

std::string CrioHardware::getGameData( void )
{
    return frc::DriverStation::GetInstance().GetGameSpecificMessage();;
}

allianceColorT CrioHardware::getAlliance( void )
{
    frc::DriverStation::Alliance color =
        frc::DriverStation::GetInstance().GetAlliance();

    if( color == frc::DriverStation::Alliance::kRed )
    {
        return ALLIANCE_COLOR_RED;
    }
    if( color == frc::DriverStation::Alliance::kBlue )
    {
        return ALLIANCE_COLOR_BLUE;
    }

    return ALLIANCE_COLOR_NONE;
}

void CrioHardware::clearPdpStickyFaults( void )
{
    ap_pdp->ClearStickyFaults();
}

void CrioHardware::clearPcmStickyFaults( void )
{
    ap_compressor->ClearAllPCMStickyFaults();
}

