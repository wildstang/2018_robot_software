#include "AutoStepFactory.h"

#include <ctype.h>
#include <cstring>
#include <cstdlib>

#include "logger/WsLogger.h"
#include "auto/script/StepFactory.h"
#include "auto/script/AutoStepFactoryArgs.h"
#include "auto/steps/AutoStep.h"
#include "utils/ws_string.h"
#include "utils/ws_debug.h"

AutoStepFactory::AutoStepFactory( void )
    : a_factories()
    , a_dictionary()
{
}

AutoStepFactory::~AutoStepFactory( void )
{
    std::map< std::string, StepFactory* >::iterator i = a_factories.begin();
    for(; i != a_factories.end(); ++i )
    {
        StepFactory* p_factory = i->second;
        delete p_factory;
    }

    a_factories.clear();
}

int AutoStepFactory::addStepFactory( StepFactory* p_factory )
{
    int err = 0;

    if( a_factories.find( p_factory->getName() ) != a_factories.end() )
    {
        WS_ERROR( "the autostep factory %s already exists",
                   p_factory->getName().c_str() );
        err++;
    }
    else
    {
        a_factories[ p_factory->getName() ] = p_factory;

        unsigned int s = a_factories.size();
        WS_INFO( "adding auto step factory %s, size %u", 
                 p_factory->getName().c_str(), s );
    }

    return err;
}

std::vector< std::string > AutoStepFactory::getInstalledSteps( void )
{
    std::vector< std::string > v;

    std::map< std::string, StepFactory* >::iterator i = a_factories.begin();
    for(; i != a_factories.end(); ++i )
    {
        v.push_back( i->first );
    }

    return v;
}

std::string AutoStepFactory::getStepUsage( const std::string& rc_step )
{
    std::string usage;

    std::map< std::string, StepFactory* >::iterator i = a_factories.find( rc_step );
    if( i == a_factories.end() )
    {
        WS_ERROR( "factory for step %s does not exist", rc_step.c_str() );
        return usage;
    }

    StepFactory* p_factory = i->second;
    AutoStepFactoryArgs args;
    AutoStep* p_step = p_factory->createStep( &args );

    std::vector< std::string > touched = args.getTouchedStr();

    delete p_step;
    p_step = NULL;

    usage.append( rc_step );

    std::vector< std::string >::iterator j = touched.begin();
    for(; j != touched.end(); ++j )
    {
        usage.append( " " );
        usage.append( *j );
        usage.append( "()" );
    }

    return usage;
}

AutoStep* AutoStepFactory::createStep( std::deque< std::string >& r_tok )
{
    AutoStepFactoryArgs args;
    int err = args.parse( r_tok );
    if( err ) return NULL;

    std::string name;
    err = args.getName( name );
    if( err ) return NULL;

    std::map< std::string, StepFactory* >::iterator i = a_factories.find( name );
    if( i == a_factories.end() )
    {
        WS_ERROR( "no autostep called %s", name.c_str() );
        return NULL;
    }

    AutoStep* p_step = i->second->createStep( &args );
    if( p_step == NULL )
    {
        WS_ERROR( "failed to create step %s because wrong arguments provided.", name.c_str() );
        return NULL;
    }

    if( p_step->asGroup() )
    {
        WS_ERROR( "%s is a group. Must specify with @", name.c_str() );
        delete p_step;
        return NULL;
    }

    return p_step;
}

AutoStepGroup* AutoStepFactory::createGroup( std::deque< std::string >& r_tok )
{
    if( r_tok.empty() )
    {
        WS_ERROR( "unexpected end of code, expected group type" );
        return NULL;
    }

    std::string group = r_tok.front();
    r_tok.pop_front();

    if( r_tok.empty() )
    {
        WS_ERROR( "expected: {, unexpected end of code." );
        return NULL;
    }

    std::string opening_curly = r_tok.front();
    r_tok.pop_front();

    if( opening_curly[ 0 ] != '{' )
    {
        WS_ERROR( "expected opening curly, found %s", opening_curly.c_str() );
        return NULL;
    }

    AutoStepFactoryArgs args;
    args.setName( group );

    std::map< std::string, StepFactory* >::iterator i = a_factories.find( group );
    if( i == a_factories.end() )
    {
        WS_ERROR( "could not find group %s", group.c_str() );
        return NULL;
    }

    AutoStep* p_step = i->second->createStep( &args );
    if( p_step == NULL )
    {
        WS_ERROR( "failed to create step %s because wrong arguments provided.", group.c_str() );
        return NULL;
    }

    AutoStepGroup* p_group = p_step->asGroup();
    if( p_group == NULL )
    {
        WS_ERROR( "%s is not a group.", group.c_str() );
        delete p_step;
        return NULL;
    }

    return p_group;
}

int AutoStepFactory::getDictionaryValue( const std::string& rc_key, std::string& r_value )
{
    std::map< std::string, std::string >::iterator i = a_dictionary.find( rc_key );
    if( i == a_dictionary.end() )
    {
        WS_ERROR( "there is no dictionary value for %s", rc_key.c_str() );
        return 1;
    }

    r_value.assign( i->second );

    return 0;
}

int AutoStepFactory::addDictionaryValue( const std::string c_key, const std::string c_value )
{
    if( a_dictionary.find( c_key ) != a_dictionary.end() )
    {
        WS_ERROR( "the dictionary already has a key %s", c_key.c_str() );
        return 1;
    }

    a_dictionary[ c_key ] = c_value;

    return 0;
}

std::vector< std::string > AutoStepFactory::listDictionaryKeys( void )
{
    std::vector< std::string > v;

    std::map< std::string, std::string >::iterator i = a_dictionary.begin();
    for(; i != a_dictionary.end(); ++i )
    {
        v.push_back( i->first );
    }

    return v;
}

