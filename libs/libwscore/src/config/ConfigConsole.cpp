#include "ConfigConsole.h"

#include "core/WildstangModule.h"
#include "config/ConfigValue.h"
#include "utils/ws_string.h"

#include <cstring>

#define CC_ERROR_BUFF_LEN 2048

ConfigConsole::ConfigConsole( WildstangModule* p_device )
    : WsNetConsole( 1112 )
    , ap_rootModule( p_device )
    , ap_currentModule( p_device )
{
}

ConfigConsole::~ConfigConsole( void )
{
}

int ConfigConsole::handleLine( const char* line )
{
    std::vector< std::string > tok = ws_strtok_sep( line, " \n\r" );
    if( tok.size() <= 0 ) return 0;

    std::string command = tok[ 0 ];

    int err = 0;
    if( 0 == strncmp( command.c_str(), "read", 4 ) )
    {
        err = this->handleRead( tok );
    }
    else if( 0 == strncmp( command.c_str(), "write", 5 ) )
    {
        err = this->handleWrite( tok );
    }
    else if( 0 == strncmp( command.c_str(), "pwd", 3 ) )
    {
        err = this->handlePwd();
    }
    else if( 0 == strncmp( command.c_str(), "module", 6 ) )
    {
        err = this->handleModule( tok );
    }
    else if( 0 == strncmp( command.c_str(), "ls", 2 ) )
    {
        err = this->handleLs();
    }
    else if( 0 == strncmp( command.c_str(), "children", 8 ) )
    {
        err = this->handleChildren();
    }
    else if( 0 == strncmp( command.c_str(), "up", 2 ) )
    {
        err = this->handleUp();
    }
    else if( 0 == strncmp( command.c_str(), "load", 4 ) )
    {
        err = this->handleLoad( tok );
    }
    else if( 0 == strncmp( command.c_str(), "save", 4 ) )
    {
        err = this->handleSave( tok );
    }
    else if( 0 == strncmp( command.c_str(), "tree", 4 ) )
    {
        err = this->handleTree();
    }
    else if( 0 == strncmp( command.c_str(), "emit", 4 ) )
    {
        err = this->handleEmit();
    }
    else if( 0 == strncmp( command.c_str(), "help", 4 ) )
    {
        net_printf( ">> read, write, pwd, module, ls, children, up, load, save, tree, emit, help\n" );
    }
    else
    {
        net_printf( "!! unrecognized command\n" );
        err = 1;
    }

    return err;
}

int ConfigConsole::handleRead( const std::vector< std::string >& rc_args )
{
    int err = 0;

    if( rc_args.size() != 3 )
    {
        net_printf( "!! expected: read PATH ITEM\n" );
        return 1;
    }

    std::string path = rc_args[ 1 ];
    std::string item = rc_args[ 2 ];
    std::string value;

    ConfigValue* p_value = this->resolveConfigValue( path, item );
    if( p_value )
    {
        p_value->getValue( value );
        net_printf( ">> %s\n", value.c_str() );
    }

    return err;
}

int ConfigConsole::handleWrite( const std::vector< std::string >& rc_args )
{
    int err = 0;

    if( rc_args.size() != 4 )
    {
        net_printf( "!! expected: write PATH ITEM VALUE\n" );
        return 1;
    }

    std::string path = rc_args[ 1 ];
    std::string item = rc_args[ 2 ];
    std::string value = rc_args[ 3 ];

    ConfigValue* p_value = this->resolveConfigValue( path, item );
    if( p_value ) p_value->updateValue( value );

    return err;
}

int ConfigConsole::handlePwd( void )
{
    net_printf( ">> %s\n", ap_currentModule->getPath().c_str() );
    return 0;
}

int ConfigConsole::handleModule( const std::vector< std::string >& rc_args )
{
    if( rc_args.size() == 1 )
    {
        ap_currentModule = ap_rootModule;
    }
    else if( rc_args.size() == 2 )
    {
        ap_currentModule = ap_rootModule->resolve( rc_args[ 1 ] );
        if( ap_currentModule == NULL )
        {
            net_printf( "!! couldn't find the module %s\n", rc_args[ 1 ].c_str() );
            ap_currentModule = ap_rootModule;
            return 1;
        }
    }

    return 0;
}

int ConfigConsole::handleLs( void )
{
    std::vector< std::string > v = ap_currentModule->listConfigValues();
    std::vector< std::string >::iterator i = v.begin();
    for(; i != v.end(); ++i )
    {
        net_printf( ">> %s\n", ( *i ).c_str() );
    }

    return 0;
}

int ConfigConsole::handleChildren( void )
{
    std::vector< std::string > v = ap_currentModule->listChildren();
    std::vector< std::string >::iterator i = v.begin();
    for(; i != v.end(); ++i )
    {
        net_printf( ">> %s\n", ( *i ).c_str() );
    }

    return 0;
}

int ConfigConsole::handleUp( void )
{
    ap_currentModule = ap_currentModule->getParent();
    if( ap_currentModule == NULL )
    {
        ap_currentModule = ap_rootModule;
        net_printf( "!! already at the top\n" );
        return 1;
    }

    return 0;
}

ConfigValue* ConfigConsole::resolveConfigValue( const std::string& rc_path,
                                                const std::string& rc_item )
{
    WildstangModule* p_device = ap_currentModule->resolve( rc_path );
    if( p_device == NULL )
    {
        net_printf( "!! device %s not found\n", rc_path.c_str() );
        return NULL;
    }

    ConfigValue* p_value = p_device->getConfigValue( rc_item );
    if( p_value == NULL )
    {
        net_printf( "!! device %s does not have value %s\n",
                    rc_path.c_str(), rc_item.c_str() );
        return NULL;
    }

    return p_value;
}

int ConfigConsole::handleLoad( const std::vector< std::string >& rc_args )
{
    if( rc_args.size() != 2 )
    {
        net_printf( "!! load FILENAME\n" );
        return 1;
    }

    std::string filename = rc_args[ 1 ];

    return ap_rootModule->loadConfigFile( filename );
}

int ConfigConsole::handleSave( const std::vector< std::string >& rc_args )
{
    if( rc_args.size() == 1 )
    {
        return ap_rootModule->saveConfigFile();
    }
    else if( rc_args.size() == 2 )
    {
        std::string filename = rc_args[ 1 ];

        return ap_rootModule->saveConfigFile( filename );
    }
    else
    {
        net_printf( "!! save [FILENAME]\n" );
        return 1;
    }
}

int ConfigConsole::handleTree( void )
{
    net_printf( ">> %s\n", ap_rootModule->getModuleName().c_str() );
    return this->makeTree( ap_currentModule, 1 );
}

#define SEND_SIZE 128
int ConfigConsole::handleEmit( void )
{
    std::string config;

    ap_rootModule->emitConfiguration( config );

    while( config.length() > SEND_SIZE )
    {
        net_printf( "%s", config.substr( 0, SEND_SIZE ).c_str() );
        config = config.substr( SEND_SIZE );
    }

    if( config.length() > 0 )
    {
        net_printf( "%s", config.c_str() );
    }

    net_printf( "\n" );

    return 0;
}

#define TAB_BUF_SIZE 256
int ConfigConsole::makeTree( WildstangModule* p_module, int idx )
{
    static const int tabstop = 2;
    char tab_buf[ TAB_BUF_SIZE ];
    memset( tab_buf, 0x00, sizeof( tab_buf ) );

    int max = TAB_BUF_SIZE - 2;
    int spaces = tabstop * idx;
    max = ( spaces < max ) ? spaces : max;

    for( int space = 0; space < max; ++space )
        tab_buf[ space ] = ' ';

    std::vector< std::string > values = p_module->listConfigValues();
    std::vector< std::string >::iterator j = values.begin();
    for(; j != values.end(); ++j )
    {
        net_printf( ">> %s*%s\n", tab_buf, ( *j ).c_str() );
    }

    int err = 0;

    std::vector< WildstangModule* > children = p_module->getChildren();
    std::vector< WildstangModule* >::iterator i = children.begin();
    for(; i != children.end(); ++i )
    {
        net_printf( ">> %s%s:\n", tab_buf, ( *i )->getModuleName().c_str() );
        err += this->makeTree( *i, idx + 1 );
    }

    return err;
}

int ConfigConsole::initializeConfigConsole( void )
{
    return this->startSocket();
}

