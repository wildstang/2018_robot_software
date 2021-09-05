#ifndef __REMOTEDIGITALOUTPUT_H__
#define __REMOTEDIGITALOUTPUT_H__

#include "io/outputs/OutputStubs.h"

#include "networktables/NetworkTable.h"

#include <memory>

class RemoteDigitalOutput : public DigitalOutput
{
    public:
        RemoteDigitalOutput( std::string name, std::string table, bool default_value );
        RemoteDigitalOutput( std::string name );
        virtual ~RemoteDigitalOutput( void );

        virtual int instantiate( void );

    protected:
        virtual void sendDataToOutput( void );

    private:
        std::shared_ptr< nt::NetworkTable > a_table;
        std::string a_tableName;
};

#endif

