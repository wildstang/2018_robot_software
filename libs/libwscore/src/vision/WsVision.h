#ifndef __WSVISION_H__
#define __WSVISION_H__

#include <pthread.h>
#include <string>
#include <stdint.h>

#include "core/WildstangModule.h"
#include "net/WsNetConsole.h"

#define WS_VISION_H_MIN     80
#define WS_VISION_S_MIN     0
#define WS_VISION_V_MIN     0
#define WS_VISION_H_MAX     135
#define WS_VISION_S_MAX     180
#define WS_VISION_V_MAX     180

#define WS_VISION_X_OFFSET  400
#define WS_VISION_THRESHOLD 10

#define WS_VISION_PORT      5800

#define CORRECTION_LEVEL_INDEX 0
#define DISTANCE_INDEX         1
#define SET_INDEX              2
#define IMAGE_INDEX            3

class WsVision : public WildstangModule, public WsNetConsole
{
    public:
        WsVision( void );
        ~WsVision( void );

        void enableVideoLogging( void );
        void disableVideoLogging( void );

        double getXCorrectionLevel( void );
        double getDistance( void );

    protected:
        virtual int handleLine( const char* line );

    private:
        int a_hMin;
        int a_sMin;
        int a_vMin;
        int a_hMax;
        int a_sMax;
        int a_vMax;
        int a_center;
        int a_threshold;
        double a_blurRadius;

        double a_xCorrectionLevel;
        double a_distance;
        
        static const int sc_hMinDefault;
        static const int sc_sMinDefault;
        static const int sc_vMinDefault;
        static const int sc_hMaxDefault;
        static const int sc_sMaxDefault;
        static const int sc_vMaxDefault;
        static const int sc_centerDefault;
        static const int sc_thresholdDefault;
        static const double sc_blurRadiusDefault;

        void sendPreferences( void );
};

#endif

