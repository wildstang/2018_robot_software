#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__

class StopWatch
{
    public:
        StopWatch( void );
        ~StopWatch( void );

        void Reset( void );
        void Start( void );
        void Stop( void );
        double GetTimeInSec( void );

    private:
        double a_totalTimeInSec;
        long a_startTimeInNanoSec;
        long a_totalTimeInNanoSec;
        bool a_timerRunning;
};

#endif

