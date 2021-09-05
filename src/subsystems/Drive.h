#ifndef __DRIVE_H__
#define __DRIVE_H__

#include "subsystems/Subsystem.h"
#include "subsystems/drive/WsDrive.h"
#include "subsystems/drive/CheesyDriveHelper.h"
#include "io/Output.h"
#include "pid/PidController.h"

#include <list>

class Input;
class TurnPidIO;
class DrivePidIO;
class DriveHeadingPidIO;
class ArcDrivePidIO;
class ArcAnglePidIO;

class Drive : public Subsystem
{
    public:
        Drive( void );
        virtual ~Drive( void );

        // subsystem functions
        virtual void init( void );
        virtual void resetState( void );
        virtual void selfTest( void );
        virtual void update( void );

        void setBrakeMode( bool brakeOn );

        void setDriveModeCheesy( void );
        void setDriveModeRaw( void );
        void setDriveModeFullBrake( void );
        void setDriveModeDriveStraight( double speed, double distance );
        void setDriveModeTurn( double angle, double speed );
        void setDriveModeArc( double radius, double angle, double speed );

        int driveInputSetHeading( double newHeading );
        int driveInputSetThrottle( double newThrottle );
        int driveInputSetQuickTurn( bool quickTurn );
        int driveInputSetAntiTurbo( bool antiTurbo );
        int driveInputLeftEncoderValue( double value );
        int driveInputRightEncoderValue( double value );
        int driveInputGyroValue( double value );

        inline double getLeftEncoderValue( void ) { return a_leftEncoderValue; }
        inline double getRightEncoderValue( void ) { return a_rightEncoderValue; }
        double getDrivePidEncoderValue( void );

        //
        // Pid IO support functions
        //
        int setDrivePidHeadingCorrection( double value );
        int setTurnPidHeadingCorrection( double value );
        int setPidDriveThrottle( double value );
        int setArcPidHeading( double heading );
        int setArcPidThrottle( double throttle );

        inline bool drivePidStabilized( void )
            { return a_autoDrivePid.isStabilized(); }
        inline bool driveHeadingPidStabilized( void )
            { return a_autoDriveHeadingPid.isStabilized(); }
        inline bool turnPidStabilized( void )
            { return a_autoTurnPid.isStabilized(); }
        inline bool arcHeadingPidStabilized( void )
            { return a_autoArcAnglePid.isStabilized(); }
        inline bool arcDrivePidStabilized( void )
            { return a_autoArcDrivePid.isStabilized(); }

        inline bool drivePidOnTarget( void )
            { return a_autoDrivePid.isOnTarget(); }
        inline bool driveHeadingPidOnTarget( void )
            { return a_autoDriveHeadingPid.isOnTarget(); }
        inline bool turnPidOnTarget( void )
            { return a_autoTurnPid.isOnTarget(); }
        inline bool arcHeadingPidOnTarget( void )
            { return a_autoArcAnglePid.isOnTarget(); }
        inline bool arcDrivePidOnTarget( void )
            { return a_autoArcDrivePid.isOnTarget(); }

        inline bool drivePidStalled( void )
            { return a_autoDrivePid.isStalled(); }
        inline bool driveHeadingPidStalled( void )
            { return a_autoDriveHeadingPid.isStalled(); }
        inline bool turnPidStalled( void )
            { return a_autoTurnPid.isStalled(); }
        inline bool arcHeadingPidStalled( void )
            { return a_autoArcAnglePid.isStalled(); }
        inline bool arcDrivePidStalled( void )
            { return a_autoArcDrivePid.isStalled(); }

        int enableAutoPidLog( void );
        int disableAutoPidLog( void );

    protected:
        void calculateTurnPidCorrections( wsDriveSignalT& r_signal,
                                          double correction,
                                          double dynamic_range = 1.0 );

        double powerate( double x, double gain );

        double getArcEncoderValue( void );

    private:
        void setMotorSpeeds( wsDriveSignalT signal );

        CheesyDriveHelper a_cheesyHelper;

        //
        // Config
        //
        double a_throttleScale;
        double a_headingScale;
        bool a_powerateHeading;
        bool a_powerateThrottle;
        double a_headingGain;
        double a_throttleGain;

        // PIDs
        DrivePidIO* ap_autoDrivePidIO;
        DriveHeadingPidIO* ap_autoDriveHeadingPidIO;
        TurnPidIO* ap_autoTurnPidIO;
        ArcDrivePidIO* ap_autoArcDrivePidIO;
        ArcAnglePidIO* ap_autoArcAnglePidIO;
        PidController a_autoDrivePid;
        PidController a_autoDriveHeadingPid;
        PidController a_autoTurnPid;
        PidController a_autoArcAnglePid;
        PidController a_autoArcDrivePid;

        double a_drivePidThrottle;
        double a_drivePidHeading;
        double a_turnPidHeading;
        double a_drivePidSpeedCoeff;
        double a_turnPidSpeedCoeff;
        double a_headingSetPoint;

        double a_arcPidThrottle;
        double a_arcPidHeading;
        double a_arcPidSpeedCoeff;
        double a_arcAngle;
        double a_arcRadius;

        // Values from inputs
        double a_throttleValue;
        double a_headingValue;
        bool a_quickTurn;
        bool a_antiTurbo;

        double a_leftEncoderValue;
        double a_rightEncoderValue;
        double a_gyroValue;

        smartOutputCmdT a_leftMaster;
        smartOutputCmdT a_rightMaster;
        smartOutputCmdT a_leftFollowerOne;
        smartOutputCmdT a_rightFollowerOne;
        smartOutputCmdT a_leftFollowerTwo;
        smartOutputCmdT a_rightFollowerTwo;

        wsDriveTypeT a_driveMode;
        wsDriveSignalT a_driveSignal;

        ProbeValue< double > a_turnCorrectionProbe;
        ProbeValue< double > a_driveHeadingPidProbe;
        ProbeValue< double > a_drivePidProbe;
        ProbeValue< double > a_leftMotorProbe;
        ProbeValue< double > a_rightMotorProbe;
        ProbeValue< double > a_remappedThrottleProbe;
        ProbeValue< double > a_remappedHeadingProbe;
        ProbeValue< double > a_headingSetPointProbe;

        // Constants
        static const double sc_robotWidthInches;
        static const double sc_wheelDiameterInches;
        static const double sc_encoderCpr;
        static const double sc_ticksToInches;
        static const double sc_radians;
        static const double sc_antiTurboFactor;
};

#endif
