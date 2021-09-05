#include "WsVision.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <sys/types.h>
#include <time.h>

#include "core/RobotCore.h"
#include "utils/ws_string.h"
#include "logger/WsLogger.h"
#include "logger/WsSmartDashboard.h"
#include "config/ConfigValueRef.h"

#define VISION_RECV_BUFF_SIZE 128
#define VISION_LINE_BUFF_SIZE 1024

const int WsVision::sc_hMinDefault = 64;
const int WsVision::sc_sMinDefault = 0;
const int WsVision::sc_vMinDefault = 0;
const int WsVision::sc_hMaxDefault = 154;
const int WsVision::sc_sMaxDefault = 255;
const int WsVision::sc_vMaxDefault = 255;
const int WsVision::sc_centerDefault = 0;
const int WsVision::sc_thresholdDefault = 10;
const double WsVision::sc_blurRadiusDefault = 5.0;

WsVision::WsVision( void )
    : WildstangModule( std::string( "vision" ), RobotCore::getRobotCore() )
    , WsNetConsole( 5800 )
    , a_hMin( sc_hMinDefault )
    , a_sMin( sc_sMinDefault )
    , a_vMin( sc_vMinDefault )
    , a_hMax( sc_hMaxDefault )
    , a_sMax( sc_sMaxDefault )
    , a_vMax( sc_vMaxDefault )
    , a_center( sc_centerDefault )
    , a_threshold( sc_thresholdDefault )
    , a_blurRadius( sc_blurRadiusDefault )
    , a_xCorrectionLevel( 0.0 )
    , a_distance( 0.0 )
{
    CONFIG_VAR( "h_min", a_hMin, int );
    CONFIG_VAR( "s_min", a_sMin, int );
    CONFIG_VAR( "v_min", a_vMin, int );
    CONFIG_VAR( "h_max", a_hMax, int );
    CONFIG_VAR( "s_max", a_sMax, int );
    CONFIG_VAR( "v_max", a_vMax, int );
    CONFIG_VAR( "center", a_center, int );
    CONFIG_VAR( "threshold", a_threshold, int );
    CONFIG_VAR( "blur_radius", a_blurRadius, double );

    sendPreferences();
}

WsVision::~WsVision( void )
{
}

int WsVision::handleLine( const char* line )
{
    std::vector< std::string > v = ws_strtok_sep( line, "[,|]\n\r" );
    std::vector< double > params;

    std::vector< std::string >::iterator i = v.begin();
    for(; i != v.end(); ++i )
    {
        params.push_back( atof( i->c_str() ) );
    }

    if( params.size() > CORRECTION_LEVEL_INDEX )
    {
        a_xCorrectionLevel = params[ CORRECTION_LEVEL_INDEX ];
    }

    if( params.size() > DISTANCE_INDEX )
    {
        a_distance = params[ DISTANCE_INDEX ];
    }

    SD_PUT( "Distance", a_distance );
    SD_PUT( "xCorrection", a_xCorrectionLevel );

    return 0;
}

void WsVision::enableVideoLogging( void )
{
    net_printf( "b\n" );
}

void WsVision::disableVideoLogging( void )
{
    net_printf( "e\n" );
}

void WsVision::sendPreferences( void )
{
    std::string buf;

    buf.append( toString( a_hMin ) );
    buf.append( "|" );
    buf.append( toString( a_sMin ) );
    buf.append( "|" );
    buf.append( toString( a_vMin ) );
    buf.append( "|" );
    buf.append( toString( a_hMax ) );
    buf.append( "|" );
    buf.append( toString( a_sMax ) );
    buf.append( "|" );
    buf.append( toString( a_vMax ) );
    buf.append( "|" );
    buf.append( toString( a_center ) );
    buf.append( "|" );
    buf.append( toString( a_threshold ) );
    buf.append( "|" );
    buf.append( toString( a_blurRadius ) );
    buf.append( "\n" );

    net_printf( "%s", buf.c_str() );
}

double WsVision::getXCorrectionLevel( void ) 
{ 
    return a_xCorrectionLevel;
}

double WsVision::getDistance( void ) 
{
    return a_distance;
}

