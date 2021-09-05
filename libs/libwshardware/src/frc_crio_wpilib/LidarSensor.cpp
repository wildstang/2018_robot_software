#include "LidarSensor.h"

#include "I2C.h"

#include <unistd.h>
#include <cstring>

#define LIDARSENSOR_DISTANCES 6

const int LidarSensor::sc_lidarConfigRegister = 0x00;
const int LidarSensor::sc_lidarDistanceRegister = 0x8f;

LidarSensor::LidarSensor( enum frc::I2C::Port port, int address )
    : ap_i2c( NULL )
    , a_distance( 0 )
    , a_smoothDistance( 0 )
    , a_thread( 0 )
    , a_mutex( PTHREAD_MUTEX_INITIALIZER )
    , a_running( false )
    , a_quit( true )
    , a_sleepTime( 100 )
{
    ap_i2c = new frc::I2C( port, address );

    pthread_mutex_init( &a_mutex, NULL );
}

LidarSensor::~LidarSensor( void )
{
    this->stop();
    this->quit();

    void* ret = NULL;
    pthread_join( a_thread, &ret );
    pthread_mutex_destroy( &a_mutex );

    delete ap_i2c;
    ap_i2c = NULL;
}

int LidarSensor::getDistance( void )
{
    int distance = 0;
    pthread_mutex_lock( &a_mutex );
    distance = a_distance;
    pthread_mutex_unlock( &a_mutex );
    return distance;
}

int LidarSensor::getSmoothedDistance( void )
{
    int distance = 0;
    pthread_mutex_lock( &a_mutex );
    distance = a_smoothDistance;
    pthread_mutex_unlock( &a_mutex );
    return distance;
}

double LidarSensor::pidGet( void )
{
    return ( double )( this->getDistance() );
}

void LidarSensor::start( int sleep )
{
    pthread_mutex_lock( &a_mutex );
    a_sleepTime = sleep;
    a_running = true;
    if( a_quit )
    {
        a_quit = false;
        pthread_create( &a_thread, NULL, LidarSensor::run, this );
    }
    pthread_mutex_unlock( &a_mutex );
}

void LidarSensor::stop( void )
{
    pthread_mutex_lock( &a_mutex );
    a_running = false;
    pthread_mutex_unlock( &a_mutex );
}

void LidarSensor::quit( void )
{
    pthread_mutex_lock( &a_mutex );
    a_quit = true;
    pthread_mutex_unlock( &a_mutex );
}

bool LidarSensor::shouldQuit( void )
{
    bool quit = false;
    pthread_mutex_lock( &a_mutex );
    quit = a_quit;
    pthread_mutex_unlock( &a_mutex );
    return quit;
}

bool LidarSensor::isRunning( void )
{
    bool run = false;
    pthread_mutex_lock( &a_mutex );
    run = a_running;
    pthread_mutex_unlock( &a_mutex );
    return run;
}

void* LidarSensor::run( void* arg )
{
    LidarSensor* p_sensor = ( LidarSensor* )arg;
    p_sensor->run();
    return NULL;
}

void LidarSensor::run( void )
{
    while( !shouldQuit() )
    {
        int dists[ LIDARSENSOR_DISTANCES ];
        memset( dists, 0x00, sizeof( dists ) );
        int pos = 0;
        int len = 0;

        while( isRunning() )
        {
            short distance = 0;
            ap_i2c->Write( sc_lidarConfigRegister, 0x04 ); // Initiate measurement
            usleep( 40000 ); // Delay for measurement to be taken
            ap_i2c->Read( sc_lidarDistanceRegister, 2, ( uint8_t* )( &distance ) ); // Read in measurement
            usleep( 5000 ); // Delay to prevent over polling

            dists[ pos ] = distance;
            pos = ( pos + 1 ) % LIDARSENSOR_DISTANCES;
            len = ( len < LIDARSENSOR_DISTANCES ) ? len + 1 : LIDARSENSOR_DISTANCES;

            int smooth_distance = 0;
            for( int i = 0; i < len; i++ )
            {
                smooth_distance += dists[ ( pos + i ) % LIDARSENSOR_DISTANCES ];
            }

            smooth_distance /= len;

            int sleep_time = 0;
            pthread_mutex_lock( &a_mutex );
            sleep_time = a_sleepTime;
            a_distance = distance;
            a_smoothDistance = smooth_distance;
            pthread_mutex_unlock( &a_mutex );

            usleep( sleep_time );
        }

        if( shouldQuit() ) break;

        usleep( 100000 );
    }
}

