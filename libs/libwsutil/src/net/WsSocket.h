#ifndef __WSSOCKET_H__
#define __WSSOCKET_H__

#include <string>
#include <vector>
#include <set>
#include <cstdarg>
#include <pthread.h>

class WsProtocol;

class WsSocket
{
    public:
        WsSocket( int port, size_t buffer_size, WsProtocol* p_protocol );
        virtual ~WsSocket( void );

        inline bool isConnected( void ) { return a_connected; }
        int sendBytes( const char* buf, size_t bytes );

        int startSocket( void );

    protected:
        size_t handleReceive( char* buf, size_t bytes );

        inline int getPort( void ) { return a_port; }
        inline bool isRunning( void ) { return a_running; }

        inline void halt( void ) { a_running = false; }

        virtual int wsConnect( void ) = 0;
        int runConnection( int fd );

        //
        // Note you must call this function to run the thread
        // and listen/connect
        //

    private:
        bool a_running;
        int a_port;
        size_t a_bufferSize;
        int a_connectionFd;
        pthread_t a_thread;
        pthread_mutex_t a_mutex;
        bool a_connected;

        WsProtocol* ap_protocol;

        static void* __run( void* arg );
        void __run( void );
};

class WsServerSocket : public WsSocket
{
    public:
        WsServerSocket( int port, size_t buffer_size, WsProtocol* p_protocol );
        virtual ~WsServerSocket( void );

    protected:
        virtual int wsConnect( void );
};

class WsClientSocket : public WsSocket
{
    public:
        WsClientSocket( int port, size_t buffer_size, 
                        std::string host, WsProtocol* p_protocol );
        virtual ~WsClientSocket( void );

    protected:
        virtual int wsConnect( void );

        std::string a_host;
};

#endif

