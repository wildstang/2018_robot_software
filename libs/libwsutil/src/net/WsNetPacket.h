#ifndef __WSNETPACKET_H__
#define __WSNETPACKET_H__

#include "WsProtocol.h"

#define SYNC_CODEWORD 0xBEEF

typedef enum
{
    PACKET_FRAMING_STATE_NONE = 0,
    
    PACKET_FRAMING_STATE_WAIT_FOR_SYNC,
    PACKET_FRAMING_STATE_WAIT_FOR_HEADER,
    PACKET_FRAMING_STATE_WAIT_FOR_DATA,

    PACKET_FRAMING_STATE_NUM
} packetFramingStateT;

typedef struct
{
    unsigned short sync;
    unsigned short crc;
    unsigned int message_size;
    unsigned int message_code;
} packetFramingHeaderT;

//
// Network header starts with the secret codeword 0xBEEF
// Search for 0xBEEF in the buffer, and then you can read
// in a network header aligned with that. You then check
// the CRC on the header to see that it is a valid header
// and not just trash that you found in the data stream.
//

class WsNetPacket : public WsProtocol
{
    public:
        WsNetPacket( WsSocket* p_socket );
        WsNetPacket( int port, size_t buffer_size );
        virtual ~WsNetPacket( void );

        //
        // if you directly inherit from WsNetPacket
        // you are expected to implement your own mutex.
        //

    protected:
        virtual size_t handleReceive( char* buf, size_t bytes ) override;

        virtual int handlePacket( packetFramingHeaderT* p_header,
                                  const char* buffer ) = 0;

    private:
        packetFramingHeaderT a_header;
        packetFramingStateT a_state;

        bool validateHeader( unsigned short crc );
        void changeHeaderToHostOrder( void );
        void changeHeaderToNetworkOrder( void );
        void prepHeaderToSend( void );
        unsigned short crc( unsigned short* buffer, size_t word_length );
};

#endif

