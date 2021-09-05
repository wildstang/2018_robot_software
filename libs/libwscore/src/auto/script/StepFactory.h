#ifndef __STEPFACTORY_H__
#define __STEPFACTORY_H__

#include <string>

#include "auto/script/AutoStepFactory.h"
#include "auto/script/AutoStepFactoryArgs.h"

class AutoStep;

#define WS_AUTO_STEP( step ) \
        { \
            std::string __name = std::string( #step ); \
            WS_ASSERT( ws_is_well_formed_name( __name.c_str() ) ); \
            this->addStepFactory( new __StepFactory< step >( __name ) ); \
        }

#define WS_AUTO_STEP_ALIAS( name, step ) \
        { \
            std::string __name = std::string( name ); \
            WS_ASSERT( ws_is_well_formed_name( __name.c_str() ) ); \
            this->addStepFactory( new __StepFactory< step >( __name ) ); \
        }


class StepFactory
{
    public:
        StepFactory( std::string name );
        virtual ~StepFactory( void );

        virtual AutoStep* createStep( AutoStepFactoryArgs* p_args ) = 0;

        inline std::string getName( void ) { return a_name; }

    protected:
        std::string a_name;
};

template< typename T >
class __StepFactory : public StepFactory
{
    public:
        __StepFactory( std::string name );
        virtual ~__StepFactory( void );

        virtual AutoStep* createStep( AutoStepFactoryArgs* p_args );
};

#define __STEPFACTORY_H__TEMPLATE__
#include "StepFactory.cpp"
#undef __STEPFACTORY_H__TEMPLATE__

#endif

