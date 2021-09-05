#ifndef __SOCKETWORKER_H__
#define __SOCKETWORKER_H__

#include <pthread.h>
#include <vector>

class WsMJPEGStreamer;

class SocketWorker
{
    public:
        SocketWorker( int counter, int socket_fd, WsMJPEGStreamer* ims );
        ~SocketWorker( void );

        void go( void );
        void stop( void );

        void send( std::vector< unsigned char >& r_v );
        
    private:
        static void* entry( void* arg );
        void writeHeader( void );
        void run( void );
        void close_connection( void );
        
    private:
        static const int sc_SEND_BUFF_SIZE;
        int a_fps;
        WsMJPEGStreamer* ap_ims;
        int a_counter;
        int a_sockFd;
        bool a_running;
        std::vector< unsigned char > a_latestImage;

        pthread_t a_thread;
        pthread_mutex_t a_mutex;
};

#endif

