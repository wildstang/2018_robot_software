#ifndef __WSLIDAR_H__
#define __WSLIDAR_H__

#include "io/inputs/InputStubs.h"

#include <string>

#include "I2C.h"

class LidarSensor;

class WsLIDAR : public DiscreteInput
{
    public:
        WsLIDAR( std::string name, enum frc::I2C::Port port, int address );
        WsLIDAR( std::string name );
        virtual ~WsLIDAR( void );

    protected:
        virtual int readRawValue( void ) override;

        virtual int instantiate( void ) override;

    private:
        LidarSensor* ap_lidar;
        frc::I2C::Port a_port;
        std::string a_portStr;
        const bool ac_factoryConstructor;
        int a_address;
};

#endif

