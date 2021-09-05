#ifndef __WS_STRING_H__
#define __WS_STRING_H__

#include <vector>
#include <deque>
#include <string>

void ws_trim( std::string& r_str );

std::string ws_std_string_printf( const char* fmt, ... );
std::string ws_std_string_vprintf( const char* fmt, va_list args );

std::string toString( double x );
std::string toString( int x );
std::string toString( bool x );
std::string toString( const std::vector< unsigned char >& rc_v );
std::string toString( uint64_t x );
std::string toString( std::string s );

int parse( const std::string& str, double* p_value );
int parse( const std::string& str, int* p_value );
int parse( const std::string& str, bool* p_value );
int parse( const std::string& str, std::vector< unsigned char >* p_value );
int parse( const std::string& str, std::string* p_value );

bool ws_contains_any_of( const char* haystack, const char* needle );
bool ws_is_well_formed_name( const char* name );

void ws_to_upper( std::string& r_str );
void ws_to_lower( std::string& r_str );

std::vector< std::string > ws_strtok_sep( const char* str, const char* delim );
std::deque< std::string > ws_to_deque( const std::vector< std::string >& rc_v );
std::deque< std::string > ws_tokenize( const char* str,
                                       const char* omit,
                                       const char* include,
                                       const char* forbid,
                                       bool handle_quotes = false );
std::string ws_strip_comments( std::string str );

void ws_to_hex( char* buf, unsigned char x );
unsigned char ws_get_hex_byte( const char* str );
unsigned char ws_decode_hex( char c );

#endif

