#ifndef __AUTOSERIALSTEPGROUP_H__
#define __AUTOSERIALSTEPGROUP_H__

#include "auto/steps/AutoStepGroup.h"

#include <string>
#include <vector>

class AutoStepFactoryArgs;

class AutoSerialStepGroup : public AutoStepGroup
{
    // Serial groups execute all contained steps sequentially

    public:
        AutoSerialStepGroup( void );
        AutoSerialStepGroup( AutoStepFactoryArgs* p_args );
        virtual ~AutoSerialStepGroup( void );
        
        virtual void initialize( void );
        virtual void update( void );
        
        virtual std::string getClassName( void );
        
        AutoStep* getCurrentStep( void );
        AutoStep* getNextStep( void );

    protected:
        size_t a_currentStep;
        bool a_finishedPreviousStep;
};

#endif

