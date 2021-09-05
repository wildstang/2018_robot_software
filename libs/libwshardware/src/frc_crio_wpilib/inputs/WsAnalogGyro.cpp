#include "WsAnalogGyro.h"

#include <sys/time.h>
#include <unistd.h>

#include "AnalogGyro.h"

#include "utils/ws_utils.h"
#include "config/ConfigValueRef.h"

// GOOD DEFAULT VALUE TO USE
const double WsAnalogGyro::sc_driftPerNanoFixed = .903;

WsAnalogGyro::WsAnalogGyro( std::string name, int channel, bool drift_comp )
    : AnalogInput( name, 0.0 )
    , ap_gyro( NULL )
    , a_startTime( 0 )
    , a_driftPerNano( sc_driftPerNanoFixed )
    , a_driftComp( drift_comp )
    , a_channel( channel )
{
}

WsAnalogGyro::WsAnalogGyro( std::string name )
    : AnalogInput( name, 0.0 )
    , ap_gyro( NULL )
    , a_startTime( 0 )
    , a_driftPerNano( sc_driftPerNanoFixed )
    , a_driftComp( false )
    , a_channel( 0 )
{
    CONFIG_VAR( "channel", a_channel, int );
    CONFIG_VAR( "drift_comp", a_driftComp, bool );
}

WsAnalogGyro::~WsAnalogGyro( void )
{
    if( ap_gyro != NULL )
    {
        delete ap_gyro;
        ap_gyro = NULL;
    }
}

int WsAnalogGyro::instantiate( void )
{
    if( ap_gyro != NULL ) delete ap_gyro;
    ap_gyro = new frc::AnalogGyro( a_channel );
    this->calibrate();

    return 0;
}

double WsAnalogGyro::readRawValue( void )
{
    uint64_t usec = ws_microseconds();
    return ( ap_gyro->GetAngle() - 
            ( ( usec - a_startTime ) * a_driftPerNano ) );
}

void WsAnalogGyro::calibrate( void )
{
    ap_gyro->Calibrate();

    if( a_driftComp )
    {
        double angle1 = ap_gyro->GetAngle();
        a_startTime = ws_microseconds();

        usleep( 1000 );

        double angle2 = ap_gyro->GetAngle();
        uint64_t endTime = ws_microseconds();

        a_driftPerNano = ( angle2 - angle1 ) / ( double )( endTime - a_startTime );
    }
}

