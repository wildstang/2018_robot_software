#include "Drive.h"

#include <cstdio>
#include <cmath>

#include "io/Input.h"
#include "io/Output.h"
#include "io/InputManager.h"
#include "io/OutputManager.h"
#include "io/routing/InputSlot.h"
#include "io/routing/OutputSource.h"
#include "logger/WsLogger.h"
#include "logger/WsSmartDashboard.h"
#include "utils/ws_utils.h"
#include "subsystems/drive/TurnPidIO.h"
#include "subsystems/drive/DriveHeadingPidIO.h"
#include "subsystems/drive/ArcAnglePidIO.h"
#include "subsystems/drive/ArcDrivePidIO.h"
#include "config/ConfigValueRef.h"

#include "robot/WsRobot.h"

#include "subsystems/drive/DrivePidIO.h"
#include "subsystems/drive/TurnPidIO.h"

const double Drive::sc_robotWidthInches = 30.0;
const double Drive::sc_wheelDiameterInches = 4.0;
const double Drive::sc_encoderCpr = 4096.0;
const double Drive::sc_radians = PI / 180.0;
const double Drive::sc_antiTurboFactor = 0.5;

Drive::Drive( void )
    : Subsystem( std::string( "drive" ) )
    , a_cheesyHelper()
    , a_throttleScale( 1.0 )
    , a_headingScale( 1.0 )
    , a_powerateHeading( false )
    , a_powerateThrottle( false )
    , a_headingGain( 1.0 )
    , a_throttleGain( 1.0 )
    , ap_autoDrivePidIO( new DrivePidIO() )
    , ap_autoDriveHeadingPidIO( new DriveHeadingPidIO() )
    , ap_autoTurnPidIO( new TurnPidIO() )
    , ap_autoArcDrivePidIO( new ArcDrivePidIO() )
    , ap_autoArcAnglePidIO( new ArcAnglePidIO() )
    , a_autoDrivePid( ap_autoDrivePidIO, ap_autoDrivePidIO,
                      std::string( "drive_pid" ), this )
    , a_autoDriveHeadingPid( ap_autoDriveHeadingPidIO, ap_autoDriveHeadingPidIO,
                             std::string( "drive_heading_pid" ), this )
    , a_autoTurnPid( ap_autoTurnPidIO, ap_autoTurnPidIO,
                     std::string( "turn_pid" ), this )
    , a_autoArcAnglePid( ap_autoArcAnglePidIO, ap_autoArcAnglePidIO, 
                         std::string( "arc_angle_pid" ), this )
    , a_autoArcDrivePid( ap_autoArcDrivePidIO, ap_autoArcDrivePidIO, 
                         std::string( "arc_distance_pid" ), this )
    , a_drivePidThrottle( 0.0 )
    , a_drivePidHeading( 0.0 )
    , a_turnPidHeading( 0.0 )
    , a_drivePidSpeedCoeff( 0.0 )
    , a_turnPidSpeedCoeff( 0.0 )
    , a_headingSetPoint( 0.0 )
    , a_arcPidThrottle( 0.0 )
    , a_arcPidHeading( 0.0 )
    , a_arcPidSpeedCoeff( 0.0 )
    , a_arcAngle( 0.0 )
    , a_arcRadius( 0.0 )
    , a_throttleValue( 0.0 )
    , a_headingValue( 0.0 )
    , a_quickTurn( false )
    , a_antiTurbo( false )
    , a_leftEncoderValue( 0.0 )
    , a_rightEncoderValue( 0.0 )
    , a_gyroValue( 0.0 )
    , a_leftMaster( { SMART_OUTPUT_CONTROL_MODE_ID_NONE,
                      0.0, 0, 0.0, 0.0, false, false } )
    , a_rightMaster( { SMART_OUTPUT_CONTROL_MODE_ID_NONE,
                       0.0, 0, 0.0, 0.0, false, false } )
    , a_leftFollowerOne( { SMART_OUTPUT_CONTROL_MODE_ID_NONE,
                           0.0, 0, 0.0, 0.0, false, false } )
    , a_rightFollowerOne( { SMART_OUTPUT_CONTROL_MODE_ID_NONE,
                            0.0, 0, 0.0, 0.0, false, false } )
    , a_leftFollowerTwo( { SMART_OUTPUT_CONTROL_MODE_ID_NONE,
                           0.0, 0, 0.0, 0.0, false, false } )
    , a_rightFollowerTwo( { SMART_OUTPUT_CONTROL_MODE_ID_NONE,
                            0.0, 0, 0.0, 0.0, false, false } )
    , a_driveMode( WS_DRIVE_TYPE_CHEESY )
    , a_driveSignal( ds_neutralSignal )
    , a_turnCorrectionProbe( "turn_pid_correction", 0.0 )
    , a_driveHeadingPidProbe( "drive_heading_pid_correction", 0.0 )
    , a_drivePidProbe( "drive_pid_throttle", 0.0 )
    , a_leftMotorProbe( "left_motor", 0.0 )
    , a_rightMotorProbe( "right_motor", 0.0 )
    , a_remappedThrottleProbe( "remapped_throttle", 0.0 )
    , a_remappedHeadingProbe( "remapped_heading", 0.0 )
    , a_headingSetPointProbe( "heading_set_point", 0.0 )
{
    CONFIG_VAR( "throttle_scale", a_throttleScale, double );
    CONFIG_VAR( "heading_scale", a_headingScale, double );
    CONFIG_VAR( "remap_heading", a_powerateHeading, bool );
    CONFIG_VAR( "remap_throttle", a_powerateThrottle, bool );
    CONFIG_VAR( "heading_gain", a_headingGain, double );
    CONFIG_VAR( "throttle_gain", a_throttleGain, double );
}

Drive::~Drive( void )
{
}

void Drive::init( void )
{
    CONNECT_INPUT( "heading", driveInputSetHeading, Drive, double );
    CONNECT_INPUT( "throttle", driveInputSetThrottle, Drive, double );

    CONNECT_INPUT( "quick_turn", driveInputSetQuickTurn, Drive, bool );
    CONNECT_INPUT( "anti_turbo", driveInputSetAntiTurbo, Drive, bool );

    CONNECT_INPUT( "gyro", driveInputGyroValue, Drive, double );
    CONNECT_INPUT( "left_master_sensor", driveInputLeftEncoderValue, Drive, double );
    CONNECT_INPUT( "right_master_sensor", driveInputRightEncoderValue, Drive, double );

    CONNECT_OUTPUT( "left_master", a_leftMaster, smartOutputCmdT );
    CONNECT_OUTPUT( "right_master", a_rightMaster, smartOutputCmdT );
    CONNECT_OUTPUT( "left_follower_one", a_leftFollowerOne, smartOutputCmdT );
    CONNECT_OUTPUT( "right_follower_one", a_rightFollowerOne, smartOutputCmdT );
    CONNECT_OUTPUT( "left_follower_two", a_leftFollowerTwo, smartOutputCmdT );
    CONNECT_OUTPUT( "right_follower_two", a_rightFollowerTwo, smartOutputCmdT );

    a_leftFollowerOne.control_id = SMART_OUTPUT_CONTROL_MODE_ID_FOLLOW;
    a_leftFollowerOne.follow_output_id = 1;

    a_rightFollowerOne.control_id = SMART_OUTPUT_CONTROL_MODE_ID_FOLLOW;
    a_rightFollowerOne.follow_output_id = 6;

    a_leftFollowerTwo.control_id = SMART_OUTPUT_CONTROL_MODE_ID_FOLLOW;
    a_leftFollowerTwo.follow_output_id = 1;

    a_rightFollowerTwo.control_id = SMART_OUTPUT_CONTROL_MODE_ID_FOLLOW;
    a_rightFollowerTwo.follow_output_id = 6;

    this->addProbe( &a_turnCorrectionProbe );
    this->addProbe( &a_driveHeadingPidProbe );
    this->addProbe( &a_drivePidProbe );
    this->addProbe( &a_leftMotorProbe );
    this->addProbe( &a_rightMotorProbe );
    this->addProbe( &a_remappedHeadingProbe );
    this->addProbe( &a_remappedThrottleProbe );
    this->addProbe( &a_headingSetPointProbe );

    resetState();
}

void Drive::resetState( void )
{
    a_headingSetPoint = a_gyroValue;
    this->setBrakeMode( false );
    this->setDriveModeCheesy();
}

void Drive::selfTest( void )
{
}

void Drive::update( void )
{
    SD_PUT( "throttle_value", a_throttleValue );
    SD_PUT( "heading_value", a_headingValue );
    SD_PUT( "drive_mode", toString( a_driveMode ) );

    a_headingSetPointProbe.update( a_headingSetPoint );

    switch( a_driveMode )
    {
        case WS_DRIVE_TYPE_CHEESY:
            {
                double effective_throttle = a_throttleValue;
                double effective_heading = a_headingValue;

                if( a_headingScale < .5 ) a_headingScale = .5;
                if( a_throttleScale < .5 ) a_throttleScale = .5;
                if( a_headingGain < 1.0 ) a_headingGain = 1.0;
                if( a_throttleGain < 1.0 ) a_throttleGain = 1.0;

                if( a_powerateHeading )
                {
                    effective_heading =
                        powerate( effective_heading,
                                  a_headingGain );
                }

                if( a_powerateThrottle )
                {
                    effective_throttle =
                        powerate( effective_throttle,
                                  a_throttleGain );
                }

                effective_throttle *= a_throttleScale;
                effective_heading *= a_headingScale;

                a_remappedThrottleProbe.update( effective_throttle );
                a_remappedHeadingProbe.update( effective_heading );

                if( a_antiTurbo )
                {
                    effective_throttle = a_throttleValue * sc_antiTurboFactor;
                }

                a_driveSignal =
                    a_cheesyHelper.cheesyDrive( effective_throttle,
                                                effective_heading,
                                                a_quickTurn );
            }
            break;

        case WS_DRIVE_TYPE_FULL_BRAKE:
            // nothing to do
            break;

        case WS_DRIVE_TYPE_AUTO_DRIVE_STRAIGHT:
            ap_autoDriveHeadingPidIO->setGyroValue( a_gyroValue );
            ap_autoDrivePidIO->setEncoderValue( this->getDrivePidEncoderValue() );

            a_autoDrivePid.calcPid();
            a_autoDriveHeadingPid.calcPid();

            if( a_drivePidThrottle >= a_drivePidSpeedCoeff )
            {
                a_drivePidThrottle = a_drivePidSpeedCoeff;
            }
            else if( a_drivePidThrottle <= ( a_drivePidSpeedCoeff * -1.0 ) )
            {
                a_drivePidThrottle = a_drivePidSpeedCoeff * -1.0;
            }

            if( a_drivePidHeading >= 1.0 )
            {
                a_drivePidHeading = 1.0;
            }
            else if( a_drivePidHeading <= -1.0 )
            {
                a_drivePidHeading = -1.0;
            }

            a_drivePidProbe.update( a_drivePidThrottle );
            a_driveHeadingPidProbe.update( a_drivePidHeading );

            a_driveSignal.leftMotor = a_drivePidThrottle;
            a_driveSignal.rightMotor = a_drivePidThrottle;

            this->calculateTurnPidCorrections( a_driveSignal, a_drivePidHeading, 1.0 );

            break;

        case WS_DRIVE_TYPE_AUTO_TURN:
            ap_autoTurnPidIO->setGyroValue( a_gyroValue );

            a_autoTurnPid.calcPid();

            if( a_turnPidHeading >= a_turnPidSpeedCoeff )
            {
                a_turnPidHeading = a_turnPidSpeedCoeff;
            }
            else if( a_turnPidHeading <= ( a_turnPidSpeedCoeff * -1.0 ) )
            {
                a_turnPidHeading = a_turnPidSpeedCoeff * -1.0;
            }

            a_turnCorrectionProbe.update( a_turnPidHeading );

            a_driveSignal.leftMotor = 0.0;
            a_driveSignal.rightMotor = 0.0;

            this->calculateTurnPidCorrections( a_driveSignal, a_turnPidHeading, 2.0 );

            break;

        case WS_DRIVE_TYPE_AUTO_ARC:
            ap_autoArcAnglePidIO->setGyroAngle( a_gyroValue );
            ap_autoArcDrivePidIO->setEncoderValue( this->getArcEncoderValue() );
            ap_autoArcDrivePidIO->setEncoderValue( this->getArcEncoderValue() );

            a_autoArcAnglePid.calcPid();
            a_autoArcDrivePid.calcPid();

            if( a_arcPidThrottle >= a_arcPidSpeedCoeff )
            {
                a_arcPidThrottle = a_arcPidSpeedCoeff;
            }
            else if( a_arcPidThrottle <= ( a_arcPidSpeedCoeff * -1.0 ) )
            {
                a_arcPidThrottle = a_arcPidSpeedCoeff * -1.0;
            }

            if( a_arcPidHeading >= 1.0 )
            {
                a_arcPidHeading = 1.0;
            }
            else if( a_arcPidHeading <= -1.0 )
            {
                a_arcPidHeading = -1.0;
            }

            a_driveSignal.leftMotor = a_arcPidThrottle;
            a_driveSignal.rightMotor = a_arcPidThrottle;

            this->calculateTurnPidCorrections( a_driveSignal, a_arcPidHeading, 1.0 );

            break;

        case WS_DRIVE_TYPE_NONE:
        case WS_DRIVE_TYPE_NUM:
        default:
            WS_ERROR( "bad drive state detected, switching to raw." );
            a_driveMode = WS_DRIVE_TYPE_RAW;

            //
            // intentional fall through
            //

        case WS_DRIVE_TYPE_RAW:
            a_driveSignal = { a_throttleValue, a_throttleValue };
            break;
    }

    setMotorSpeeds( a_driveSignal );

    SD_PUT( "left_encoder", this->getLeftEncoderValue() );
    SD_PUT( "right_encoder", this->getRightEncoderValue() );
}

void Drive::setBrakeMode( bool brakeOn )
{
    a_rightMaster.brake_mode = brakeOn;
    a_leftMaster.brake_mode = brakeOn;
    a_rightFollowerOne.brake_mode = brakeOn;
    a_leftFollowerOne.brake_mode = brakeOn;
    a_rightFollowerTwo.brake_mode = brakeOn;
    a_leftFollowerTwo.brake_mode = brakeOn;
}

void Drive::setMotorSpeeds( wsDriveSignalT signal )
{
    a_leftMotorProbe.update( signal.leftMotor );
    a_rightMotorProbe.update( signal.rightMotor );

    a_leftMaster.control_id =
        SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
    a_leftMaster.speed = signal.leftMotor;

    a_rightMaster.control_id =
        SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
    a_rightMaster.speed = signal.rightMotor;
}

void Drive::setDriveModeCheesy( void )
{
    a_driveMode = WS_DRIVE_TYPE_CHEESY;
    setBrakeMode( false );
}

void Drive::setDriveModeRaw( void )
{
    a_driveMode = WS_DRIVE_TYPE_RAW;
    setBrakeMode( false );
}

void Drive::setDriveModeFullBrake( void )
{
    // Set talons to hold their current position
    if( a_driveMode != WS_DRIVE_TYPE_FULL_BRAKE )
    {
#if 0
        ap_leftMaster->SelectProfileSlot( BASE_LOCK_PROFILE_SLOT );
        ap_leftMaster->SetTalonControlMode( CANTalon::kPositionMode );
        ap_leftMaster->SetAllowableClosedLoopErr(
            BRAKE_MODE_ALLOWABLE_ERROR );
        ap_leftMaster->Set( ap_leftMaster->GetPosition() );

        ap_rightMaster->SelectProfileSlot( BASE_LOCK_PROFILE_SLOT );
        ap_rightMaster->SetTalonControlMode( CANTalon::kPositionMode );
        ap_rightMaster->SetAllowableClosedLoopErr(
            BRAKE_MODE_ALLOWABLE_ERROR );
        ap_rightMaster->Set( ap_rightMaster->GetPosition() );
#endif

        setBrakeMode( true );
    }

    a_driveMode = WS_DRIVE_TYPE_FULL_BRAKE;
}

int Drive::driveInputSetHeading( double newHeading )
{
    a_headingValue = newHeading;
    return 0;
}

int Drive::driveInputSetThrottle( double newThrottle )
{
    a_throttleValue = newThrottle;
    return 0;
}

int Drive::driveInputSetQuickTurn( bool quickTurn )
{
    a_quickTurn = quickTurn;
    return 0;
}

int Drive::driveInputSetAntiTurbo( bool antiTurbo )
{
    a_antiTurbo = antiTurbo;
    return 0;
}

int Drive::driveInputGyroValue( double value )
{
    a_gyroValue = value;
    return 0;
}

int Drive::driveInputLeftEncoderValue( double value )
{
    a_leftEncoderValue = value;
    return 0;
}

int Drive::driveInputRightEncoderValue( double value )
{
    a_rightEncoderValue = value;
    return 0;
}

int Drive::setTurnPidHeadingCorrection( double heading )
{
    a_turnPidHeading = heading;
    return 0;
}

int Drive::setDrivePidHeadingCorrection( double heading )
{
    a_drivePidHeading = heading;
    return 0;
}

int Drive::setPidDriveThrottle( double throttle )
{
    a_drivePidThrottle = throttle;
    return 0;
}

void Drive::calculateTurnPidCorrections( wsDriveSignalT& r_signal,
                                         double correction,
                                         double dynamic_range )
{
    correction = ( correction > 1.0 ) ? 1.0 : correction;
    correction = ( correction < -1.0 ) ? -1.0 : correction;

    correction *= dynamic_range;

    double turn_pid_value = correction * 0.5;
    r_signal.leftMotor -= turn_pid_value;
    r_signal.rightMotor += turn_pid_value;

    if( r_signal.leftMotor > 1.0 )
    {
        double diff = r_signal.leftMotor - 1.0;
        r_signal.leftMotor = 1.0;
        r_signal.rightMotor -= diff;
    }
    else if( r_signal.rightMotor > 1.0 )
    {
        double diff = r_signal.rightMotor - 1.0;
        r_signal.rightMotor = 1.0;
        r_signal.leftMotor -= diff;
    }
    else if( r_signal.leftMotor < -1.0 )
    {
        double diff = r_signal.leftMotor + 1.0;
        r_signal.leftMotor = -1.0;
        r_signal.rightMotor -= diff;
    }
    else if( r_signal.rightMotor < -1.0 )
    {
        double diff = r_signal.rightMotor + 1.0;
        r_signal.rightMotor = -1.0;
        r_signal.leftMotor -= diff;
    }

    r_signal.rightMotor = ( r_signal.rightMotor > 1.0 ) ? 1.0 : r_signal.rightMotor;
    r_signal.rightMotor = ( r_signal.rightMotor < -1.0 ) ? -1.0 : r_signal.rightMotor;
    r_signal.leftMotor = ( r_signal.leftMotor > 1.0 ) ? 1.0 : r_signal.leftMotor;
    r_signal.leftMotor = ( r_signal.leftMotor < -1.0 ) ? -1.0 : r_signal.leftMotor;
}

void Drive::setDriveModeDriveStraight( double speed, double distance )
{
    a_autoDrivePid.disable();
    a_autoDrivePid.enable();

    a_autoDriveHeadingPid.disable();
    a_autoDriveHeadingPid.enable();

    a_driveMode = WS_DRIVE_TYPE_AUTO_DRIVE_STRAIGHT;
    a_autoDrivePid.setSetPoint(
        this->getDrivePidEncoderValue() + distance );
    a_autoDriveHeadingPid.setSetPoint( a_headingSetPoint );

    a_drivePidSpeedCoeff = speed;
    if( a_drivePidSpeedCoeff < 0.0 ) a_drivePidSpeedCoeff *= -1.0;
}

void Drive::setDriveModeTurn( double angle, double speed )
{
    a_autoTurnPid.disable();
    a_autoTurnPid.enable();

    a_headingSetPoint += angle;

    a_driveMode = WS_DRIVE_TYPE_AUTO_TURN;
    a_autoTurnPid.setSetPoint( a_headingSetPoint );

    a_turnPidSpeedCoeff = speed;
    if( a_turnPidSpeedCoeff < 0.0 ) a_turnPidSpeedCoeff *= -1.0;
}

double Drive::getDrivePidEncoderValue( void )
{
    return this->getLeftEncoderValue();
}

double Drive::powerate( double x, double gain )
{
    if( fabs( x ) < 0.05 ) return 0.0;
    if( gain < 1.0 ) gain = 1.0;

    bool sign = ( x < 0.0 );

    x = fabs( x );
    x = pow( x, gain );

    x = sign ? ( -1.0 * x ) : x;

    return x;
}

int Drive::enableAutoPidLog( void )
{
    int err = 0;
    std::string home_dir = std::string( "/home/lvuser/" );
#ifdef TARGET_SIMULATION
    home_dir = std::string( "/home/sam/" );
#endif
    err += a_autoDrivePid.enableFileLogging(
        home_dir + std::string( "auto_drive_pid.txt" ) );
    err += a_autoTurnPid.enableFileLogging(
        home_dir + std::string( "auto_turn_pid.txt" ) );
    err += a_autoDriveHeadingPid.enableFileLogging(
        home_dir + std::string( "auto_drive_heading_pid.txt" ) );
    return err;
}

int Drive::disableAutoPidLog( void )
{
    int err = 0;
    err += a_autoDrivePid.disableFileLogging();
    err += a_autoTurnPid.disableFileLogging();
    err += a_autoDriveHeadingPid.disableFileLogging();
    return err;
}

void Drive::setDriveModeArc( double radius, double angle, double speed )
{
    a_arcRadius = radius;
    a_arcPidSpeedCoeff = speed;
    a_driveMode = WS_DRIVE_TYPE_AUTO_ARC;

    a_headingSetPoint += angle;
    a_arcAngle = a_headingSetPoint;

    ap_autoArcAnglePidIO->setRadius( a_arcRadius );
    a_autoArcAnglePid.disable();
    a_autoArcDrivePid.disable();
    a_autoArcAnglePid.enable();
    a_autoArcDrivePid.enable();

    a_autoArcDrivePid.setSetPoint( this->getArcEncoderValue() + 
        ( ( 2.0 * ( double )PI * fabs( radius ) * angle ) / 360.0 ) );
    a_autoArcAnglePid.setSetPoint( 0.0 );
}

double Drive::getArcEncoderValue( void )
{
    double value = 0.0;

    if( a_arcAngle < 0.0 )
    {
        value = this->getRightEncoderValue();
    }
    else
    {
        value = this->getLeftEncoderValue();
    }

    return value;
}

int Drive::setArcPidHeading( double heading )
{
    a_arcPidHeading = heading;
    return 0;
}

int Drive::setArcPidThrottle( double throttle )
{
    a_arcPidThrottle = throttle;
    return 0;
}

