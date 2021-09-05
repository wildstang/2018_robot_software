#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <string>
#include <vector>

#include "core/WildstangModule.h"

typedef enum
{
    SMART_OUTPUT_CONTROL_MODE_ID_NONE = 0,

    SMART_OUTPUT_CONTROL_MODE_ID_FOLLOW,
    SMART_OUTPUT_CONTROL_MODE_ID_PERCENT_VOLTAGE,
    SMART_OUTPUT_CONTROL_MODE_ID_POSITION_PID,
    SMART_OUTPUT_CONTROL_MODE_ID_VELOCITY_PID,

    SMART_OUTPUT_CONTROL_MODE_ID_NUM
} smartOutputControlModeT;

typedef struct
{
    smartOutputControlModeT control_id;
    double speed;
    int follow_output_id;
    double pos_setpoint;
    double vel_setpoint;
    bool brake_mode;
    bool reset_sensor;
} smartOutputCmdT;

std::string toString( smartOutputControlModeT mode );
std::string toString( smartOutputCmdT x );

class OutputManager;
class OutputSource;
class WsIOInsert;

class Output : public WildstangModule
{
    public:
        Output( OutputManager* p_manager, std::string name );
        virtual ~Output( void );

        inline std::string getName( void ) { return a_name; }

        void update( void );

        void instantiateOutput( void );

        inline void enable( void ) { a_enabled = true; }
        inline void disable( void ) { a_enabled = false; }
        inline bool isEnabled( void ) { return a_enabled; }
        inline bool isInstantiated( void ) { return a_instantiated; }

        virtual void setValue( double value );
        virtual void setValue( int value );
        virtual void setValue( bool value );
        virtual void setValue( std::vector< unsigned char > value );
        virtual void setValue( smartOutputCmdT value );

        Output* addInsert( WsIOInsert* p_insert );
        void clearInserts( void );

        void setOutputSource( OutputSource* p_source );

    protected:
        virtual void sendDataToOutput( void ) = 0;
        virtual int instantiate( void ) = 0;

        std::vector< WsIOInsert* > a_inserts;

        static const smartOutputCmdT sc_nullCommand;

    private:
        OutputManager* ap_outputManager;
        std::string a_name;

        bool a_enabled;
        OutputSource* ap_source;
        bool a_instantiated;

        int a_instantiationWarningCount;
};

#endif

