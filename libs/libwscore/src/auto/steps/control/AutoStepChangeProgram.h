#ifndef __AUTOSTEPCHANGEPROGRAM_H__
#define __AUTOSTEPCHANGEPROGRAM_H__

#include "auto/steps/AutoStep.h"

#include "utils/WsTimer.h"

class AutoStepFactoryArgs;

class AutoStepChangeProgram : public AutoStep
{
    public:        
        AutoStepChangeProgram( std::string program_name, bool reversed );
        AutoStepChangeProgram( AutoStepFactoryArgs* p_args );
        virtual ~AutoStepChangeProgram( void );
        
        virtual void initialize( void ) override;
        virtual void update( void ) override;
        
        virtual std::string getClassName( void ) override;
        virtual AutoStepFactoryArgs getArguments( void );

    protected:
        std::string a_programName;
        bool a_reversed;
};

#endif

