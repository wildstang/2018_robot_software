#include "Analogifier.h"

#include "config/ConfigValueRef.h"
#include "io/OutputManager.h"

Analogifier::Analogifier( std::string name, 
                          std::string output_name, 
                          double default_value )
    : AnalogOutput( name, default_value )
    , a_outputName( output_name )
    , ap_motor( NULL )
{
}

Analogifier::Analogifier( std::string name )
    : AnalogOutput( name, 0.0 )
    , a_outputName()
    , ap_motor( NULL )
{
    CONFIG_VAR( "output_name", a_outputName, std::string );
}

Analogifier::~Analogifier( void )
{
}

int Analogifier::instantiate( void )
{
    ap_motor =
        RobotCore::getRobotCore()->getOutputManager()->getOutput( a_outputName );
    if( !ap_motor )
    {
        WS_ERROR( "output %s does not exist", a_outputName.c_str() );
        return 1;
    }

    return 0;
}

void Analogifier::sendDataToOutput( void )
{
    if( !this->isInstantiated() ) return;

    double speed = this->getValue();

    smartOutputCmdT cmd;
    cmd.control_id = SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE;
    cmd.speed = speed;
    cmd.follow_output_id = 0;
    cmd.pos_setpoint = 0.0;
    cmd.vel_setpoint = 0.0;
    cmd.brake_mode = false;

    ap_motor->setValue( cmd );
}

