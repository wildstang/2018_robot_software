#ifndef __AUTOSTEPGROUP_H__
#define __AUTOSTEPGROUP_H__

#include <vector>

#include "auto/steps/AutoStep.h"

class AutoStepGroup : public AutoStep
{
    public:
        AutoStepGroup( void );
        virtual ~AutoStepGroup( void );

        void addStep( AutoStep* p_step );

        int parseCode( const std::string& rc_code );
        int parseCode( std::deque< std::string >& r_tok );

        virtual std::string generateCode( void ) override;

        virtual AutoStepGroup* asGroup( void );

    protected:
        std::vector< AutoStep* > a_steps;
};

#endif

