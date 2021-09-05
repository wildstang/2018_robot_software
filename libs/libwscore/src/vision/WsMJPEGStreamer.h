#ifndef __WSMJPEGSTREAMER_H__
#define __WSMJPEGSTREAMER_H__

#include "subsystems/Subsystem.h"

#include <string>
#include <vector>
#include <set>

class Mat;
class StopWatch;
class SocketWorker;

class WsMJPEGStreamer : public Subsystem
{
    public:
        WsMJPEGStreamer( void );
        virtual ~WsMJPEGStreamer( void );

        virtual void init( void );
        virtual void selfTest( void );
        virtual void update( void );
        virtual void resetState( void );

        static const int sc_FPS;
        static const int sc_PORT;

        void finish( SocketWorker* p_sw );

    private:
        void send( Mat* img );
        int getPort( void );

        static void* run( void* arg );
        void run( void );

        void flushPendingCleanupQueue( void );

        bool a_running;
        StopWatch* ap_stopwatch;
        pthread_t a_thread;
        pthread_mutex_t a_mutex;
        std::set< SocketWorker* > a_socketWorkers;
        std::vector< SocketWorker* > a_pendingCleanup;
};

#endif

