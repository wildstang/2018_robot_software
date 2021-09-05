#ifndef __AUTOMANAGER_H__
#define __AUTOMANAGER_H__

#include <string>
#include <vector>
#include <map>

#include "net/WsNetConsole.h"
#include "core/WildstangModule.h"

class AutoProgram;
class AutoStepFactory;

class AutoManager : public WildstangModule, public WsNetConsole
{
    public:
        AutoManager( AutoStepFactory* p_factory );
        ~AutoManager( void );

        void update( void );
        void clear( void );
        void init( void );

        void startSelectedProgram( void );
        void startProgramByName( std::string program_name );
        void startProgram( AutoProgram* p_program );
        void changeProgram( std::string program_name, bool reverse = false );
        void changeProgram( AutoProgram* p_program, bool reverse = false );

        std::string getRunningProgramName( void );
        std::vector< std::string > getListOfPrograms( void );

        inline AutoStepFactory* getAutoStepFactory( void )
            { return ap_factory; }
        void setAutoProgramDir( std::string dir_name );

        inline bool isReversed( void ) { return a_reversed; }

    protected:
        virtual int handleLine( const char* line );

        int handleReceive( const std::vector< std::string >& rc_args );
        int handleList( void );
        int handleSend( const std::vector< std::string >& rc_args );
        int handleGenerate( const std::vector< std::string >& rc_args );
        int handleDelete( const std::vector< std::string >& rc_args );
        int handleListSteps( void );
        int handleDict( void );
        int handleUsage( const std::vector< std::string >& rc_args );
        int handleApi( void );
        int handleCommit( const std::vector< std::string >& rc_args );

        int loadProgramsFromDirectory( std::string dir_name );
        void addProgram( AutoProgram* program );

        AutoProgram* getRunningProgram( void );

    private:
        void definePrograms( void );

        std::map< std::string, AutoProgram* > a_programs;
        AutoProgram* ap_runningProgram;
        AutoProgram* ap_sleeperProgram;
        AutoProgram* ap_queuedProgram;

        bool a_autoProgramsDirectorySet;
        std::string a_directoryName;
        std::string a_receivingProgram;
        std::string a_receivingCode;
        bool a_currentlyReceivingProgram;
        bool a_reversed;

        int a_forceStopAtStep;

        AutoStepFactory* ap_factory;
};

#endif
