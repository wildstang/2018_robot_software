#include "ws_string.h"

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>

#include "logger/WsLogger.h"

#define STD_STRING_VPRINTF_BUF_LENGTH 512
#define STRTOK_COPY_STRING_BUF_LENGTH 4096

void ws_trim( std::string& r_str )
{
    size_t begin = 0;
    size_t end = r_str.length() + 1;
    bool begin_located = false;

    const char* bad_chars = " \t\n\r";

    for( size_t i = 0; i < r_str.length(); ++i )
    {
        bool found = false;
        for( const char* j = bad_chars; *j != '\0'; ++j )
        {
            if( r_str[ i ] == *j )
            {
                found = true;
                break;
            }
        }

        if( !found )
        {
            if( !begin_located )
            {
                begin = i;
                end = i;
                begin_located = true;
            }
            else
            {
                end = i;
            }
        }
    }

    r_str.assign( r_str.substr( begin, end - begin + 1 ) );
}

std::string ws_std_string_printf( const char* fmt, ... )
{
    if( fmt == NULL ) return std::string( "" );

    va_list v;
    va_start( v, fmt );

    return ws_std_string_vprintf( fmt, v );
}

std::string ws_std_string_vprintf( const char* fmt, va_list args )
{
    if( fmt == NULL ) return std::string( "" );
    char buf[ STD_STRING_VPRINTF_BUF_LENGTH ];
    buf[ STD_STRING_VPRINTF_BUF_LENGTH - 1 ] = '\0';
    vsnprintf( buf, STD_STRING_VPRINTF_BUF_LENGTH - 1, fmt, args );
    return std::string( buf );
}

std::string toString( double x )
{
    return ws_std_string_printf( "%f", x );
}

std::string toString( int x )
{
    return ws_std_string_printf( "%d", x );
}

std::string toString( bool x )
{
    std::string str;

    if( x )
    {
        str.assign( "true" );
    }
    else
    {
        str.assign( "false" );
    }

    return str;
}

std::string toString( const std::vector< unsigned char >& rc_v )
{
    std::string str;
    char buf[ 3 ] = { '\0', '\0', '\0' };

    std::vector< unsigned char >::const_iterator i = rc_v.cbegin();
    for(; i != rc_v.cend(); ++i )
    {
        unsigned char c = *i;
        ws_to_hex( buf, c );
        str.append( std::string( buf ) );
    }

    return str;
}

std::string toString( std::string s )
{
    // for compatibility with templates
    return s;
}

std::string toString( uint64_t x )
{
#if defined(TARGET_SIMULATION)
    return ws_std_string_printf( "%lu", x );
#elif defined(TARGET_FRC_CRIO_WPILIB)
    return ws_std_string_printf( "%llu", x );
#else
#error "undefined target"
#endif
}

int parse( const std::string& str, double* p_value )
{
    if( p_value == NULL ) return 1;

    std::string copy( str );
    ws_trim( copy );

    *p_value = atof( copy.c_str() );

    return 0;
}

int parse( const std::string& str, int* p_value )
{
    if( p_value == NULL ) return 1;

    std::string copy( str );
    ws_trim( copy );

    *p_value = atoi( copy.c_str() );

    return 0;
}

int parse( const std::string& str, bool* p_value )
{
    if( p_value == NULL ) return 1;

    std::string copy( str );
    ws_trim( copy );
    ws_to_lower( copy );
    int err = 0;

    if( copy.length() == 0 )
    {
        WS_WARNING( "empty string to parse to bool" );
        err++;
        *p_value = false;
    }
    else if( copy.c_str()[ 0 ] == '0' )
    {
        *p_value = false;
    }
    else if( copy.c_str()[ 0 ] == '1' )
    {
        *p_value = true;
    }
    else if( copy.c_str()[ 0 ] == 't' )
    {
        *p_value = true;
    }
    else if( copy.c_str()[ 0 ] == 'f' )
    {
        *p_value = false;
    }
    else if( 0 == strncmp( copy.c_str(), "on", 2 ) )
    {
        *p_value = true;
    }
    else if( 0 == strncmp( copy.c_str(), "off", 3 ) )
    {
        *p_value = false;
    }
    else
    {
        WS_WARNING( "invalid string passed to bool parser: %s", copy.c_str() );
        err++;
        *p_value = false;
    }

    return err;
}

int parse( const std::string& str, std::vector< unsigned char >* p_value )
{
    if( p_value == NULL ) return 1;

    std::string upper( str );
    ws_to_upper( upper );

    const char* hex = strstr( upper.c_str(), "0X" );
    if( hex == NULL )
        hex = upper.c_str();
    else
        hex += 2;

    size_t len = strlen( hex );

    for( size_t i = 0; i < len; i+= 2 )
    {
        unsigned char c = ws_get_hex_byte( hex + i );
        p_value->push_back( c );
    }

    return 0;
}

//
// just for compatibility with templates
//
int parse( const std::string& str, std::string* p_value )
{
    if( p_value == NULL ) return 1;

    p_value->assign( str );

    return 0;
}

void ws_to_upper( std::string& r_str )
{
    for( unsigned int i = 0; i < r_str.length(); i++ )
    {
        r_str[ i ] = toupper( r_str[ i ] );
    }
}

void ws_to_lower( std::string& r_str )
{
    for( unsigned int i = 0; i < r_str.length(); i++ )
    {
        r_str[ i ] = tolower( r_str[ i ] );
    }
}

std::vector< std::string > ws_strtok_sep( const char* str, const char* delim )
{
    std::vector< std::string > v;
    char buf[ STRTOK_COPY_STRING_BUF_LENGTH ];
    memset( buf, 0x00, sizeof( buf ) );

    if( str == NULL ) return v;
    if( delim == NULL ) return v;

    strncpy( buf, str, STRTOK_COPY_STRING_BUF_LENGTH - 1 );

    char* strtok_save = NULL;
    char* tok = strtok_r( buf, delim, &strtok_save );

    while( tok )
    {
        v.push_back( std::string( tok ) );
        tok = strtok_r( NULL, delim, &strtok_save );
    }

    return v;
}

std::deque< std::string > ws_to_deque( const std::vector< std::string >& rc_v )
{
    std::deque< std::string > d;

    std::vector< std::string >::const_iterator i = rc_v.cbegin();
    for(; i != rc_v.cend(); ++i )
    {
        d.push_back( *i );
    }

    return d;
}

std::deque< std::string > ws_tokenize( const char* str,
                                       const char* omit,
                                       const char* include,
                                       const char* forbid,
                                       bool handle_quotes )
{
    std::deque< std::string > d;
    std::string current;
    bool has_content = false;

    if( str == NULL ) return d;
    if( omit == NULL ) omit = "";
    if( include == NULL ) include = "";
    if( forbid == NULL ) forbid = "";

    int l = strlen( str );
    for( int i = 0; i < l; ++i )
    {
        char c = str[ i ];
        if( ( c == '\"' ) && handle_quotes )
        {
            if( has_content )
            {
                d.push_back( current );
                has_content = false;
                current.clear();
            }

            const char* begin_quote = str + i;
            const char* end_quote = strchr( str + i + 1, '\"' );
            if( !end_quote )
            {
                WS_ERROR( "unclosed quote" );
                d.clear();
                return d;
            }

            d.push_back( std::string( begin_quote + 1 ).
                substr( 0, end_quote - begin_quote - 1 ) );

            i += end_quote - begin_quote;
        }
        else
        {
            if( strchr( forbid, c ) )
            {
                WS_ERROR( "forbidden character found" );
                d.clear();
                return d;
            }
            else if( strchr( omit, c ) )
            {
                if( has_content )
                {
                    d.push_back( current );
                    has_content = false;
                    current.clear();
                }
            }
            else if( strchr( include, c ) )
            {
                if( has_content )
                {
                    d.push_back( current );
                    has_content = false;
                    current.clear();
                }

                current.push_back( c );
                d.push_back( current );
                current.clear();
            }
            else
            {
                current.push_back( c );
                has_content = true;
            }
        }
    }

    return d;
}

void ws_to_hex( char* buf, unsigned char x )
{
    if( buf == NULL ) return;

    const char* hex = "0123456789ABCDEF";

    buf[ 0 ] = hex[ x >> 4 ];
    buf[ 1 ] = hex[ x & 0x0F ];
}

unsigned char ws_get_hex_byte( const char* str )
{
    unsigned char c = 0;

    if( str == NULL ) return 0;
    if( str[ 0 ] == '\0' ) return 0;
    c = ws_decode_hex( str[ 0 ] ) << 4;
    if( str[ 1 ] == '\0' ) return c;
    c = c | ws_decode_hex( str[ 1 ] );

    return c;
}

unsigned char ws_decode_hex( char c )
{
    const char* hex = "0123456789ABCDEF";
    const char* loc = strchr( hex, c );
    if( loc == NULL ) return 0;
    return ( ( unsigned char )( loc - hex ) & 0x0F );
}

std::string ws_strip_comments( std::string str )
{
    std::string out;
    out.reserve( str.length() );

    bool in_comment = false;
    const char* p_str = str.c_str();
    for( size_t i = 0; i < str.length(); ++i )
    {
        if( in_comment )
        {
            if( p_str[ i ] == '\n' ) in_comment = false;
        }
        else
        {
            if( p_str[ i ] == '#' )
            {
                in_comment = true;
            }
            else
            {
                out.push_back( p_str[ i ] );
            }
        }
    }

    return out;
}

bool ws_contains_any_of( const char* haystack, const char* needle )
{
    if( haystack == NULL ) return false;
    if( needle == NULL ) return false;

    while( *needle != '\0' )
    {
        if( strchr( haystack, *( needle++ ) ) != NULL ) return true;
    }

    return false;
}

bool ws_is_well_formed_name( const char* name )
{
    if( name == NULL ) return false;

    for(; *name != '\0'; name++ )
    {
        if( !isalpha( *name ) && 
            !isdigit( *name ) && 
            ( *name != '_' ) )
            return false;
    }

    return true;
}

