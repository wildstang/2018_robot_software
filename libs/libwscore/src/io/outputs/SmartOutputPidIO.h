#ifndef __SMARTOUTPUTPIDIO_H__
#define __SMARTOUTPUTPIDIO_H__

#include "pid/PidInput.h"
#include "pid/PidOutput.h"

class SmartOutputPidIO : public PidInput, public PidOutput
{
    public:
        SmartOutputPidIO( void );
        virtual ~SmartOutputPidIO( void );

        virtual double pidRead( void );
        virtual void pidWrite( double output );

        void setInput( double value );
        double getOutput( void ); 

    private:
        double a_output;
        double a_input;
};

#endif

