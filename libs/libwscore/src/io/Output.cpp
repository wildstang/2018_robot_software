#include "Output.h"

#include "logger/WsLogger.h"
#include "io/OutputManager.h"
#include "io/routing/OutputSource.h"
#include "io/inserts/WsIOInsert.h"

#include "utils/ws_string.h"
#include "utils/ws_debug.h"

const smartOutputCmdT Output::sc_nullCommand = 
    { SMART_OUTPUT_CONTROL_MODE_ID_NONE, 0.0, 0, 0.0, 0.0, false, false };

Output::Output( OutputManager* p_manager, std::string name )
    : WildstangModule( name, p_manager )
    , a_inserts()
    , ap_outputManager( p_manager )
    , a_name( name )
    , a_enabled( true )
    , ap_source( NULL )
    , a_instantiated( false )
    , a_instantiationWarningCount( 0 )
{
    WS_ASSERT( ws_is_well_formed_name( name.c_str() ) );
}

Output::~Output( void )
{
    this->clearInserts();
}

void Output::update( void )
{
    if( !isInstantiated() )
    {
        if( a_instantiationWarningCount == 0 )
        {
            WS_WARNING( "tried to use output %s: not instantiated.",
                        this->getName().c_str() );
        }

        a_instantiationWarningCount =
            ( a_instantiationWarningCount + 1 ) % 1000;

        return;
    }

    //WS_FUNC_HEAD;

    if( isEnabled() )
    {
        if( ap_source )
        {
            ap_source->pushToOutput( this );
        }

        sendDataToOutput();
    }

    //WS_FUNC_FOOT;
}

void Output::setValue( double value )
{
    WS_ERROR( "output %s: NOT IMPLEMENTED", this->getName().c_str() );
}

void Output::setValue( int value )
{
    WS_ERROR( "output %s: NOT IMPLEMENTED", this->getName().c_str() );
}

void Output::setValue( bool value )
{
    WS_ERROR( "output %s: NOT IMPLEMENTED", this->getName().c_str() );
}

void Output::setValue( std::vector< unsigned char > value )
{
    WS_ERROR( "output %s: NOT IMPLEMENTED", this->getName().c_str() );
}

void Output::setValue( smartOutputCmdT value )
{
    WS_ERROR( "output %s: NOT IMPLEMENTED", this->getName().c_str() );
}

void Output::setOutputSource( OutputSource* p_source )
{
    if( ap_source ) delete ap_source;
    ap_source = p_source;
}

Output* Output::addInsert( WsIOInsert* p_insert )
{
    if( p_insert != NULL )
    {
        a_inserts.push_back( p_insert );
    }
    else
    {
        WS_ERROR( "tried to add a null insert to %s",
                  this->getName().c_str() );
    }

    return this;
}

void Output::clearInserts( void )
{
    std::vector< WsIOInsert* >::iterator i = a_inserts.begin();
    for(; i != a_inserts.end(); ++i )
    {
        WsIOInsert* p_insert = *i;
        delete p_insert;
    }

    a_inserts.clear();
}

void Output::instantiateOutput( void )
{
    int err = this->instantiate();
    if( !err )
    {
        a_instantiated = true;
        WS_INFO( "%s instantiated", this->getName().c_str() );
    }
    else
    {
        WS_ERROR( "error instantiating output %s", this->getName().c_str() );
    }
}

std::string toString( smartOutputControlModeT mode )
{
    std::string str;

    switch( mode )
    {
        case SMART_OUTPUT_CONTROL_MODE_ID_FOLLOW:
            str = std::string( "follow" );
            break;
        case SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE:
            str = std::string( "percent_voltage" );
            break;
        case SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID:
            str = std::string( "position_pid" );
            break;
        case SMART_OUTPUT_CONTROL_MODE_ID_VELOCITY_PID:
            str = std::string( "velocity_pid" );
            break;
        case SMART_OUTPUT_CONTROL_MODE_ID_NONE:
        case SMART_OUTPUT_CONTROL_MODE_ID_NUM:
        default:
            str = std::string( "undefined" );
            break;
    }

    return str;
}

std::string toString( smartOutputCmdT x )
{
    return ws_std_string_printf( 
            "smart_output_control mode '%s' speed %.3f follow %d pos_sp %.3f vel_sp %.3f brake %d reset %d",
            toString( x.control_id ).c_str(), x.speed, x.follow_output_id, 
            x.pos_setpoint, x.vel_setpoint, x.brake_mode ? 1 : 0,
            x.reset_sensor ? 1 : 0 );
}

