#ifndef __INPUTLISTENER_H__
#define __INPUTLISTENER_H__

#include <vector>

class Input;

class InputListener
{
    public:
        InputListener( void );
        virtual ~InputListener( void );

        /**
         * Notifies the listener that an input event has occurred.
         *
         * @param source the Input that has updated
         */
        virtual void inputUpdate( Input* p_source, double value );
        virtual void inputUpdate( Input* p_source, bool value );
        virtual void inputUpdate( Input* p_source, int value );
        virtual void inputUpdate( Input* p_source, std::vector< unsigned char > value );
};

#endif

