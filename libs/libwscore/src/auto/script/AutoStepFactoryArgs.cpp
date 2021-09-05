#include "AutoStepFactoryArgs.h"

#include "logger/WsLogger.h"
#include "auto/script/AutoStepFactory.h"
#include "auto/AutoManager.h"
#include "core/RobotCore.h"

#include <cstring>
#include <cstdlib>

std::string toString( autoStepFactoryArgTypeT arg )
{
    std::string str = std::string( "none" );

    switch( arg )
    {
        case STEP_NAME:
            str = std::string( "name" );
            break;
        case STEP_SPEED:
            str = std::string( "speed" );
            break;
        case STEP_INCHES:
            str = std::string( "inches" );
            break;
        case STEP_FILE:
            str = std::string( "file" );
            break;
        case STEP_STATE:
            str = std::string( "state" );
            break;
        case STEP_ANGLE:
            str = std::string( "angle" );
            break;
        case STEP_DISTANCE:
            str = std::string( "distance" );
            break;
        case STEP_F_GAIN:
            str = std::string( "f_gain" );
            break;
        case STEP_MILLISECONDS:
            str = std::string( "milliseconds" );
            break;
        case STEP_DIRECTION:
            str = std::string( "direction" );
            break;
        case STEP_POSITION:
            str = std::string( "position" );
            break;
        case STEP_TIMEOUT:
            str = std::string( "timeout" );
            break;
        case STEP_PROGRAM:
            str = std::string( "program" );
            break;
        case STEP_REVERSED:
            str = std::string( "reversed" );
            break;
        case STEP_ARG_1:
            str = std::string( "arg_1" );
            break;
        case STEP_ARG_2:
            str = std::string( "arg_2" );
            break;
        case STEP_ARG_3:
            str = std::string( "arg_3" );
            break;
        case STEP_ARG_4:
            str = std::string( "arg_4" );
            break;
        case STEP_ARG_5:
            str = std::string( "arg_5" );
            break;
        case STEP_ARG_6:
            str = std::string( "arg_6" );
            break;
        case STEP_ARG_7:
            str = std::string( "arg_7" );
            break;
        case STEP_ARG_8:
            str = std::string( "arg_8" );
            break;
        case STEP_ARG_9:
            str = std::string( "arg_9" );
            break;
        case STEP_ARG_10:
            str = std::string( "arg_10" );
            break;
        case STEP_NUM:
        case STEP_NONE:
        default:
            break;
    }

    return str;
}

int parse( const std::string& rc_str, autoStepFactoryArgTypeT& r_arg )
{
    r_arg = STEP_NONE;

    char buff[ 32 ];
    memset( buff, 0x00, 32 * sizeof( char ) );

    int l = ( rc_str.length() > 31 ) ? 31 : rc_str.length();

    for( int i = 0; i < l; i++ )
    {
        buff[ i ] = tolower( rc_str[ i ] );
    }

    //
    // Do not implement name, can't refer to it by text indirection.
    //

    if( 0 == strcmp( buff, "speed" ) ) r_arg = STEP_SPEED;
    else if( 0 == strcmp( buff, "inches" ) ) r_arg = STEP_INCHES;
    else if( 0 == strcmp( buff, "file" ) ) r_arg = STEP_FILE;
    else if( 0 == strcmp( buff, "state" ) ) r_arg = STEP_STATE;
    else if( 0 == strcmp( buff, "angle" ) ) r_arg = STEP_ANGLE;
    else if( 0 == strcmp( buff, "distance" ) ) r_arg = STEP_DISTANCE;
    else if( 0 == strcmp( buff, "f_gain" ) ) r_arg = STEP_F_GAIN;
    else if( 0 == strcmp( buff, "milliseconds" ) ) r_arg = STEP_MILLISECONDS;
    else if( 0 == strcmp( buff, "ms" ) ) r_arg = STEP_MILLISECONDS;
    else if( 0 == strcmp( buff, "direction" ) ) r_arg = STEP_DIRECTION;
    else if( 0 == strcmp( buff, "position" ) ) r_arg = STEP_POSITION;
    else if( 0 == strcmp( buff, "timeout" ) ) r_arg = STEP_TIMEOUT;
    else if( 0 == strcmp( buff, "program" ) ) r_arg = STEP_PROGRAM;
    else if( 0 == strcmp( buff, "reversed" ) ) r_arg = STEP_REVERSED;
    else if( 0 == strcmp( buff, "arg_1" ) ) r_arg = STEP_ARG_1;
    else if( 0 == strcmp( buff, "arg_2" ) ) r_arg = STEP_ARG_2;
    else if( 0 == strcmp( buff, "arg_3" ) ) r_arg = STEP_ARG_3;
    else if( 0 == strcmp( buff, "arg_4" ) ) r_arg = STEP_ARG_4;
    else if( 0 == strcmp( buff, "arg_5" ) ) r_arg = STEP_ARG_5;
    else if( 0 == strcmp( buff, "arg_6" ) ) r_arg = STEP_ARG_6;
    else if( 0 == strcmp( buff, "arg_7" ) ) r_arg = STEP_ARG_7;
    else if( 0 == strcmp( buff, "arg_8" ) ) r_arg = STEP_ARG_8;
    else if( 0 == strcmp( buff, "arg_9" ) ) r_arg = STEP_ARG_9;
    else if( 0 == strcmp( buff, "arg_10" ) ) r_arg = STEP_ARG_10;

    return ( r_arg == STEP_NONE ) ? 1 : 0;
}

AutoStepFactoryArgs::AutoStepFactoryArgs( void )
    : a_args()
    , a_touched()
    , a_errors( false )
{
    memset( a_touched, 0x00, sizeof( a_touched ) );
}

AutoStepFactoryArgs::~AutoStepFactoryArgs( void )
{
}

int AutoStepFactoryArgs::parse( std::deque< std::string >& r_tok )
{
    if( r_tok.empty() )
    {
        WS_ERROR( "unexpected end of code." );
        a_errors = true;
        return 1;
    }

    std::string name = r_tok.front();
    r_tok.pop_front();

    if( a_args.find( STEP_NAME ) != a_args.end() )
    {
        WS_ERROR( "unknown error parsing code, name already exists?" );
        a_errors = true;
        return 1;
    }

    a_args[ STEP_NAME ] = name;

    while( !r_tok.empty() )
    {
        std::string property_str = r_tok.front();
        r_tok.pop_front();

        if( property_str[ 0 ] == ';' )
        {
            break;
        }

        if( r_tok.empty() )
        {
            WS_ERROR( "expected '(' but found end of code, step %s property %s", 
                      name.c_str(), property_str.c_str() );
            a_errors = true;
            return 1;
        }

        autoStepFactoryArgTypeT property;
        if( ::parse( property_str, property ) )
        {
            WS_ERROR( "Error parsing: the property %s is not a real one. step %s", 
                      property_str.c_str(), name.c_str() );
            a_errors = true;
            return 1;
        }

        if( a_args.find( property ) != a_args.end() )
        {
            WS_ERROR( "Error parsing: the property %s already exists.",
                      toString( property ).c_str() );
            a_errors = true;
            return 1;
        }

        std::string open_paren = r_tok.front();
        r_tok.pop_front();

        if( open_paren[ 0 ] != '(' )
        {
            WS_ERROR( "expected '(', found %s, step %s property %s", 
                      open_paren.c_str(), name.c_str(), property_str.c_str() );
            a_errors = true;
            return 1;
        }

        if( r_tok.empty() )
        {
            WS_ERROR( "expected value, but found end of code. step %s property %s",
                      name.c_str(), property_str.c_str() );
            a_errors = true;
            return 1;
        }

        std::string value = r_tok.front();
        r_tok.pop_front();

        if( value[ 0 ] != '&' )
        {
            if( r_tok.empty() )
            {
                WS_ERROR( "expected ')' but found end of code, step %s, property %s",
                        name.c_str(), property_str.c_str() );
                a_errors = true;
                return 1;
            }
        }
        else
        {
            if( r_tok.empty() )
            {
                WS_ERROR( "expected dictionary value but found end of code, step %s, property %s",
                        name.c_str(), property_str.c_str() );
                a_errors = true;
                return 1;
            }

            std::string dictionary = r_tok.front();
            r_tok.pop_front();

            AutoStepFactory* p_factory = 
                RobotCore::getRobotCore()->getAutoManager()->getAutoStepFactory();

            value.clear();
            int err = p_factory->getDictionaryValue( dictionary, value );
            if( err )
            {
                WS_ERROR( "could not look up %s in the dictionary", dictionary.c_str() );
                a_errors = true;
                return 1;
            }
        }

        std::string closing_paren = r_tok.front();
        r_tok.pop_front();

        if( closing_paren[ 0 ] != ')' )
        {
            WS_ERROR( "expected ')', found %s, step %s property %s",
                    closing_paren.c_str(), name.c_str(), property_str.c_str() );
            a_errors = true;
            return 1;
        }

        a_args[ property ] = value;
    }

    return 0;
}

int AutoStepFactoryArgs::get( autoStepFactoryArgTypeT arg, std::string& r_v )
{
    int a = ( int )arg;
    if( ( a < 0 ) || ( a >= ( int )STEP_NUM ) )
    {
        WS_WARNING( "something is wrong with the enum autoStepFactoryArgTypeT" );
    }
    else
    {
        a_touched[ a ] = true;
    }

    std::map< autoStepFactoryArgTypeT, std::string >::iterator i = a_args.find( arg );
    if( i == a_args.end() )
    {
        a_errors = true;
        WS_ERROR( "argument %s is required for the step", toString( arg ).c_str() );
        return 1;
    }

    r_v.assign( i->second );
    return 0;
}

int AutoStepFactoryArgs::get( autoStepFactoryArgTypeT arg, double& r_v )
{
    std::string str;
    int err = this->get( arg, str );
    if( err ) return err;
    r_v = atof( str.c_str() );
    return 0;
}

int AutoStepFactoryArgs::get( autoStepFactoryArgTypeT arg, int& r_v )
{
    std::string str;
    int err = this->get( arg, str );
    if( err ) return err;
    r_v = atoi( str.c_str() );
    return 0;
}

int AutoStepFactoryArgs::get( autoStepFactoryArgTypeT arg, unsigned int& r_v )
{
    int x;
    int err = this->get( arg, x );
    if( err ) return err;
    r_v = ( unsigned int )x;
    return 0;
}

int AutoStepFactoryArgs::get( autoStepFactoryArgTypeT arg, bool& r_v )
{
    std::string str;
    int err = this->get( arg, str );
    if( err ) return err;

    err = ::parse( str, &r_v );
    if( err ) a_errors = true;

    return 0;
}

int AutoStepFactoryArgs::optional( autoStepFactoryArgTypeT arg, std::string& r_v )
{
    int a = ( int )arg;
    if( ( a < 0 ) || ( a >= ( int )STEP_NUM ) )
    {
        WS_WARNING( "something is wrong with the enum autoStepFactoryArgTypeT" );
    }
    else
    {
        a_touched[ a ] = true;
    }

    std::map< autoStepFactoryArgTypeT, std::string >::iterator i = a_args.find( arg );
    if( i != a_args.end() )
    {
        r_v.assign( i->second );
        return 0;
    }

    return 1;
}

int AutoStepFactoryArgs::optional( autoStepFactoryArgTypeT arg, double& r_v )
{
    std::string str;
    int err = this->optional( arg, str );
    if( !err )
    {
        r_v = atof( str.c_str() );
    }
    return 0;
}

int AutoStepFactoryArgs::optional( autoStepFactoryArgTypeT arg, int& r_v )
{
    std::string str;
    int err = this->optional( arg, str );
    if( !err )
    {
        r_v = atoi( str.c_str() );
    }
    return 0;
}

int AutoStepFactoryArgs::optional( autoStepFactoryArgTypeT arg, unsigned int& r_v )
{
    int x;
    int err = this->optional( arg, x );
    if( !err )
    {
        r_v = ( unsigned int )x;
    }
    return 0;
}

int AutoStepFactoryArgs::optional( autoStepFactoryArgTypeT arg, bool& r_v )
{
    std::string str;
    int err = this->optional( arg, str );

    if( !err )
    {
        err = ::parse( str, &r_v );
        if( err ) a_errors = true;
    }

    return 0;
}

int AutoStepFactoryArgs::set( autoStepFactoryArgTypeT arg, const std::string& rc_v )
{
    int a = ( int )arg;
    if( ( a < 0 ) || ( a >= ( int )STEP_NUM ) )
    {
        WS_WARNING( "something is wrong with the enum autoStepFactoryArgTypeT" );
    }
    else
    {
        a_touched[ a ] = true;
    }

    a_args[ arg ] = rc_v;
    return 0;
}

int AutoStepFactoryArgs::set( autoStepFactoryArgTypeT arg, const double& rc_v )
{
    return this->set( arg, toString( rc_v ) );
}

int AutoStepFactoryArgs::set( autoStepFactoryArgTypeT arg, const int& rc_v )
{
    return this->set( arg, toString( rc_v ) );
}

int AutoStepFactoryArgs::set( autoStepFactoryArgTypeT arg, const unsigned int& rc_v )
{
    int x = ( int )rc_v;
    return this->set( arg, toString( x ) );
}

int AutoStepFactoryArgs::set( autoStepFactoryArgTypeT arg, const bool& rc_v )
{
    return this->set( arg, toString( rc_v ) );
}

bool AutoStepFactoryArgs::exists( autoStepFactoryArgTypeT arg )
{
    std::map< autoStepFactoryArgTypeT, std::string >::iterator i = a_args.find( arg );
    return ( i != a_args.end() );
}

std::string AutoStepFactoryArgs::generateCode( void )
{
    std::string code;

    if( 0 != this->getName( code ) )
    {
        WS_ERROR( "couldn't generate code, no name." );
    }
    else
    {
        std::map< autoStepFactoryArgTypeT, std::string >::iterator i = a_args.begin();
        for(; i != a_args.end(); ++i )
        {
            if( i->first == STEP_NAME ) continue;

            code.append( ws_std_string_printf( " %s( %s )",
                         toString( i->first ).c_str(), i->second.c_str() ) );
        }

        code.append( ";" );
    }

    return code;
}

std::vector< autoStepFactoryArgTypeT > AutoStepFactoryArgs::getTouched( void )
{
    std::vector< autoStepFactoryArgTypeT > v;

    for( int i = 0; i < ( int )STEP_NUM; ++i )
    {
        if( a_touched[ i ] )
        {
            v.push_back( ( autoStepFactoryArgTypeT )i );
        }
    }

    return v;
}

std::vector< std::string > AutoStepFactoryArgs::getTouchedStr( void )
{
    std::vector< autoStepFactoryArgTypeT > args = this->getTouched();
    std::vector< std::string > v;

    std::vector< autoStepFactoryArgTypeT >::iterator i = args.begin();
    for(; i != args.end(); ++i )
    {
        v.push_back( toString( *i ) );
    }

    return v;
}

