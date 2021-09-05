#ifndef __WSI2CINPUT_H__
#define __WSI2CINPUT_H__

#include "io/inputs/InputStubs.h"

#include <string>

#include "I2C.h"

class WsI2CInput : public BinaryInput
{
    public:
        WsI2CInput( std::string name, enum frc::I2C::Port port, 
                    int address, int size = 2 );
        WsI2CInput( std::string name );
        virtual ~WsI2CInput( void );
        
    protected:
        virtual std::vector< unsigned char > readRawValue( void ) override;

        virtual int instantiate( void ) override;

    private:
        frc::I2C* ap_i2c;
        int a_size;
        frc::I2C::Port a_port;
        int a_address;
        const bool ac_factoryConstructor;
        std::string a_portStr;

        static std::vector< unsigned char > s_empty;
};

#endif

