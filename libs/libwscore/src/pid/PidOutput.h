#ifndef __PIDOUTPUT_H__
#define __PIDOUTPUT_H__

class PidOutput
{
    public:
        PidOutput( void );
        virtual ~PidOutput( void );

        virtual void pidWrite( double output ) = 0;
};

#endif

