#ifndef __TOGGLEINSERT_H__
#define __TOGGLEINSERT_H__

#include "io/inserts/WsIOInsert.h"

class ToggleInsert : public WsIOInsert
{
    public:
        ToggleInsert( void );
        virtual ~ToggleInsert( void );

        virtual bool process( bool value ) override;

        virtual std::string getName( void ) override;

    protected:
        bool a_prev;
        bool a_state;
};

#endif

