#ifndef __REMOTEANALOGOUTPUT_H__
#define __REMOTEANALOGOUTPUT_H__

#include "io/outputs/OutputStubs.h"

#include "networktables/NetworkTable.h"

#include <memory>
#include <string>

class RemoteAnalogOutput : public AnalogOutput
{
    public:
        RemoteAnalogOutput( std::string name, std::string table, double default_value );
        RemoteAnalogOutput( std::string name );
        virtual ~RemoteAnalogOutput( void );

        virtual int instantiate( void );

    protected:
        virtual void sendDataToOutput( void );

    private:
        std::shared_ptr< nt::NetworkTable > a_table;
        std::string a_tableName;
};

#endif

