#include "WsNetPacket.h"

#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "logger/WsLogger.h"

WsNetPacket::WsNetPacket( WsSocket* p_socket )
    : WsProtocol( p_socket )
    , a_header()
    , a_state( PACKET_FRAMING_STATE_WAIT_FOR_SYNC )
{
}

WsNetPacket::WsNetPacket( int port, size_t buffer_size )
    : WsProtocol( port, buffer_size )
    , a_header()
    , a_state( PACKET_FRAMING_STATE_WAIT_FOR_SYNC )
{
}

WsNetPacket::~WsNetPacket( void )
{
}

bool WsNetPacket::validateHeader( unsigned short crc )
{
    return ( ( a_header.sync == SYNC_CODEWORD ) &&
             ( this->crc( ( unsigned short* )( &a_header ),
                 sizeof( a_header ) / sizeof( short ) ) == crc ) );
}

unsigned short WsNetPacket::crc( unsigned short* buffer, size_t word_length )
{
    // SWH TODO compute the internet crc16 here.
    return 0;
}

size_t WsNetPacket::handleReceive( char* buf, size_t bytes )
{
    size_t bytes_read = 0;

    if( a_state == PACKET_FRAMING_STATE_WAIT_FOR_SYNC )
    {
        //
        // hunt for the first byte
        //

        unsigned short sync = SYNC_CODEWORD;
        sync = htons( sync );
        unsigned char* byte_list = ( unsigned char* )( &sync );
        for( size_t i = 0; i < bytes; ++i )
        {
            if( buf[ i ] == byte_list[ 0 ] ) 
            {
                if( ( i + 1 ) < bytes )
                {
                    if( buf[ i + 1 ] == byte_list[ 1 ] )
                    {
                        bytes_read = i;
                        a_state = PACKET_FRAMING_STATE_WAIT_FOR_HEADER;
                        break;
                    }
                    // otherwise, this is not a real sync.
                }
                else
                {
                    bytes_read = i;
                    // do not set the state.
                    // the second byte is not in the read buffer.
                    // move it to the front and try again next time.
                }
            }
        }
    }
    else if( a_state == PACKET_FRAMING_STATE_WAIT_FOR_HEADER )
    {
        if( bytes >= sizeof( a_header ) )
        {
            memcpy( &a_header, buf, sizeof( a_header ) );
            changeHeaderToHostOrder();
            unsigned short crc = a_header.crc;
            bool valid_header = validateHeader( crc );

            if( valid_header )
            {
                a_state = PACKET_FRAMING_STATE_WAIT_FOR_DATA;
                bytes_read = sizeof( a_header );
            }
            else
            {
                a_state = PACKET_FRAMING_STATE_WAIT_FOR_SYNC;
                bytes_read = 1;
            }
        }
    }
    else if( a_state == PACKET_FRAMING_STATE_WAIT_FOR_DATA )
    {
        int err = this->handlePacket( &a_header, buf );
        if( err )
        {
            bytes_read = 1;
            a_state = PACKET_FRAMING_STATE_WAIT_FOR_SYNC;
        }
        else
        {
            bytes_read = a_header.message_size;
            a_state = PACKET_FRAMING_STATE_WAIT_FOR_HEADER;
        }
    }
    else
    {
        WS_INFO( "reset state for packet framing server" );
        a_state = PACKET_FRAMING_STATE_WAIT_FOR_SYNC;
    }

    return bytes_read;
}

void WsNetPacket::changeHeaderToHostOrder( void )
{
    a_header.sync = ntohs( a_header.sync );
    a_header.crc = ntohs( a_header.crc );
    a_header.message_size = ntohl( a_header.message_size );
    a_header.message_code = ntohl( a_header.message_code );
}

void WsNetPacket::changeHeaderToNetworkOrder( void )
{
    a_header.sync = htons( a_header.sync );
    a_header.crc = htons( a_header.crc );
    a_header.message_size = htonl( a_header.message_size );
    a_header.message_code = htonl( a_header.message_code );
}

void WsNetPacket::prepHeaderToSend( void )
{
    a_header.crc = crc( ( unsigned short* )( &a_header ),
                        sizeof( a_header ) / sizeof( unsigned short ) );
    changeHeaderToNetworkOrder();
}

