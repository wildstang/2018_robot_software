#ifndef __DEBOUNCEINSERT_H__
#define __DEBOUNCEINSERT_H__

#include "io/inserts/WsIOInsert.h"

class DebounceInsert : public WsIOInsert
{
    public:
        DebounceInsert( void );
        DebounceInsert( int debounce_cycles );
        virtual ~DebounceInsert( void );

        virtual bool process( bool value ) override;

        virtual std::string getName( void ) override;

    protected:
        bool a_currentValue;
        int a_cyclesOnCurrentValue;
        bool a_lastValue;
        int a_debounceCycles;
};

#endif

