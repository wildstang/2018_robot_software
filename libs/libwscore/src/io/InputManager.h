#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__

#include <map>
#include <string>

#include "core/WildstangModule.h"
#include "io/routing/InputSlot.h"

class Input;
class InputListener;
class RobotCore;

/**
 * This class in the manager for all inputs.
 *
 * @author Steve
 *
 */
class InputManager : public WildstangModule
{
    public:
        InputManager( RobotCore* p_robot );
        ~InputManager( void );
        
        void update( void );
        void init( void );

        void addInput( Input* p_input );
        Input* getInput( std::string name );
        inline InputSlotManager* getSlotManager( void )
            { return &a_slotManager; }

        int becomeListener( std::string input, InputListener* p_listener );

        std::vector< std::string > listInputs( void );
        
    private:
        std::map< std::string, Input* > a_inputs;
        RobotCore* ap_robot;
        InputSlotManager a_slotManager;
};

#endif

