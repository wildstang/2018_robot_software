#ifndef __LIDARSENSOR_H__
#define __LIDARSENSOR_H__

#include <vector>
#include <pthread.h>

#include "I2C.h"

class LidarSensor
{
    public:
        LidarSensor( enum frc::I2C::Port port, int address );
        ~LidarSensor( void ); 

        //
        // Distance in cm
        //

        int getDistance( void );
        
        //
        // Returns average of the last 5 readings
        //

        int getSmoothedDistance( void );
        
        double pidGet( void );
        
        void start( int sleep = 100 );
        void stop( void );
        
    protected:
        void quit( void );
        bool shouldQuit( void );
        bool isRunning( void );

    private:
        static void* run( void* arg );
        void run( void );

        frc::I2C* ap_i2c;
        int a_distance;
        int a_smoothDistance;

        pthread_t a_thread;
        pthread_mutex_t a_mutex;

        bool a_running;
        bool a_quit;
        int a_sleepTime;
        
        static const int sc_lidarConfigRegister;
        static const int sc_lidarDistanceRegister;
};

#endif

