#ifndef __MONITOR_H__
#define __MONITOR_H__

#include "subsystems/Subsystem.h"

class Monitor : public Subsystem
{
    public:
        Monitor( void );
        virtual ~Monitor( void );
        
        virtual void init( void );
        virtual void resetState( void );
        virtual void update( void );
        virtual void selfTest( void );
        
    private:
        unsigned int a_length;
        
        static const char* const sc_names[];
};

#endif

