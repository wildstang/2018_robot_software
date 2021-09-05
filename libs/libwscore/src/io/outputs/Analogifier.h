#ifndef __ANALOGIFIER_H__
#define __ANALOGIFIER_H__

#include "io/outputs/OutputStubs.h"

class Analogifier : public AnalogOutput
{
    public:
        Analogifier( std::string name, 
                     std::string output_name, 
                     double default_value );
        Analogifier( std::string name );
        virtual ~Analogifier( void );

    protected:
        virtual void sendDataToOutput( void );

        virtual int instantiate( void );

    private:
        std::string a_outputName;
        Output* ap_motor;
};

#endif

