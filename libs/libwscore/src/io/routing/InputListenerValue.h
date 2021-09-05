#ifndef __INPUTLISTENERVALUE_H__
#define __INPUTLISTENERVALUE_H__

#include "io/routing/InputListener.h"

template< typename T >
class InputListenerValue : public InputListener
{
    public:
        InputListenerValue( T* p_value_ref );
        virtual ~InputListenerValue( void );

        virtual void inputUpdate( Input* p_source, T value ) override;

    private:
        T* ap_valueRef;
};

#define __INPUTLISTENERVALUE_H__TEMPLATE__
#include "InputListenerValue.cpp"
#undef __INPUTLISTENERVALUE_H__TEMPLATE__

#endif

