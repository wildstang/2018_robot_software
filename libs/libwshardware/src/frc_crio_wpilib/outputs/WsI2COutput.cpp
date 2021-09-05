#include "WsI2COutput.h"

#include "config/ConfigValueRef.h"

#include "crio_utils.h"

std::vector< unsigned char > WsI2COutput::s_empty;

WsI2COutput::WsI2COutput( std::string name, enum frc::I2C::Port port, int address )
    : BinaryOutput( name, s_empty )
    , ap_i2c( NULL )
    , a_thread( 0 )
    , a_mutex( PTHREAD_MUTEX_INITIALIZER )
    , a_waitCondition( PTHREAD_COND_INITIALIZER )
    , a_running( true )
    , a_dataToSend( false )
    , a_bytesReceived()
    , a_bytesToSend()
    , a_port( port )
    , a_portStr()
    , ac_factoryConstructor( false )
    , a_address( address )
{
}

WsI2COutput::WsI2COutput( std::string name )
    : BinaryOutput( name, s_empty )
    , ap_i2c( NULL )
    , a_thread( 0 )
    , a_mutex( PTHREAD_MUTEX_INITIALIZER )
    , a_waitCondition( PTHREAD_COND_INITIALIZER )
    , a_running( true )
    , a_dataToSend( false )
    , a_bytesReceived()
    , a_bytesToSend()
    , a_port( frc::I2C::kOnboard )
    , a_portStr()
    , ac_factoryConstructor( true )
    , a_address( 0 )
{
    CONFIG_VAR( "port", a_portStr, std::string );
    CONFIG_VAR( "address", a_address, int );
}

WsI2COutput::~WsI2COutput( void )
{
    if( isRunning() )
    {
        this->stop();

        pthread_join( a_thread, NULL );
        pthread_mutex_destroy( &a_mutex );
        pthread_cond_destroy( &a_waitCondition );
    }

    if( ap_i2c != NULL )
    {
        delete ap_i2c;
        ap_i2c = NULL;
    }
}

int WsI2COutput::instantiate( void )
{
    if( ac_factoryConstructor )
    {
        a_port = parse_i2c_port( a_portStr );
    }

    if( ap_i2c != NULL ) delete ap_i2c;
    ap_i2c = new frc::I2C( a_port, a_address );

    pthread_mutex_init( &a_mutex, NULL );
    pthread_cond_init( &a_waitCondition, NULL );
    pthread_create( &a_thread, NULL, WsI2COutput::threadEntry, NULL );

    return 0;
}

void WsI2COutput::sendDataToOutput( void )
{
    std::vector< unsigned char > data = getValue();

    if( data.size() > 0 )
    {
        this->setSendData( data );
        pthread_cond_signal( &a_waitCondition );
    }
}

void* WsI2COutput::threadEntry( void* arg )
{
    WsI2COutput* p_instance = ( WsI2COutput* )arg;
    p_instance->instanceThreadEntry();
    return NULL;
}

void WsI2COutput::instanceThreadEntry( void )
{
    while( this->isRunning() )
    {
        // blocking sleep until someone calls notify.
        pthread_cond_wait( &a_waitCondition, &a_mutex );

        // Need at least a byte and someone has to have called setSendData.
        if( ( a_bytesToSend.size() >= 0 ) && a_dataToSend )
        {
            // set receive size to 0 to avoid sending an i2c read request.
            a_bytesReceived.resize( RESIZE_BUFFER_SIZE );

            ap_i2c->Transaction( &( a_bytesToSend[ 0 ] ), 
                    a_bytesToSend.size(), 
                    &( a_bytesReceived[ 0 ] ), 
                    RESIZE_BUFFER_SIZE );

            a_bytesToSend.clear();
            a_dataToSend = false;
        }

        pthread_mutex_unlock( &a_mutex );
    }
}

void WsI2COutput::setSendData( std::vector< unsigned char >& r_data )
{
    pthread_mutex_lock( &a_mutex );
    a_bytesToSend = r_data;
    a_dataToSend = true;
    pthread_mutex_unlock( &a_mutex );
}

void WsI2COutput::stop( void )
{
    pthread_mutex_lock( &a_mutex );
    a_running = false;
    pthread_mutex_unlock( &a_mutex );
}

bool WsI2COutput::isRunning( void )
{
    bool running = false;

    pthread_mutex_lock( &a_mutex );
    running = a_running;
    pthread_mutex_unlock( &a_mutex );

    return running;
}

