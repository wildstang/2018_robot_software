#ifndef __WS_FILE_H__
#define __WS_FILE_H__

#include <string>
#include <vector>

std::string ws_get_file_contents_string( std::string filename );
char* ws_get_file_contents_binary( std::string filename, long* p_bytes );
int ws_write_file( std::string filename, const char* data, long bytes );
int ws_write_file( std::string filename, std::string data );

std::string ws_get_current_working_directory( void );
int ws_get_file_listing( const std::string& rc_directory, 
                         std::vector< std::string >& r_list );

#endif

