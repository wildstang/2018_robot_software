#ifndef __REMOTEDIGITALINPUT_H__
#define __REMOTEDIGITALINPUT_H__

#include "io/inputs/InputStubs.h"

#include "networktables/NetworkTable.h"

#include <memory>

class RemoteDigitalInput : public DigitalInput
{
    public:
        RemoteDigitalInput( std::string name, std::string network_table );
        RemoteDigitalInput( std::string name );
        virtual ~RemoteDigitalInput( void );

    protected:
        virtual bool readRawValue( void ) override;

        virtual int instantiate( void ) override;

    private:
        std::shared_ptr< nt::NetworkTable > a_table;
        std::string a_tableName;
};

#endif

