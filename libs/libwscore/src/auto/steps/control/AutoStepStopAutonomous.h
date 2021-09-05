#ifndef __AUTOSTEPSTOPAUTONOMOUS_H__
#define __AUTOSTEPSTOPAUTONOMOUS_H__

#include "auto/steps/AutoStep.h"

class AutoStepFactoryArgs;

class AutoStepStopAutonomous : public AutoStep
{
    // Do nothing. This step does nothing, and never finishes, effectively
    // halting autonomous operations.
    // Note: If included in a parallel step group, it only halts operations
    // after all other steps in the group finish.

    public:
        AutoStepStopAutonomous( void );
        AutoStepStopAutonomous( AutoStepFactoryArgs* p_args );
        virtual ~AutoStepStopAutonomous( void );
        
        virtual void initialize( void ) override;
        virtual void update( void ) override;
        
        virtual std::string getClassName( void ) override;
        virtual AutoStepFactoryArgs getArguments( void ) override;
};

#endif

