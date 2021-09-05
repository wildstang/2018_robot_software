#ifndef __TARGET_H__
#define __TARGET_H__

#if defined(TARGET_FRC_CRIO_WPILIB)

#include "IterativeRobot.h"
#include "CrioHardware.h"
#include "WsIterativeRobot.h"

#define WS_START_ROBOT_CLASS( class_name ) \
    START_ROBOT_CLASS( WsIterativeRobot< class_name > )
#define HARDWARE_CLASS CrioHardware

#elif defined(TARGET_SIMULATION)

#include "WsSimulation.h"
#include "SimulationHardware.h"
#include <cstdio>
#define WS_START_ROBOT_CLASS( class_name ) \
    int main( int argc, char** argv ) \
    { \
        printf( "program is running\n" ); \
        WsSimulation s( new class_name() ); \
        if( argc > 1 ) s.disablePrints(); \
        s.run(); \
        return 0; \
    }
#define HARDWARE_CLASS SimulationHardware

#else

#error "no target defined."

#endif

#endif

