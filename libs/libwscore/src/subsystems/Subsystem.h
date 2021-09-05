#ifndef __SUBSYSTEM_H__
#define __SUBSYSTEM_H__

//
// Wpilib is the worst
//
//#define NAMESPACED_WPILIB 1 
//
// now in makefiles
//

#include "core/WildstangModule.h"
#include "core/RobotCore.h"

#include <string>

class Subsystem : public WildstangModule
{
    public:
        Subsystem( std::string name );
        virtual ~Subsystem( void );

        /**
         * Initialise the subsystem.  Performs any required setup work.
         */
        virtual void init( void ) = 0;

        /**
         * Performs a self test of the subsystem.
         */
        virtual void selfTest( void ) = 0;

        /**
         * Called to cause the subsystem to update its state and set new values on outputs.
         *
         * Sending values to the hardware outputs is done outside of this method by the framework.
         */
        virtual void update( void ) = 0;

        /**
         * Returns the name of the subsystem.
         *
         * @return the name of the subsystem
         */
        std::string getName( void );

        /**
         * Can be called to reset any state variables.  Can be used when changing modes or
         * reenabling system to reset to a default state without reinitialising connected
         * components.
         */
        virtual void resetState( void ) = 0; 

    private:
        std::string a_name;
};

#endif

