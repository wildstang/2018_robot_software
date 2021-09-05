#include "Monitor.h"

#include "utils/ws_string.h"

#include "robot/WsRobot.h"

const char* const Monitor::sc_names[] = { "drive_right_master",
                                          "drive_right_follower",
                                          "shooter_right",
                                          "climber",
                                          "feed_right",
                                          "pcm",
                                          "mxp",
                                          "unmonitored_7",
                                          "unmonitored_8",
                                          "unmonitored_9",
                                          "Intake",
                                          "feed_left",
                                          "unmonitored_12",
                                          "shooter_left",
                                          "drive_left_master",
                                          "drive_left_follower",
                                          NULL };

Monitor::Monitor( void )
    : Subsystem( std::string( "monitor" ) )
    , a_length( 0 )
{
    for( a_length = 0; sc_names[ a_length ] != NULL; a_length++ );
}

Monitor::~Monitor( void )
{
}

void Monitor::init( void )
{
}

void Monitor::resetState( void )
{
}

void Monitor::update( void )
{
}

void Monitor::selfTest( void )
{
}

// TODO
#if 0
int Monitor::reportState( void )
{
    HardwareHandle* p_hw = RobotCore::getRobotCore()->getHardware();
    for( unsigned int i = 0; i < a_length; i++ )
    {
        double current = p_hw->getCurrent( i );
        this->addState( std::string( sc_names[ i ] ), toString( current ) );
        this->addState( ws_std_string_printf( "current_%d", i ), toString( current ) );
    }

    double totalCurrent = p_hw->getTotalCurrent();
    this->addState( "total_ current", toString( totalCurrent ) );

    double voltage = p_hw->getVoltage();
    this->addState( "voltage", toString( voltage ) );

    double pdpTemp = p_hw->getTemperature();
    this->addState( "temperature", toString( pdpTemp ) );

    bool isRobotEnabled = frc::DriverStation::GetInstance().IsEnabled();
    bool isRobotTeleop = frc::DriverStation::GetInstance().IsOperatorControl();
    bool isRobotAuton = frc::DriverStation::GetInstance().IsAutonomous();

    this->addState( "enabled", toString( isRobotEnabled ) );
    this->addState( "teleop", toString( isRobotTeleop ) );
    this->addState( "auto", toString( isRobotAuton ) );

    return 0;
}
#endif

