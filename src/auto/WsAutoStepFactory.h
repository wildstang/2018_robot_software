#ifndef __WSAUTOSTEPFACTORY_H__
#define __WSAUTOSTEPFACTORY_H__

#include "auto/script/AutoStepFactory.h"

class WsAutoStepFactory : public AutoStepFactory
{
    public:
        WsAutoStepFactory( void );
        virtual ~WsAutoStepFactory( void );

        virtual int createStepFactories( void ) override;
};

#endif

