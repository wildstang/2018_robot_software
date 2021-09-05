#ifndef __OUTPUTMANAGER_H__
#define __OUTPUTMANAGER_H__

#include <map>
#include <string>

#include "core/WildstangModule.h"

class Output;
class RobotCore;

/**
 * This class in the manager for all outputs.
 *
 * @author Steve
 *
 */
class OutputManager : public WildstangModule
{
    public:
        OutputManager( RobotCore* p_robot );
        ~OutputManager( void );
        
        void update( void );
        void init( void );

        void addOutput( Output* p_output );
        Output* getOutput( std::string name );

        std::vector< std::string > listOutputs( void );

    private:
        std::map< std::string, Output* > a_outputs;
        RobotCore* ap_robot;
};

#endif

