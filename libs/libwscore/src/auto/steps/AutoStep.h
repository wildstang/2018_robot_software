#ifndef __AUTOSTEP_H__
#define __AUTOSTEP_H__

#include <string>

#include "io/routing/InputListener.h"
#include "auto/script/AutoStepFactoryArgs.h"

#define AUTO_STEP_GROUP_TABSTOP 4

class AutoStepGroup;

class AutoStep : public InputListener
{
    // TODO: fix this
    friend class AutoStepGroup;

    public:
        AutoStep( void );
        virtual ~AutoStep( void );
        
        virtual void initialize( void ) = 0;
        // This method is called once, when the
        // step is first run. Use this method to
        // set up anything that is necessary for
        // the step.
        
        virtual void update( void ) = 0;
        // This method is called on the active step,
        // once per call to
        // RobotTemplate.autonomousPeriodic().
        // Steps will continue to have this method called until they set finished to
        // true.
        // Note: this method is first called right after initialize(), with no delay
        // in between.
        
        bool isFinished( void );
        void setFinished( bool is_finished );
        
        //
        // When you implement this, please be honest.
        // You're only hurting yourself if you aren't.
        //
        // Will find a better way to do this later.
        //
        virtual std::string getClassName( void ) = 0;

        virtual std::string generateCode( void );
        virtual AutoStepFactoryArgs getArguments( void );

        virtual AutoStepGroup* asGroup( void );

    protected:
        inline void setDepth( int d ) { a_depth = d; }
        inline int getDepth( void ) { return a_depth; }

    private:
        bool a_finished;
        int a_depth;
};

#endif

