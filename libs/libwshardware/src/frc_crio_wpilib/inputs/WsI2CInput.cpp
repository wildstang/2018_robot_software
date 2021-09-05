#include "WsI2CInput.h"

#include "config/ConfigValueRef.h"

#include "crio_utils.h"

std::vector< unsigned char > WsI2CInput::s_empty;

// SWH TODO:
//
// See hash 34348e6ee81a5aa76b9b9c2c7ec1dd5a672b0df4
//
// moved the reads to a background thread. That's probably okay.

WsI2CInput::WsI2CInput( std::string name, enum frc::I2C::Port port, 
                        int address, int size )
    : BinaryInput( name, s_empty )
    , ap_i2c( NULL )
    , a_size( size )
    , a_port( port )
    , a_address( address )
    , ac_factoryConstructor( false )
    , a_portStr()
{
}

WsI2CInput::WsI2CInput( std::string name )
    : BinaryInput( name, s_empty )
    , ap_i2c( NULL )
    , a_size( 0 )
    , a_port( frc::I2C::kOnboard )
    , a_address( 0 )
    , ac_factoryConstructor( true )
    , a_portStr()
{
    CONFIG_VAR( "port", a_portStr, std::string );
    CONFIG_VAR( "address", a_address, int );
    CONFIG_VAR( "size", a_size, int );
}

WsI2CInput::~WsI2CInput( void )
{
    if( ap_i2c != NULL )
    {
        delete ap_i2c;
        ap_i2c = NULL;
    }
}

int WsI2CInput::instantiate( void )
{
    if( ac_factoryConstructor )
    {
        a_port = parse_i2c_port( a_portStr );
    }

    if( ap_i2c != NULL ) delete ap_i2c;
    ap_i2c = new frc::I2C( a_port, a_address );
    return 0;
}

std::vector< unsigned char > WsI2CInput::readRawValue( void )
{
    std::vector< unsigned char > v;
    v.resize( a_size );
    unsigned char* p_arr = &( v[ 0 ] );

    bool ret = ap_i2c->ReadOnly( a_size, p_arr );
    if( !ret )
    {
        WS_ERROR( "could not read from i2c %s", this->getName().c_str() );
    }

    return v;
}

//   private static class MessageHandler implements Runnable
//   {
//      // Offload to a thread avoid blocking main thread with I2C sends.
//
//      static byte[] rcvBytes;
//      byte[] sendData;
//      int sendSize = 0;
//      boolean running = true;
//      boolean dataToSend = false;
//      private I2C i2c;
//
//      public MessageHandler(I2C p_i2c)
//      {
//         // Get ourselves an i2c instance to send out some data.
//         i2c = p_i2c;
//      }
//
//      @Override
//      public void run()
//      {
//         while (running)
//         {
//            synchronized (this)
//            {
//               try
//               {
//                  // blocking sleep until someone calls notify.
//                  this.wait();
//                  // Need at least a byte and someone has to have called
//                  // setSendData.
//                  if (sendSize >= 0 && dataToSend)
//                  {
//                     // set receive size to 0 to avoid sending an i2c read
//                     // request.
//                     i2c.writeBulk(sendData);// transaction(sendData, sendSize,
//                                             // rcvBytes, 0);
//                     dataToSend = false;
//                  }
//               }
//               catch (InterruptedException e)
//               {
//               }
//            }
//         }
//      }
//
//      public void setSendData(byte[] data, int size)
//      {
//         sendData = data;
//         sendSize = size;
//         dataToSend = true;
//      }
//
//      public void stop()
//      {
//         running = false;
//      }
//   }

