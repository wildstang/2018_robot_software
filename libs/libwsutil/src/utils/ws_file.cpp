#include "ws_file.h"

#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "logger/WsLogger.h"

std::string ws_get_file_contents_string( std::string filename )
{
    std::string s;

    long bytes = 0;
    char* buff = ws_get_file_contents_binary( filename, &bytes );
    if( buff == NULL )
        return s;

    s = std::string( buff ).substr( 0, bytes );

    free( buff );

    return s;
}

#define READ_INCREMENT 1024
char* ws_get_file_contents_binary( std::string filename, long* p_bytes )
{
    if( p_bytes == NULL ) return NULL;

    FILE* p_file = fopen( filename.c_str(), "r" );
    if( p_file == NULL )
    {
        WS_ERROR( "could not open the file %s", filename.c_str() );
        return NULL;
    }

    fseek( p_file, 0, SEEK_END );
    *p_bytes = ftell( p_file );
    rewind( p_file );

    if( *p_bytes < 0 )
    {
        WS_ERROR( "issue reading the length of the file %s", filename.c_str() );
        fclose( p_file );
        return NULL;
    }

    char* buff = ( char* )malloc( ( *p_bytes + 1 ) * sizeof( char ) );
    memset( buff, 0x00, ( *p_bytes + 1 ) * sizeof( char ) );

    size_t bytes_remaining = ( size_t )( *p_bytes );
    size_t pos = 0;
    while( bytes_remaining > 0 )
    {
        size_t bytes_to_read = bytes_remaining;
        if( bytes_to_read > READ_INCREMENT )
            bytes_to_read = READ_INCREMENT;

        size_t bytes_read = fread( buff + pos, 1, bytes_to_read, p_file );
        if( bytes_read != bytes_to_read )
        {
            int __bytes_read = ( int )bytes_read;
            int __bytes_to_read = ( int )bytes_to_read;
            WS_ERROR( "incorrect read size %d %d",
                __bytes_read, __bytes_to_read );
            free( buff );
            fclose( p_file );
            return NULL;
        }

        bytes_remaining -= bytes_read;
        pos += bytes_read;
    }

    fclose( p_file );

    return buff;
}

#define WRITE_INCREMENT 1024
int ws_write_file( std::string filename, const char* data, long bytes )
{
    FILE* p_file = fopen( filename.c_str(), "wb" );
    if( p_file == NULL )
    {
        WS_ERROR( "unable to open file %s for write", filename.c_str() );
        return 1;
    }

    size_t pos = 0;
    while( bytes > 0 )
    {
        size_t bytes_to_write = bytes;
        if( bytes_to_write > WRITE_INCREMENT )
            bytes_to_write = WRITE_INCREMENT;

        size_t bytes_written = fwrite( data + pos, 1, bytes_to_write, p_file );
        if( bytes_written != bytes_to_write )
        {
            WS_ERROR( "wrong number of bytes written." );
            fclose( p_file );
            return 1;
        }

        pos += bytes_to_write;
        bytes -= bytes_to_write;
    }

    fclose( p_file );

    return 0;
}

int ws_write_file( std::string filename, std::string data )
{
    return ws_write_file( filename, data.c_str(), data.length() );
}

std::string ws_get_current_working_directory( void )
{
    std::string s;

    char wd[ 2048 ];
    memset( wd, 2048, 0x00 * sizeof( char ) );

    if( !getcwd( wd, sizeof( wd ) ) )
    {
        WS_ERROR( "failed to get the current working directory" );
        return s;
    }

    s = std::string( wd );

    return s;
}

int ws_get_file_listing( const std::string& rc_directory, 
                         std::vector< std::string >& r_list )
{
    int err = 0;
    r_list.clear();

    DIR* p_directory = opendir( rc_directory.c_str() );
    if( p_directory == NULL )
    {
        WS_ERROR( "could not open directory %s", rc_directory.c_str() );
        return 1;
    }

    struct dirent* p_entry;
    while( ( p_entry = readdir( p_directory ) ) != NULL )
    {
        std::string filename = rc_directory + "/" + p_entry->d_name;
        struct stat stbuf;

        if( stat( filename.c_str(), &stbuf ) == -1 )
        {
            WS_ERROR( "cannot stat %s", filename.c_str() );
            continue;
        }

        if( ( stbuf.st_mode & S_IFMT ) == S_IFDIR )
        {
            // is a directory
            continue;
        }

        r_list.push_back( filename );
    }

    closedir( p_directory );

    return err;
}

