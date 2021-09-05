#ifndef __WSCOMPASSINPUT_H__
#define __WSCOMPASSINPUT_H__

#include "WsI2CInput.h"
#include "PIDSource.h"

#include "I2C.h"

class WsCompassInput : public WsI2CInput, frc::PIDSource
{
    public:
        WsCompassInput( std::string name, enum frc::I2C::Port port, int address );
        WsCompassInput( std::string name );
        virtual ~WsCompassInput( void );
        
        virtual void SetPIDSourceType( frc::PIDSourceType pidSource ) override;
        virtual frc::PIDSourceType GetPIDSourceType( void ) const override;
        virtual double PIDGet( void ) override;
};

#endif

