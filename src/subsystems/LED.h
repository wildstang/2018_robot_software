#ifndef __LED_H__
#define __LED_H__

#include "subsystems/Subsystem.h"
#include "io/Output.h"
#include "utils/WsTimer.h"

class Claw;

class LED : public Subsystem
{
    public:
        LED( void );
        virtual ~LED( void );

        virtual void init( void );
        virtual void resetState( void );
        virtual void update( void );
        virtual void selfTest( void );

        void setOutputs( int );
        int inputWantCube( bool );
        void setTeleop( bool );

    private:
        Claw* ap_claw;
        int a_state;
        int a_allianceColor;
        bool a_wantCubeButton;
        bool a_useRunningLights;
        bool a_inTeleop;
        bool a_ledTributeMode;

        bool a_pinOne;
        bool a_pinTwo;
        bool a_pinThree;
        WsTimer a_timer;
};

#endif

