#include "core/WildstangModule.h"

#include "logger/WsLogger.h"
#include "logger/Probe.h"
#include "utils/ws_string.h"
#include "utils/ws_debug.h"
#include "utils/ws_file.h"
#include "config/ConfigValue.h"

#include <unistd.h>

WildstangModule::WildstangModule( const std::string c_name,
                                  WildstangModule* p_parent )
    : a_name( c_name )
    , ap_parent( p_parent )
    , a_children()
    , a_configValues()
    , a_emissionEnable( true )
    , a_loadedFromFile( false )
    , a_lastConfigFileLoad()
    , a_probes()
{
    WS_INFO( "WildstangModule constructor %s", c_name.c_str() );
    WS_ASSERT( ws_is_well_formed_name( c_name.c_str() ) );

    if( ap_parent )
    {
        ap_parent->addChild( this );
    }
}

WildstangModule::~WildstangModule( void )
{
    WS_WARNING( "WildstangModule destructor %s", this->getModuleName().c_str() );

    std::map< std::string, ConfigValue* >::iterator i = a_configValues.begin();
    for(; i != a_configValues.end(); ++i )
    {
        ConfigValue* p_value = i->second;
        delete p_value;
    }
    a_configValues.clear();

    this->deleting();
}

WildstangModule* WildstangModule::getChild( const std::string c_child )
{
    WildstangModule* p_child = NULL;

    std::map< std::string, WildstangModule* >::iterator i = a_children.find( c_child );
    if( i != a_children.end() )
    {
        p_child = i->second;
    }

    return p_child;
}

void WildstangModule::addChild( WildstangModule* p_child )
{
    if( p_child )
    {
        if( a_children.find( p_child->getModuleName() ) != a_children.end() )
        {
            WS_WARNING( "WildstangModule %s already has a child named %s",
                        this->getModuleName().c_str(), 
                        p_child->getModuleName().c_str() );
            return;
        }

        a_children[ p_child->getModuleName() ] = p_child;
    }
    else
    {
        WS_ERROR( "tried to add a null child to %s",
                  this->getModuleName().c_str() );
    }
}

void WildstangModule::parentIsDeleted( void )
{
    ap_parent = NULL;
}

void WildstangModule::childIsDeleted( WildstangModule* p_device )
{
    // not great, maybe fix later.
    std::map< std::string, WildstangModule* >::iterator i = 
        a_children.find( p_device->getModuleName() );

    if( i != a_children.end() )
    {
        a_children.erase( i );
    }
}

void WildstangModule::deleting( void )
{
    std::map< std::string, WildstangModule* >::iterator i = a_children.begin();
    for(; i != a_children.end(); ++i )
    {
        i->second->parentIsDeleted();
    }

    if( ap_parent )
    {
        ap_parent->childIsDeleted( this );
    }
}

WildstangModule* WildstangModule::resolve( std::string path )
{
    std::vector< std::string > v = ws_strtok_sep( path.c_str(), "." );
    std::vector< std::string >::iterator i = v.begin();
    for(; i != v.end(); ++i )
    {
        ws_trim( *i );
    }

    std::deque< std::string > d = ws_to_deque( v );

    return this->__resolve( d );
}

WildstangModule* WildstangModule::__resolve( std::deque< std::string >& r_d )
{
    if( r_d.size() == 0 ) return this;

    std::string child = r_d.front();
    r_d.pop_front();

    if( child[ 0 ] == '$' )
    {
        return this;
    }

    WildstangModule* p_child = this->getChild( child );

    if( p_child == NULL )
    {
        WS_ERROR( "could not resolve the %s.%s", 
                  this->getModuleName().c_str(), child.c_str() );
        return NULL;
    }

    return p_child->__resolve( r_d );
}

ConfigValue* WildstangModule::getConfigValue( std::string item )
{
    std::map< std::string, ConfigValue* >::iterator i = a_configValues.find( item );
    if( i == a_configValues.end() ) return NULL;
    return i->second;
}

std::string WildstangModule::getPath( void )
{
    std::string path;
    if( ap_parent )
    {
        path = ap_parent->getPath();
        path = path + "." + this->getModuleName();
    }
    else
    {
        path = this->getModuleName();
    }

    return path;
}

std::vector< std::string > WildstangModule::listChildren( void )
{
    std::vector< std::string > v;

    std::map< std::string, WildstangModule* >::iterator i = a_children.begin();
    for(; i != a_children.end(); ++i )
    {
        v.push_back( i->first );
    }

    return v;
}

std::vector< std::string > WildstangModule::listConfigValues( void )
{
    std::vector< std::string > v;

    std::map< std::string, ConfigValue* >::iterator i = a_configValues.begin();
    for(; i != a_configValues.end(); ++i )
    {
        v.push_back( i->first );
    }

    return v;
}

int WildstangModule::addConfigValue( ConfigValue* p_value )
{
    if( p_value == NULL ) return 1;

    std::map< std::string, ConfigValue* >::iterator i =
        a_configValues.find( p_value->getName() );
    if( i != a_configValues.end() ) return 1;

    a_configValues[ p_value->getName() ] = p_value;

    return 0;
}

int WildstangModule::loadConfigFile( std::string filename )
{
    std::string config_str = ws_get_file_contents_string( filename );
    if( config_str.length() <= 0 ) return 1;
    std::deque< std::string > tokenized =
        ws_tokenize( config_str.c_str(), " \n\r\t,", "=:{}", ".", true );

    if( tokenized.empty() )
    {
        WS_ERROR( "no json code to parse" );
        return 1;
    }

    std::string object_name = tokenized.front();
    tokenized.pop_front();

    if( object_name != this->getModuleName() )
    {
        WS_ERROR( "config loaded on the wrong module" );
        return 1;
    }

    if( tokenized.empty() )
    {
        WS_ERROR( "expected ':'" );
        return 1;
    }

    std::string colon = tokenized.front();
    tokenized.pop_front();

    if( colon[ 0 ] != ':' )
    {
        WS_ERROR( "expected ':'" );
        return 1;
    }

    if( tokenized.empty() )
    {
        WS_ERROR( "expected: '{'" );
        return 1;
    }

    std::string opening_brace = tokenized.front();
    tokenized.pop_front();

    if( opening_brace[ 0 ] != '{' )
    {
        WS_ERROR( "expected: '{'" );
        return 1;
    }

    int err = this->parseAndApplyConfiguration( tokenized );

    if( !err )
    {
        a_loadedFromFile = true;
        a_lastConfigFileLoad.assign( filename );
    }

    return err;
}

int WildstangModule::saveConfigFile( void )
{
    int err = 0;

    if( a_loadedFromFile )
    {
        err = this->saveConfigFile( a_lastConfigFileLoad );
    }
    else
    {
        WS_ERROR( "No last config file name." );
        err++;
    }

    return err;
}

int WildstangModule::saveConfigFile( std::string filename )
{
    std::string json_contents;
    int err = this->emitConfiguration( json_contents );
    if( err ) return err;
    err = ws_write_file( filename, json_contents );

    sync();

    return err;
}

int WildstangModule::parseAndApplyConfiguration( std::deque< std::string >& r_d )
{
    if( r_d.empty() )
    {
        WS_ERROR( "unexpected end of json code, module %s",
                  this->getPath().c_str() );
        return 1;
    }

    std::string item = r_d.front();
    r_d.pop_front();

    int err = 0;
    while( item[ 0 ] != '}' )
    {
        if( r_d.empty() )
        {
            WS_ERROR( "unexpected end of json code, module %s",
                      this->getPath().c_str() );
            err++;
            break;
        }

        std::string colon = r_d.front();
        r_d.pop_front();

        if( colon[ 0 ] != ':' )
        {
            WS_ERROR( "improperly formatted json, expected ':' found %s module %s",
                      colon.c_str(), this->getPath().c_str() );
            err++;
            break;
        }

        if( r_d.empty() )
        {
            WS_ERROR( "unexpected end of json code, module %s",
                      this->getPath().c_str() );
            err++;
            break;
        }

        std::string value = r_d.front();
        r_d.pop_front();

        if( value[ 0 ] == '{' )
        {
            WildstangModule* p_child = this->getChild( item );
            if( p_child == NULL )
            {
                WS_WARNING( "module %s has no child %s",
                            this->getPath().c_str(), item.c_str() );

                //
                // consume everything up to where we would need to be.
                //

                err += this->disregardItem( r_d );
                if( err ) break;
            }
            else
            {
                err += p_child->parseAndApplyConfiguration( r_d );
                if( err ) break;
            }
        }
        else
        {
            // config value
            ConfigValue* p_value = this->getConfigValue( item );
            if( p_value == NULL )
            {
                WS_WARNING( "module %s has no config value %s",
                            this->getPath().c_str(), item.c_str() );

                //
                // consume everything up to where we would need to be.
                // in this case, there's actually nothing to consume
                //
            }
            else
            {
                err = p_value->updateValue( value );
                if( err ) break;
            }
        }

        if( r_d.empty() )
        {
            WS_ERROR( "unexpected end of json code, module %s",
                      this->getPath().c_str() );
            err++;
            break;
        }

        item = r_d.front();
        r_d.pop_front();
    }

    return err;
}

int WildstangModule::__emitConfiguration( std::string& r_config_json )
{
    const int c_tabstop = 4;
    int err = 0;

    std::string indentation;
    indentation.reserve( 512 );
    int spaces = this->getDepth() * c_tabstop;
    for(; spaces > 0; spaces-- )
        indentation.append( " " );

    bool first = true;
    std::map< std::string, WildstangModule* >::iterator i = a_children.begin();
    for(; i != a_children.end(); ++i )
    {
        if( !( i->second->a_emissionEnable ) ) continue;

        if( !first ) r_config_json.append( ",\n" );
        first = false;
        r_config_json.append( indentation );
        r_config_json.append( ws_std_string_printf( "\"%s\": {\n", i->first.c_str() ) );
        i->second->__emitConfiguration( r_config_json );
        r_config_json.append( indentation );
        r_config_json.append( "}" );
    }

    std::map< std::string, ConfigValue* >::iterator j = a_configValues.begin();
    for(; j != a_configValues.end(); ++j )
    {
        if( !first ) r_config_json.append( ",\n" );
        first = false;
        r_config_json.append( indentation );
        std::string value;
        j->second->getValue( value );
        r_config_json.append( ws_std_string_printf( "\"%s\": \"%s\"",
            j->first.c_str(), value.c_str() ) );
    }

    if( !first ) r_config_json.append( "\n" );

    return err;
}

int WildstangModule::emitConfiguration( std::string& r_config_json )
{
    r_config_json.append( ws_std_string_printf( "\"%s\": {\n", 
                            this->getModuleName().c_str() ) );

    int err = this->__emitConfiguration( r_config_json );

    r_config_json.append( "}\n" );

    return err;
}

int WildstangModule::getDepth( void )
{
    if( ap_parent ) return ap_parent->getDepth() + 1;
    return 1;
}

std::vector< WildstangModule* > WildstangModule::getChildren( void )
{
    std::vector< WildstangModule* > children;

    std::map< std::string, WildstangModule* >::iterator i = a_children.begin();
    for(; i != a_children.end(); ++i )
    {
        children.push_back( i->second );
    }

    return children;
}

int WildstangModule::disregardItem( std::deque< std::string >& r_d )
{
    int search_depth = 1;
    std::string search_for_close( "{" );

    while( !r_d.empty() && ( search_depth > 0 ) )
    {
        search_for_close = r_d.front();
        r_d.pop_front();

        if( search_for_close[ 0 ] == '{' )
            search_depth++;
        else if( search_for_close[ 0 ] == '}' )
            search_depth--;
    }

    if( search_depth > 0 )
    {
        WS_ERROR( "unexpected end of json code" );
        return 1;
    }

    return 0;
}

void WildstangModule::setEmissionEnable( bool enable )
{
    a_emissionEnable = enable;
}

int WildstangModule::addProbe( Probe* p_probe )
{
    if( p_probe == NULL )
    {
        WS_ERROR( "tried to add a null probe to %s", this->getModuleName().c_str() );
        return 1;
    }

    if( a_probes.find( p_probe->getName() ) != a_probes.end() )
    {
        WS_ERROR( "probe %s already exists in module %s", 
                  p_probe->getName().c_str(), this->getModuleName().c_str() );
        return 1;
    }

    a_probes[ p_probe->getName() ] = p_probe;

    return 0;
}

Probe* WildstangModule::getProbe( const std::string& rc_probe_name )
{
    std::map< std::string, Probe* >::iterator i = a_probes.find( rc_probe_name );
    if( i == a_probes.end() )
    {
        WS_ERROR( "module %s does not have a probe named %s",
                  this->getModuleName().c_str(), rc_probe_name.c_str() );
        return NULL;
    }

    return i->second;
}

std::vector< std::string > WildstangModule::listProbes( void )
{
    std::vector< std::string > v;

    std::map< std::string, Probe* >::iterator i = a_probes.begin();
    for(; i != a_probes.end(); ++i )
    {
        v.push_back( i->first );
    }

    return v;
}

