#ifndef __AUTOPARALLELSTEPGROUP_H__
#define __AUTOPARALLELSTEPGROUP_H__

#include "auto/steps/AutoStepGroup.h"

#include <string>
#include <set>

class AutoStepFactoryArgs;

class AutoParallelStepGroup : public AutoStepGroup
{
    // Parallel groups execute all contained steps in the same frame. Be
    // careful!
    // Note: a finished step is immediately removed from the list. update() is
    // not called on any step that finishes.

    public:
        AutoParallelStepGroup( void );
        AutoParallelStepGroup( AutoStepFactoryArgs* p_args );
        virtual ~AutoParallelStepGroup( void );
        
        virtual void initialize( void );
        virtual void update( void );
        
        virtual std::string getClassName( void );

    protected:
        std::set< AutoStep* > a_remaining;
};

#endif

