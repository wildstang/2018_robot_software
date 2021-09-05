#ifndef __SLEEPER_H__
#define __SLEEPER_H__

#include "auto/AutoProgram.h"

class SleeperProgram : public AutoProgram
{
    public:
        SleeperProgram( void );
        virtual ~SleeperProgram( void );

        virtual void defineSteps( void ) override;
};

#endif

