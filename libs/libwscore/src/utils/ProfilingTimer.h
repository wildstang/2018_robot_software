#ifndef __PROFILINGTIMER_H__
#define __PROFILINGTIMER_H__

#include <string>

class ProfilingTimer
{
    public:
        ProfilingTimer( std::string name, int iterations );
        ~ProfilingTimer( void );

        void startTimingSection( void );
        double endTimingSection( void );

    private:
        double a_startingTime;
        double a_endingTime;
        int a_iterations;
        std::string a_name;
};

#endif

