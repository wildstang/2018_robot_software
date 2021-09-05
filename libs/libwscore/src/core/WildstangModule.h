#ifndef __WILDSTANGMODULE_H__
#define __WILDSTANGMODULE_H__

#include <map>
#include <string>
#include <vector>
#include <deque>

class ConfigValue;
class Probe;

class WildstangModule
{
    //
    // General structure support
    //
    public:
        WildstangModule( const std::string c_name,
                         WildstangModule* p_parent );
        virtual ~WildstangModule( void );

        WildstangModule* resolve( std::string path );
        WildstangModule* getChild( const std::string c_child );
        std::string getPath( void );
        std::vector< std::string > listChildren( void );
        std::vector< WildstangModule* > getChildren( void );
        inline WildstangModule* getParent( void ) { return ap_parent; }
        inline std::string getModuleName( void ) { return a_name; }

    protected:
        int getDepth( void );

    private:
        std::string a_name;
        WildstangModule* ap_parent;
        std::map< std::string, WildstangModule* > a_children;

        void deleting( void );
        void parentIsDeleted( void );
        void childIsDeleted( WildstangModule* p_device );
        void addChild( WildstangModule* p_child );

        WildstangModule* __resolve( std::deque< std::string >& r_v );

    //
    // Configuration
    //
    public:
        ConfigValue* getConfigValue( std::string item );
        std::vector< std::string > listConfigValues( void );

        int loadConfigFile( std::string filename );
        int saveConfigFile( std::string filename );
        int saveConfigFile( void );

        int emitConfiguration( std::string& r_config_json );

    protected:
        int addConfigValue( ConfigValue* p_value );

        void setEmissionEnable( bool enable );

    private:
        std::map< std::string, ConfigValue* > a_configValues;
        bool a_emissionEnable;
        bool a_loadedFromFile;
        std::string a_lastConfigFileLoad;

        int parseAndApplyConfiguration( std::deque< std::string >& r_d );
        int disregardItem( std::deque< std::string >& r_d );
        int __emitConfiguration( std::string& r_config_json );

    //
    // Probes
    //
    public:
        Probe* getProbe( const std::string& rc_probe_name );
        std::vector< std::string > listProbes( void );

    protected:
        int addProbe( Probe* p_probe );

    private:
        std::map< std::string, Probe* > a_probes;
};

#endif

