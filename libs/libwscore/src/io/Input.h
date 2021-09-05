#ifndef __INPUT_H__
#define __INPUT_H__

#include <vector>
#include <string>

#include "core/WildstangModule.h"

class InputListener;
class InputManager;
class WsIOInsert;

/**
 * This is an abstract implementation of the Input interface. This class
 * implements the Input listener mechanism.
 *
 * Flow of an input:
 *
 * - First you have to add the input to the input manager.
 * - Then the input manager instantiates the input and it's ready to go.
 * - Then in the loop, update is called on the input. Then if the input
 *   is enabled, it reads the raw value from the input, and the child class
 *   is responsible for storing the value after it is read from the input.
 *   This is implemented in the InputTemplate class, and the InputTemplate
 *   also decides whether the input has changed from before.
 * - The input template will run the input value through all of the inserts
 *   that you have installed in the order you installed them.
 * - In the update loop, after the value has been read into the child class
 *   and the has changed flag is updated, the notifyListeners function is
 *   called. If the value has changed, then all of the input listeners are
 *   pushed the new value.
 *
 */
class Input : public WildstangModule
{
    public:
        Input( InputManager* p_manager, std::string name );
        virtual ~Input( void );

        void addInputListener( InputListener* p_listener );

        void instantiateInput( void );

        void update( void );
        void flush( void );

        inline void enable( void ) { a_enabled = true; }
        inline void disable( void ) { a_enabled = false; }
        inline bool isEnabled( void ) { return a_enabled; }

        Input* addInsert( WsIOInsert* p_insert );
        void clearInserts( void );

        inline std::string getName( void ) { return a_name; }

    protected:
        void notifyListeners( void );

        inline bool hasValueChanged( void ) { return a_valueChanged; }
        inline bool isInstantiated( void ) { return a_instantiated; }

        virtual void readDataFromInput( void ) = 0;
        virtual void notifyListener( InputListener* p_listener ) = 0;

        std::vector< WsIOInsert* > a_inserts;
        bool a_valueChanged;

        virtual int instantiate( void ) = 0;

    private:
        std::vector< InputListener* > a_listeners;

        std::string a_name;
        InputManager* ap_inputManager;

        bool a_enabled;
        bool a_instantiated;

        int a_instantiationWarningCount;
};

#endif

