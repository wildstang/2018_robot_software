#ifndef __DRIVESETBRAKEMODESTEP_H__
#define __DRIVESETBRAKEMODESTEP_H__

#include "auto/steps/AutoStep.h"

class Drive;
class AutoStepFactoryArgs;

class DriveSetBrakeModeStep : public AutoStep
{
    public:
        DriveSetBrakeModeStep( bool brake );
        DriveSetBrakeModeStep( AutoStepFactoryArgs* p_args );
        virtual ~DriveSetBrakeModeStep( void );

        virtual void initialize( void );
        virtual void update( void );
        virtual std::string getClassName( void );
        virtual AutoStepFactoryArgs getArguments( void );

    private:
        Drive* ap_drive;
        bool a_brake;
};

#endif
