#ifndef __SAFETYINSERT_H__
#define __SAFETYINSERT_H__

#include "io/inserts/WsIOInsert.h"

class SafetyInsert : public WsIOInsert
{
    public:
        SafetyInsert( void );
        SafetyInsert( uint64_t timeRequired );
        virtual ~SafetyInsert( void );

        virtual bool process( bool value ) override;

        virtual std::string getName( void ) override;

    protected:
        bool a_currentValue;
        bool a_lastValue;
        uint64_t a_timeActivated;
        uint64_t a_timeRequired;
};

#endif

