#ifndef __WSI2COUTPUT_H__
#define __WSI2COUTPUT_H__

#include "io/outputs/OutputStubs.h"

#include <string>

#include <pthread.h>
#include <vector>

#include "I2C.h"

#define RESIZE_BUFFER_SIZE 64

class WsI2COutput : public BinaryOutput
{
    public:
        WsI2COutput( std::string name, enum frc::I2C::Port port, int address );
        WsI2COutput( std::string name );
        virtual ~WsI2COutput( void );

        virtual int instantiate( void );
        
    protected:
        virtual void sendDataToOutput( void );

    private:
        frc::I2C* ap_i2c;
        pthread_t a_thread;
        pthread_mutex_t a_mutex;
        pthread_cond_t a_waitCondition;
        bool a_running;
        bool a_dataToSend;
        std::vector< unsigned char > a_bytesReceived;
        std::vector< unsigned char > a_bytesToSend;

        enum frc::I2C::Port a_port;
        std::string a_portStr;
        const bool ac_factoryConstructor;
        int a_address;

        static void* threadEntry( void* arg );
        void instanceThreadEntry( void );

        void setSendData( std::vector< unsigned char >& r_data );
                
        void stop( void );
        bool isRunning( void );

        static std::vector< unsigned char > s_empty;
};

#endif

