#ifndef __INVERTINSERT_H__
#define __INVERTINSERT_H__

#include "io/inserts/WsIOInsert.h"

class InvertInsert : public WsIOInsert
{
    public:
        InvertInsert( void );
        virtual ~InvertInsert( void );

        virtual double process( double value ) override;
        virtual int process( int value ) override;
        virtual bool process( bool value ) override;

        virtual std::string getName( void ) override;
};

#endif

