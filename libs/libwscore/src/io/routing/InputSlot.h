#ifndef __INPUTSLOT_H__
#define __INPUTSLOT_H__

#include <set>
#include <vector>

#include "InputListener.h"

#define CONNECT_INPUT( name, function, sub, type ) \
        { \
            InputManager* __im = RobotCore::getRobotCore()->getInputManager(); \
            Input* __input = __im->getInput( name ); \
            WS_INFO( "connecting input %s to %s.%s( %s )", \
                     name, this->getName().c_str(), #function, #type ); \
            if( __input != NULL ) \
            { \
                InputSlot* __slot = \
                    new FunctionInputSlot< sub, type >( \
                        this, &sub::function, __input ); \
                __im->getSlotManager()->addSlot( __slot ); \
            } \
            else \
            { \
                WS_ERROR( "input %s doesn't exist.", name ); \
            } \
        } 

class InputSlot : public InputListener
{
    public:
        InputSlot( Input* p_input );
        virtual ~InputSlot( void );

        virtual void inputUpdate( Input* p_source, double value ) override;
        virtual void inputUpdate( Input* p_source, bool value ) override;
        virtual void inputUpdate( Input* p_source, int value ) override;
        virtual void inputUpdate( Input* p_source, std::vector< unsigned char > value ) override;

        virtual void inputReceive( const double& rc_value );
        virtual void inputReceive( const bool& rc_value );
        virtual void inputReceive( const int& rc_value );
        virtual void inputReceive( const std::vector< unsigned char >& rc_value );

    protected:
        Input* ap_input;
};

class InputSlotManager
{
    public:
        InputSlotManager( void );
        virtual ~InputSlotManager( void );

        void addSlot( InputSlot* p_slot );
        void deletingSlot( InputSlot* p_slot );

    protected:
        std::set< InputSlot* > a_slots;
        bool a_destructionMode;
};

template< typename S, typename T >
class FunctionInputSlot : public InputSlot
{
    public:
        FunctionInputSlot( S* p_subsystem, 
                           int ( S::*p_function )( T value ),
                           Input* p_input );
        virtual ~FunctionInputSlot( void );

        virtual void inputReceive( const T& rc_value ) override;

    protected:
        S* ap_subsystem;
        int ( S::*ap_inputSlotFunction )( T value );
};

#define __INPUTSLOT_H__TEMPLATE__
#include "InputSlot.cpp"
#undef __INPUTSLOT_H__TEMPLATE__

#endif

