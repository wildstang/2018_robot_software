#ifndef __AUTOCHOOSER_H__
#define __AUTOCHOOSER_H__

#include "subsystems/Subsystem.h"

#include <string>
#include <vector>

class Input;

typedef enum
{
    AUTO_MOD_RR = 0,
    AUTO_MOD_RL,
    AUTO_MOD_LL,
    AUTO_MOD_LR,
    AUTO_MOD_DELAY,
    AUTO_MOD_POS,

    AUTO_MOD_NUM
} autoModT;

typedef enum
{
    AUTO_START_LEFT = 0,
    AUTO_START_CENTER,
    AUTO_START_RIGHT,

    AUTO_START_NUM
} autoStartPosT;

std::string toString( autoStartPosT pos );

class AutoChooser : public Subsystem
{
    public:
        AutoChooser( void );
        virtual ~AutoChooser( void );

        virtual void init( void );
        virtual void resetState( void );
        virtual void update( void );
        virtual void selfTest( void );

        std::string getProgramName( std::string letters );
        double getDelay( void );

        void startup( void );

    protected:
        void renderString( void );

        std::string getProgramAtIndex( unsigned int index );

        int inputNextAutoButton( bool value );
        int inputPrevAutoButton( bool value );
        int inputNextFieldAutoButton( bool value );
        int inputPrevFieldAutoButton( bool value );

        const int ac_delayTicks;
        const double ac_maxDelay;
        int a_delayIndex;

        std::vector< std::string > a_programList;

        unsigned int a_rrIndex;
        unsigned int a_rlIndex;
        unsigned int a_llIndex;
        unsigned int a_lrIndex;

        std::string a_rrDefault;
        std::string a_rlDefault;
        std::string a_llDefault;
        std::string a_lrDefault;
        
        autoModT a_modifyingElementIndex;
        autoStartPosT a_startPos;

        bool a_infoUpdated;
        bool a_initialized;
};

#endif
