#ifndef __SUBSYSTEMMANAGER_H__
#define __SUBSYSTEMMANAGER_H__

#include <vector>

#include "core/WildstangModule.h"

class Subsystem;

/**
 * This class in the manager for all outputs.
 *
 * @author Steve
 *
 */
class SubsystemManager : public WildstangModule
{
    public:
        SubsystemManager( void );
        virtual ~SubsystemManager( void );

        void resetState( void );
        void init( void );
        void selfTest( void );

        void update( void );

        void addSubsystem( Subsystem* p_subsystem );

    private:
        std::vector< Subsystem* > a_subsystems;
};

#endif

