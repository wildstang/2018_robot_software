#ifndef __AUTOPARALLELSHORTSTEPGROUP_H__
#define __AUTOPARALLELSHORTSTEPGROUP_H__

#include <string>
#include <vector>

#include "auto/steps/AutoStepGroup.h"

class AutoStepFactoryArgs;

class AutoParallelShortStepGroup : public AutoStepGroup
{
    public:
        AutoParallelShortStepGroup( void );
        AutoParallelShortStepGroup( AutoStepFactoryArgs* p_args );
        virtual ~AutoParallelShortStepGroup( void );
        
        virtual void initialize( void );
        virtual void update( void );
        
        virtual std::string getClassName( void );
};

#endif

