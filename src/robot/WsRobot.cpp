#include "WsRobot.h"

#include <signal.h>
#include <unistd.h>
#include <execinfo.h>
#include <cstdlib>
#include <cstdio>

#include "subsystems/SubsystemManager.h"
#include "subsystems/Subsystem.h"

#include "utils/ws_debug.h"

#include "logger/WsLogger.h"
#include "logger/WsDebugLogOutput.h"

#include "io/Input.h"
#include "io/Output.h"
#include "io/InputManager.h"
#include "io/OutputManager.h"
#include "io/inputs/NullInputs.h"
#include "io/inputs/ModButtonInput.h"
#include "io/inputs/LogicButton.h"
#include "io/inserts/SafetyInsert.h"
#include "io/inserts/InvertInsert.h"
#include "io/inserts/DeadzoneInsert.h"
#include "io/inserts/DebounceInsert.h"
#include "io/outputs/Analogifier.h"

#include "auto/AutoManager.h"
#include "auto/WsAutoStepFactory.h"

#include "config/ConfigValueRef.h"

#include "Target.h"

#include "subsystems/Drive.h"
#include "subsystems/Monitor.h"
#include "subsystems/Arm.h"
#include "subsystems/Lift.h"
#include "subsystems/Claw.h"
#include "subsystems/Manipulator.h"
#include "subsystems/LED.h"
#include "subsystems/AutoChooser.h"

void ws_backtrace( int sig );

WS_START_ROBOT_CLASS( WsRobot );

WsRobot* WsRobot::sp_instance = NULL;

WsRobot::WsRobot( void )
    : RobotCore( new HARDWARE_CLASS(), new WsAutoStepFactory() )
    , a_lastCycleTime( 0 )
    , a_firstDisabled( true )
    , a_firstRun( true )
    , a_autoProgramHardcodeName()
    , a_useHardcodedProgram( false )
    , a_autoProgramStarted( false )
    , ap_monitor( NULL )
    , ap_drive( NULL )
    , ap_arm( NULL )
    , ap_lift( NULL )
    , ap_claw( NULL )
    , ap_manipulator( NULL )
    , ap_led( NULL )
    , a_autoDelayTimer()
{
    signal( SIGSEGV, ws_backtrace );

    // learn knowledge to anyone who
    // tries to use this class wrong.
    WS_ASSERT( sp_instance == NULL );
    sp_instance = this;

    CONFIG_VAR( "hardcoded_program_name", a_autoProgramHardcodeName, std::string );
    CONFIG_VAR( "use_hardcoded_name", a_useHardcodedProgram, bool );

#if defined(TARGET_SIMULATION)
    setConfigFileName( "/home/sam/robot_competition.json" );
    setAutoProgramDir( "/home/sam/autos" );
#elif defined(TARGET_FRC_CRIO_WPILIB)
    setConfigFileName( "/home/lvuser/robot_competition.json" );
    setAutoProgramDir( "/home/lvuser/autos" );
#else
#error "unsupported target"
#endif
}

WsRobot::~WsRobot( void )
{
    WS_ASSERT( sp_instance == this );
    sp_instance = NULL;
}

WsRobot* WsRobot::getInstance( void )
{
    //
    // this is not like a singleton class.
    // it is created by the START_ROBOT_CLASS macro,
    // so we don't actually have control over where the
    // instance is created. So we approximate the behavior
    // by saving off a pointer when the constructor runs
    // and just hoping to god that nobody tries to make
    // another one.
    //

    return sp_instance;
}

void WsRobot::wsRobotInit( void )
{
    this->getAutoChooser()->startup();
}

void WsRobot::wsRobotPeriodic( void )
{
}

void WsRobot::wsDisabledInit( void )
{
    this->getLED()->setTeleop( false );

    this->getSubsystemManager()->resetState();
    this->getAutoManager()->clear();

    WsDebugLogOutput::disableDebugLogging();
    this->getDrive()->disableAutoPidLog();
}

void WsRobot::wsDisabledPeriodic( void )
{
}

void WsRobot::resetRobotState( void )
{
}

int WsRobot::startAutonomousProgram( void )
{
    int err = 0;

    if( a_useHardcodedProgram )
    {
        this->getAutoManager()->startProgramByName( a_autoProgramHardcodeName );
    }
    else
    {
        std::string letters = this->getHardware()->getGameData().substr( 0, 2 );
        std::string program = this->getAutoChooser()->getProgramName( letters );
        if( !program.empty() )
        {
            this->getAutoManager()->startProgramByName( program );
        }
    }

    return err;
}

void WsRobot::wsAutonomousInit( void )
{
    this->getLED()->setTeleop( false );

    this->getAutoManager()->clear();
    this->getSubsystemManager()->resetState();

    WsDebugLogOutput::enableDebugLogging();
    this->getDrive()->enableAutoPidLog();

    a_autoProgramStarted = false;

    a_autoDelayTimer.stop();
    a_autoDelayTimer.reset();
    a_autoDelayTimer.start();
}

void WsRobot::wsAutonomousPeriodic( void )
{
    if( !a_autoProgramStarted )
    {
        if( a_autoDelayTimer.hasPeriodPassed( this->getAutoChooser()->getDelay() ) )
        {
            this->startAutonomousProgram();
            a_autoProgramStarted = true;

            a_autoDelayTimer.stop();
            a_autoDelayTimer.reset();
        }
    }
}

void WsRobot::wsTeleopInit( void )
{
    this->getLED()->setTeleop( true );

    this->getSubsystemManager()->resetState();
    this->getAutoManager()->clear();

    WsDebugLogOutput::disableDebugLogging();
    this->getDrive()->disableAutoPidLog();
}

void WsRobot::wsTeleopPeriodic( void )
{
}

void WsRobot::wsTestInit( void )
{
    this->getSubsystemManager()->resetState();
    this->getAutoManager()->clear();
}

void WsRobot::wsTestPeriodic( void )
{
}

void WsRobot::createOutputs( void )
{
    //---------------------------------
    // Motors
    //---------------------------------
    makeOutput( "left_master", OUTPUT_TYPE_CAN_TALON_CTRE_MAG_ENCODER );
    makeOutput( "left_follower_one", OUTPUT_TYPE_CAN_TALON );
    makeOutput( "left_follower_two", OUTPUT_TYPE_CAN_TALON );
    makeOutput( "right_master", OUTPUT_TYPE_CAN_TALON_CTRE_MAG_ENCODER );
    makeOutput( "right_follower_one", OUTPUT_TYPE_CAN_TALON );
    makeOutput( "right_follower_two", OUTPUT_TYPE_CAN_TALON );

    makeOutput( "lift_winch", OUTPUT_TYPE_CAN_TALON_CTRE_MAG_ENCODER );
    makeOutput( "lift_follower", OUTPUT_TYPE_CAN_TALON );

    makeOutput( new Analogifier( "lift_winch_analog", "lift_winch", 0.0 ) );
    makeSmartOutput( "lift_winch_smart", "lift_winch_sensor", "lift_winch_analog" );

    makeOutput( "lift_brake", OUTPUT_TYPE_DOUBLE_SOLENOID );

    makeOutput( "intake_left_motor", OUTPUT_TYPE_CAN_VICTOR );
    makeOutput( "intake_right_motor", OUTPUT_TYPE_CAN_VICTOR );
    makeOutput( new Analogifier( "intake_left", "intake_left_motor", 0.0 ) );
    makeOutput( new Analogifier( "intake_right", "intake_right_motor", 0.0 ) );

    makeOutput( "claw_in", OUTPUT_TYPE_SOLENOID );
    makeOutput( "claw_out", OUTPUT_TYPE_SOLENOID );

    makeOutput( "arm_motor_victor", OUTPUT_TYPE_CAN_TALON );
    makeOutput( "arm_follower", OUTPUT_TYPE_CAN_TALON );
    makeOutput( new Analogifier( "arm_motor", "arm_motor_victor", 0.0 ) );
    makeSmartOutput( "arm_smart", "arm_pot", "arm_motor" );

    makeOutput( "led_pin_one", OUTPUT_TYPE_DIGITAL_OUTPUT );
    makeOutput( "led_pin_two", OUTPUT_TYPE_DIGITAL_OUTPUT );
    makeOutput( "led_pin_three", OUTPUT_TYPE_DIGITAL_OUTPUT );
}

void WsRobot::createInputs( void )
{
    /*===============================================================
    /       Driver and Manipulator Controller Button Locations
    / ===============================================================
    /
    /    +-------------------------------------------------------+
    /  +  +---------+              [TOP]              +---------+  +
    /  |  |    6    |                                 |    7    |  |
    /  |  +---------+                                 +---------+  |
    /  |                                                           |
    /  |  +---------+                                 +---------+  |
    /  |  |    4    |                                 |    5    |  |
    /  +  +---------+                                 +---------+  +
    /    +-------------------------------------------------------+
    /
    /    +-------------------------------------------------------+
    /   /    +--+                 [FRONT]                         \
    /  +     |2 |                                         (3)      +
    /  |  +--+  +--+        +----+       +----+                    |
    /  |  |0     1 |        | 8  |  (X)  |  9 |       (0)     (2)  |
    /  |  +--+  +--+        +----+       +----+                    |
    /  |     |3 |                                         (1)      |
    /  |     +--+     +--+          (X)          +--+              |
    /  |             /    \                     /    \             |
    /  |         0  |  10  |                2  |  11  |            |
    /  |             \    /                     \    /             |
    /  +              +--+                       +--+              +
    /   \              1                          3               /
    /    \            +-----------------------------+            /
    /     \          /                               \          /
    /      \        /                                 \        /
    /       \      /                                   \      /
    /        +----+                                     +----+
    /
    / */

    // --------------------------------
    // Driver Buttons
    // --------------------------------
    makeInput( "quick_turn", INPUT_TYPE_JOYSTICK_BUTTON );
    makeInput( "anti_turbo", INPUT_TYPE_JOYSTICK_BUTTON );

    makeInput( "next_auto_button", INPUT_TYPE_JOYSTICK_BUTTON );
    makeInput( "prev_auto_button", INPUT_TYPE_JOYSTICK_BUTTON );

    makeInput( "next_field_auto_button", INPUT_TYPE_JOYSTICK_BUTTON );
    makeInput( "prev_field_auto_button", INPUT_TYPE_JOYSTICK_BUTTON );

    makeInput( "want_cube_button", INPUT_TYPE_JOYSTICK_BUTTON );

    //---------------------------------
    // Driver Joysticks
    //---------------------------------
    makeInput( "throttle", INPUT_TYPE_JOYSTICK_AXIS );
    makeInput( "heading", INPUT_TYPE_JOYSTICK_AXIS );

    // --------------------------------
    // Manipulator Buttons
    // --------------------------------
    makeInput( "intake_button", INPUT_TYPE_JOYSTICK_BUTTON );
    makeInput( "outtake_button_target", INPUT_TYPE_JOYSTICK_BUTTON );

    makeInput( "claw_open_lock_button", INPUT_TYPE_JOYSTICK_BUTTON )
        ->addInsert( new DebounceInsert( 5 ) );
    makeInput( "claw_un_open_lock_button_target", INPUT_TYPE_JOYSTICK_BUTTON )
        ->addInsert( new DebounceInsert( 3 ) );

    makeInput( "claw_drop_button", INPUT_TYPE_JOYSTICK_BUTTON )
        ->addInsert( new DebounceInsert( 3 ) );
    makeInput( "claw_close_lock_button_target", INPUT_TYPE_JOYSTICK_BUTTON )
        ->addInsert( new DebounceInsert( 3 ) );

    makeInput( "plus_one_shift_button", INPUT_TYPE_JOYSTICK_BUTTON );
    makeInput( "endgame_safety_shift_button", INPUT_TYPE_JOYSTICK_BUTTON )
        ->addInsert( new SafetyInsert( 1000 ) );

    makeInput( "switch_position_button", INPUT_TYPE_JOYSTICK_BUTTON )
        ->addInsert( new DebounceInsert( 3 ) );
    makeInput( "scale_position_button", INPUT_TYPE_JOYSTICK_BUTTON )
        ->addInsert( new DebounceInsert( 3 ) );
    makeInput( "intake_position_button", INPUT_TYPE_JOYSTICK_BUTTON )
        ->addInsert( new DebounceInsert( 3 ) );
    makeInput( "carry_position_button", INPUT_TYPE_JOYSTICK_BUTTON )
        ->addInsert( new DebounceInsert( 3 ) );
    makeInput( "scale_backwards_position_button", INPUT_TYPE_JOYSTICK_BUTTON )
        ->addInsert( new DebounceInsert( 3 ) );

    //---------------------------------
    // Manipulator Mod Buttons
    //---------------------------------
    makeInput( new ModButtonInput( "claw_drop_spit_button",
                                   "plus_one_shift_button",
                                   "outtake_button_target", false ) );
    makeInput( new ModButtonInput( "outtake_button",
                                   "plus_one_shift_button",
                                   "outtake_button_target", true ) );

    makeInput( new ModButtonInput( "switch_normal_position_button",
                                   "plus_one_shift_button",
                                   "switch_position_button", false ) );
    makeInput( new ModButtonInput( "switch_plus_one_position_button",
                                   "plus_one_shift_button",
                                   "switch_position_button", true ) );

    makeInput( new ModButtonInput( "scale_normal_position_button",
                                   "plus_one_shift_button",
                                   "scale_position_button", false ) );
    makeInput( new ModButtonInput( "scale_plus_one_position_button",
                                   "plus_one_shift_button",
                                   "scale_position_button", true ) );

    makeInput( new ModButtonInput( "intake_normal_position_button",
                                   "plus_one_shift_button",
                                   "intake_position_button", false ) );
    makeInput( new ModButtonInput( "intake_plus_one_position_button",
                                   "plus_one_shift_button",
                                   "intake_position_button", true ) );

    makeInput( new ModButtonInput( "carry_normal_position_button",
                                   "plus_one_shift_button",
                                   "carry_position_button", false ) );
    makeInput( new ModButtonInput( "carry_plus_one_position_button",
                                   "plus_one_shift_button",
                                   "carry_position_button", true ) );

    makeInput( new ModButtonInput( "scale_reverse_normal_position_button",
                                   "plus_one_shift_button",
                                   "scale_backwards_position_button", false ) );
    makeInput( new ModButtonInput( "scale_reverse_plus_one_position_button",
                                   "plus_one_shift_button",
                                   "scale_backwards_position_button", true ) );

    makeInput( new ModButtonInput( "fork_deploy_button",
                                   "endgame_safety_shift_button",
                                   "claw_un_open_lock_button_target", true ) );
    makeInput( new ModButtonInput( "climb_button",
                                   "endgame_safety_shift_button",
                                   "claw_close_lock_button_target", true ) );

    makeInput( new ModButtonInput( "claw_un_open_lock_button",
                                   "endgame_safety_shift_button",
                                   "claw_un_open_lock_button_target", false ) );
    makeInput( new ModButtonInput( "claw_close_lock_button",
                                   "endgame_safety_shift_button",
                                   "claw_close_lock_button_target", false ) );

    //---------------------------------
    // Manipulator Joysticks
    //---------------------------------
    makeInput( "lift_joy", INPUT_TYPE_JOYSTICK_AXIS )
        ->addInsert( new InvertInsert() )
        ->addInsert( new DeadzoneInsert( 0.1 ) );
    makeInput( "arm_joy", INPUT_TYPE_JOYSTICK_AXIS )
        ->addInsert( new DeadzoneInsert( 0.1 ) );

    //---------------------------------
    // Sensors
    //---------------------------------
    makeInput( "gyro", INPUT_TYPE_NAVX_GYRO );
    makeInput( "arm_pot", INPUT_TYPE_ABSOLUTE_ENCODER );

    makeInput( "lift_bottom_switch", INPUT_TYPE_DIGITAL )
        ->addInsert( new DebounceInsert( 2 ) );
    makeInput( "arm_bottom_switch", INPUT_TYPE_DIGITAL )
        ->addInsert( new InvertInsert() )
        ->addInsert( new DebounceInsert( 2 ) );
    makeInput( "arm_top_switch", INPUT_TYPE_DIGITAL )
        ->addInsert( new InvertInsert() )
        ->addInsert( new DebounceInsert( 2 ) );

    makeInput( "claw_left_switch", INPUT_TYPE_DIGITAL )
        ->addInsert( new InvertInsert() )
        ->addInsert( new DebounceInsert( 2 ) );
    makeInput( "claw_right_switch", INPUT_TYPE_DIGITAL )
        ->addInsert( new InvertInsert() )
        ->addInsert( new DebounceInsert( 2 ) );
}

void WsRobot::createSubsystems( void )
{
    SubsystemManager* p_mgr = this->getSubsystemManager();

    // WHERE TO ADD SUBSYSTEMS
    p_mgr->addSubsystem( ap_drive = new Drive() );
    p_mgr->addSubsystem( ap_arm = new Arm() );
    p_mgr->addSubsystem( ap_monitor = new Monitor() );
    p_mgr->addSubsystem( ap_claw = new Claw() );
    p_mgr->addSubsystem( ap_lift = new Lift() );
    p_mgr->addSubsystem( ap_manipulator = new Manipulator() );
    p_mgr->addSubsystem( ap_led = new LED() );
    p_mgr->addSubsystem( ap_autoChooser = new AutoChooser() );
}

void ws_backtrace( int sig )
{
    printf( "CRASHED, caught sig %d\n", sig );

    ws_bt();

    exit( -1 );
}
