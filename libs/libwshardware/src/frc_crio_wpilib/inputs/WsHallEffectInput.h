#ifndef __WSHALLEFFECTINPUT_H__
#define __WSHALLEFFECTINPUT_H__

#include "io/inputs/InputStubs.h"

#include <pthread.h>

#include "I2C.h"

class WsHallEffectInput : public DiscreteInput
{
    public:
        WsHallEffectInput( std::string name, frc::I2C::Port port, int address );
        WsHallEffectInput( std::string name );
        virtual ~WsHallEffectInput( void );
        
        void start( int sleep = 100 );
        void stop( void );
        
    protected:
        virtual int readRawValue( void ) override;

        virtual int instantiate( void ) override;

        bool shouldQuit( void );
        void quit( void );
        bool isRunning( void );
        
    private:
        static void* run( void* arg );
        void run( void );

        frc::I2C* ap_i2c;

        int a_lastHallEffectSensor;
        int a_selectedHallEffectSensor;

        bool a_running;
        bool a_quit;
        int a_sleepTime;
        pthread_t a_thread;
        pthread_mutex_t a_mutex;

        frc::I2C::Port a_port;
        std::string a_portStr;
        int a_address;
        const bool ac_factoryConstructor;
        int a_pollRate;
};

#endif

