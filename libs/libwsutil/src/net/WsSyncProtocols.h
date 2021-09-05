#ifndef __WSSYNCPROTOCOLS_H__
#define __WSSYNCPROTOCOLS_H__

#include "net/WsProtocol.h"
#include "net/WsNetConsole.h"
#include "net/WsNetPacket.h"

#include <string>
#include <deque>

class WsSynchronizedProtocol : public WsProtocol
{
    public:
        WsSynchronizedProtocol( int port, size_t buffer_size );
        virtual ~WsSynchronizedProtocol( void );

        int update( void );

    protected:
        virtual size_t handleReceive( char* buf, size_t bytes ) override;

        virtual size_t syncHandleReceive( char* buf, size_t bytes ) = 0;

    private:
        pthread_mutex_t a_mutex;
        char* ap_buffer;
        size_t a_bytes;
        size_t a_bufferSize;
};

class WsSynchronizedNetConsole : public WsNetConsole
{
    public:
        WsSynchronizedNetConsole( int port );
        virtual ~WsSynchronizedNetConsole( void );

        int update( void );

    protected:
        virtual int handleLine( const char* line );

        virtual int syncHandleLine( const char* line ) = 0;

    private:
        pthread_mutex_t a_mutex;
        pthread_cond_t a_cond;
        std::string a_line;
        bool a_hasData;
        int a_ret;
};

typedef struct
{
    packetFramingHeaderT header;
    const char* buffer;
} syncPacketFramingDataT;

class WsSynchronizedNetPacket : public WsNetPacket
{
    public:
        WsSynchronizedNetPacket( int port, size_t buffer_size );
        virtual ~WsSynchronizedNetPacket( void );

        int update( void );

    protected:
        virtual int handlePacket( packetFramingHeaderT* p_header,
                                  const char* buffer );

        virtual int syncHandlePacket( packetFramingHeaderT* p_header,
                                      const char* buffer ) = 0;

    private:
        pthread_mutex_t a_mutex;
        std::deque< syncPacketFramingDataT > a_frames;
};

#endif

