#ifndef __PIDINPUT_H__
#define __PIDINPUT_H__

class PidInput
{
    public:
        PidInput( void );
        virtual ~PidInput( void );

        virtual double pidRead( void ) = 0;
};

#endif

