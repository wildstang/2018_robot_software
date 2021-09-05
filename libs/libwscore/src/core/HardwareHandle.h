#ifndef __HARDWAREHANDLE_H__
#define __HARDWAREHANDLE_H__

#include <string>

typedef enum
{
    ALLIANCE_COLOR_NONE = 0,
    ALLIANCE_COLOR_RED,
    ALLIANCE_COLOR_BLUE,

    ALLIANCE_COLOR_NUM
} allianceColorT;

class AutoProgram;
class WsSmartDashboard;
class InputOutputFactory;

class HardwareHandle
{
    public:
        HardwareHandle( void );
        virtual ~HardwareHandle( void );

        virtual int initialize( void );

        virtual double getFPGATimestamp( void );
        virtual double getVoltage( void );
        virtual double getTemperature( void );
        virtual double getCurrent( int channel );
        virtual double getTotalCurrent( void );
        virtual double getTotalPower( void );
        virtual double getTotalEnergy( void );

        virtual void clearPdpStickyFaults( void );
        virtual void clearPcmStickyFaults( void );

        virtual std::string getGameData( void );
        virtual allianceColorT getAlliance( void );

        virtual int addAutoProgram( AutoProgram* p_program );
        virtual AutoProgram* getSelectedAutoProgram( void );
        virtual bool isAutoProgramLockedIn( void );
        virtual WsSmartDashboard* getSmartDashboard( void ) = 0;
        virtual InputOutputFactory* getIOFactory( void ) = 0;
};

#endif
