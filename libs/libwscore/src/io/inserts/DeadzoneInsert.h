#ifndef __DEADZONEINSERT_H__
#define __DEADZONEINSERT_H__

#include "io/inserts/WsIOInsert.h"

class DeadzoneInsert : public WsIOInsert
{
    public:
        DeadzoneInsert( void );
        DeadzoneInsert( double thresh );
        virtual ~DeadzoneInsert( void );

        virtual double process( double value ) override;

        virtual std::string getName( void ) override;

    protected:
        double a_threshold;
};

#endif

