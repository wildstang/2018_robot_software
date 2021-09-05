#ifndef __WSTIMER_H__
#define __WSTIMER_H__

#include <stdint.h>

typedef enum
{
    WS_TIMER_STATE_NONE = 0,

    WS_TIMER_STATE_RUNNING,
    WS_TIMER_STATE_STOPPED,

    WS_TIMER_STATE_NUM
} wsTimerStateT;

/**
 *
 * @author Nathan
 */
class WsTimer
{
    public:
        WsTimer( void );
        ~WsTimer( void );

        double get( void );
        void reset( void );
        void start( void );
        void stop( void );

        inline bool isRunning( void )
            { return a_state == WS_TIMER_STATE_RUNNING; }

        // Returns true if the given period, in seconds, has elapsed
        bool hasPeriodPassed( double period );

    private:
        uint64_t a_resetTimestamp;
        uint64_t a_stopTimestamp;
        wsTimerStateT a_state;
};

#endif

