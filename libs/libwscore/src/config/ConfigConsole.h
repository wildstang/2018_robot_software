#ifndef __CONFIGCONSOLE_H__
#define __CONFIGCONSOLE_H__

#include "net/WsNetConsole.h"

#include <string>
#include <vector>

class ConfigValue;
class WildstangModule;

class ConfigConsole : public WsNetConsole
{
    public:
        ConfigConsole( WildstangModule* p_device );
        virtual ~ConfigConsole( void );

        int initializeConfigConsole( void );

    protected:
        virtual int handleLine( const char* line ) override;

        ConfigValue* resolveConfigValue( const std::string& rc_path,
                                         const std::string& rc_item );

        int handleRead( const std::vector< std::string >& rc_args );
        int handleWrite( const std::vector< std::string >& rc_args );
        int handlePwd( void );
        int handleModule( const std::vector< std::string >& rc_args );
        int handleLs( void );
        int handleChildren( void );
        int handleUp( void );
        int handleLoad( const std::vector< std::string >& rc_args );
        int handleSave( const std::vector< std::string >& rc_args );
        int handleTree( void );
        int handleEmit( void );

        int makeTree( WildstangModule* p_module, int idx = 0 );

        WildstangModule* ap_rootModule;
        WildstangModule* ap_currentModule;
};

#endif

