#ifndef __REMOTEANALOGINPUT_H__
#define __REMOTEANALOGINPUT_H__

#include "io/inputs/InputStubs.h"

#include "networktables/NetworkTable.h"

#include <memory>

class RemoteAnalogInput : public AnalogInput
{
    public:
        RemoteAnalogInput( std::string name, std::string network_table );
        RemoteAnalogInput( std::string name );
        virtual ~RemoteAnalogInput( void );
        
    protected:
        virtual double readRawValue( void ) override;

        virtual int instantiate( void ) override;
        
    private:
        std::shared_ptr< nt::NetworkTable > a_table;
        std::string a_tableName;
};

#endif

