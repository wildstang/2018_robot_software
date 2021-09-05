#include "WsHallEffectInput.h"

#include <unistd.h>
#include <cstdio>

#include "config/ConfigValueRef.h"
#include "crio_utils.h"

WsHallEffectInput::WsHallEffectInput( std::string name, frc::I2C::Port port, int address )
    : DiscreteInput( name, 0 )
    , ap_i2c( NULL )
    , a_lastHallEffectSensor( -1 )
    , a_selectedHallEffectSensor( -1 )
    , a_running( false )
    , a_quit( false )
    , a_sleepTime( 20000 )
    , a_thread( 0 )
    , a_mutex( PTHREAD_MUTEX_INITIALIZER )
    , a_port( port )
    , a_portStr()
    , a_address( address )
    , ac_factoryConstructor( false )
    , a_pollRate( 20000 )
{
}

WsHallEffectInput::WsHallEffectInput( std::string name )
    : DiscreteInput( name, 0 )
    , ap_i2c( NULL )
    , a_lastHallEffectSensor( -1 )
    , a_selectedHallEffectSensor( -1 )
    , a_running( false )
    , a_quit( false )
    , a_sleepTime( 20000 )
    , a_thread( 0 )
    , a_mutex( PTHREAD_MUTEX_INITIALIZER )
    , a_port( frc::I2C::kOnboard )
    , a_portStr()
    , a_address( 0 )
    , ac_factoryConstructor( true )
    , a_pollRate( 20000 )
{
    CONFIG_VAR( "address", a_address, int );
    CONFIG_VAR( "port", a_portStr, std::string );
}

WsHallEffectInput::~WsHallEffectInput( void )
{
    if( isRunning() )
    {
        this->stop();
        this->quit();

        void* arg = NULL;
        pthread_join( a_thread, &arg );
        pthread_mutex_destroy( &a_mutex );
    }

    if( ap_i2c != NULL )
    {
        delete ap_i2c;
        ap_i2c = NULL;
    }
}

int WsHallEffectInput::instantiate( void )
{
    if( ac_factoryConstructor )
    {
        a_port = parse_i2c_port( a_portStr );
    }

    if( ap_i2c != NULL ) delete ap_i2c;
    ap_i2c = new frc::I2C( a_port, a_address );

    pthread_mutex_init( &a_mutex, NULL );
    pthread_create( &a_thread, NULL, WsHallEffectInput::run, this );
    
    // Update at 50Hz
    this->start( a_pollRate );

    return 0;
}

void WsHallEffectInput::start( int sleep )
{
    pthread_mutex_lock( &a_mutex );
    a_running = true;
    a_sleepTime = sleep;
    pthread_mutex_unlock( &a_mutex );
}

void WsHallEffectInput::stop( void )
{
    pthread_mutex_lock( &a_mutex );
    a_running = false;
    pthread_mutex_unlock( &a_mutex );
}

int WsHallEffectInput::readRawValue( void )
{
    // The selected sensor is set by the background thread update. 
    // Simply return the last value we know of
    return a_selectedHallEffectSensor;
}

bool WsHallEffectInput::shouldQuit( void )
{
    bool quit = false;
    pthread_mutex_lock( &a_mutex );
    quit = a_quit;
    pthread_mutex_unlock( &a_mutex );
    return quit;
}

void WsHallEffectInput::quit( void )
{
    pthread_mutex_lock( &a_mutex );
    a_quit = true;
    pthread_mutex_unlock( &a_mutex );
}

bool WsHallEffectInput::isRunning( void )
{
    bool running = false;
    pthread_mutex_lock( &a_mutex );
    running = a_running;
    pthread_mutex_unlock( &a_mutex );
    return running;
}

void* WsHallEffectInput::run( void* arg )
{
    WsHallEffectInput* p_hall_effect = ( WsHallEffectInput* )arg;
    p_hall_effect->run();
    return NULL;
}

void WsHallEffectInput::run( void )
{
    int sleep_time = 0;
    pthread_mutex_lock( &a_mutex );
    sleep_time = a_sleepTime;
    pthread_mutex_unlock( &a_mutex );

    while( !shouldQuit() )
    {
        while( isRunning() )
        {
            unsigned char c;
            ap_i2c->ReadOnly( 1, &c );

            pthread_mutex_lock( &a_mutex );
            a_selectedHallEffectSensor = c;

            if( a_selectedHallEffectSensor != a_lastHallEffectSensor )
            {
                printf( "READ HALL EFFECT: " + a_selectedHallEffectSensor );
            }

            a_lastHallEffectSensor = a_selectedHallEffectSensor;
            pthread_mutex_unlock( &a_mutex );

            if( !isRunning() ) break;

            usleep( sleep_time );
        }

        if( shouldQuit() ) break;

        usleep( 100000 );
    }
}

