#ifndef __WSRELAY_H__
#define __WSRELAY_H__

#include "io/outputs/OutputStubs.h"
#include "Relay.h"

typedef enum
{
    WS_RELAY_STATE_NONE = 0,

    WS_RELAY_STATE_ON,
    WS_RELAY_STATE_FORWARD,
    WS_RELAY_STATE_REVERSE,

    WS_RELAY_STATE_NUM
} wsRelayStateT;

class WsRelay : public DiscreteOutput
{
    public:        
        WsRelay( std::string name, int channel,
                 enum frc::Relay::Direction direction );
        WsRelay( std::string name );
        virtual ~WsRelay( void );

        virtual int instantiate( void );
        
    protected:
        virtual void sendDataToOutput( void );

    private:
        frc::Relay* ap_relay;
        int a_channel;
        enum frc::Relay::Direction a_direction;
        std::string a_directionStr;
        const bool ac_factoryConstructor;
};

#endif

