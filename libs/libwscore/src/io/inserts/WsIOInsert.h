#ifndef __WSIOINSERT_H__
#define __WSIOINSERT_H__

#include <vector>
#include <string>

#include "io/Output.h"

class WsIOInsert
{
    public:
        WsIOInsert( void );
        virtual ~WsIOInsert( void );

        virtual double process( double value );
        virtual int process( int value );
        virtual bool process( bool value );
        virtual std::vector< unsigned char > process( std::vector< unsigned char > value );
        virtual smartOutputCmdT process( smartOutputCmdT value );

        virtual std::string getName( void );
};

#endif

