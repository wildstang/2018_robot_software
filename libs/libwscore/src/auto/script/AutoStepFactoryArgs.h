#ifndef __AUTOSTEPFACTORYARGS_H__
#define __AUTOSTEPFACTORYARGS_H__

#include <string>
#include <vector>
#include <map>
#include <deque>

typedef enum
{
    STEP_NONE = 0,

    //
    // Named args
    //

    STEP_NAME,
    STEP_SPEED,
    STEP_INCHES,
    STEP_FILE,
    STEP_STATE,
    STEP_ANGLE,
    STEP_DISTANCE,
    STEP_F_GAIN,
    STEP_MILLISECONDS,
    STEP_DIRECTION,
    STEP_POSITION,
    STEP_TIMEOUT,
    STEP_PROGRAM,
    STEP_REVERSED,

    //
    // Generic Args
    //

    STEP_ARG_1,
    STEP_ARG_2,
    STEP_ARG_3,
    STEP_ARG_4,
    STEP_ARG_5,
    STEP_ARG_6,
    STEP_ARG_7,
    STEP_ARG_8,
    STEP_ARG_9,
    STEP_ARG_10,

    STEP_NUM
} autoStepFactoryArgTypeT;

std::string toString( autoStepFactoryArgTypeT arg );
int parse( const std::string& rc_str, autoStepFactoryArgTypeT& r_arg );

class AutoStepFactoryArgs
{
    public:
        AutoStepFactoryArgs( void );
        ~AutoStepFactoryArgs( void );

        int parse( std::deque< std::string >& r_tok );

        int get( autoStepFactoryArgTypeT arg, std::string& r_v );
        int get( autoStepFactoryArgTypeT arg, double& r_v );
        int get( autoStepFactoryArgTypeT arg, int& r_v );
        int get( autoStepFactoryArgTypeT arg, unsigned int& r_v );
        int get( autoStepFactoryArgTypeT arg, bool& r_v );

        int set( autoStepFactoryArgTypeT arg, const std::string& rc_v );
        int set( autoStepFactoryArgTypeT arg, const double& rc_v );
        int set( autoStepFactoryArgTypeT arg, const int& rc_v );
        int set( autoStepFactoryArgTypeT arg, const unsigned int& rc_v );
        int set( autoStepFactoryArgTypeT arg, const bool& rc_v );

        int optional( autoStepFactoryArgTypeT arg, std::string& r_v );
        int optional( autoStepFactoryArgTypeT arg, double& r_v );
        int optional( autoStepFactoryArgTypeT arg, int& r_v );
        int optional( autoStepFactoryArgTypeT arg, unsigned int& r_v );
        int optional( autoStepFactoryArgTypeT arg, bool& r_v );

        bool exists( autoStepFactoryArgTypeT arg );

        inline int getName( std::string& r_v ) { return get( STEP_NAME, r_v ); }
        inline int getSpeed( double& r_v ) { return get( STEP_SPEED, r_v ); }
        inline int getInches( int& r_v ) { return get( STEP_INCHES, r_v ); }
        inline int getInches( double& r_v ) { return get( STEP_INCHES, r_v ); }
        inline int getFile( std::string& r_v ) { return get( STEP_FILE, r_v ); }
        inline int getState( bool& r_v ) { return get( STEP_STATE, r_v ); }
        inline int getState( int& r_v ) { return get( STEP_STATE, r_v ); }
        inline int getAngle( double& r_v ) { return get( STEP_ANGLE, r_v ); }
        inline int getAngle( int& r_v ) { return get( STEP_ANGLE, r_v ); }
        inline int getDistance( int& r_v ) { return get( STEP_DISTANCE, r_v ); }
        inline int getDistance( double& r_v ) { return get( STEP_DISTANCE, r_v ); }
        inline int getFGain( double& r_v ) { return get( STEP_F_GAIN, r_v ); }
        inline int getMilliseconds( int& r_v ) { return get( STEP_MILLISECONDS, r_v ); }
        inline int getDirection( int& r_v ) { return get( STEP_DIRECTION, r_v ); }
        inline int getDirection( double& r_v ) { return get( STEP_DIRECTION, r_v ); }
        inline int getPosition( int& r_v ) { return get( STEP_POSITION, r_v ); }
        inline int getPosition( double& r_v ) { return get( STEP_POSITION, r_v ); }
        inline int getTimeout( double& r_v ) { return get( STEP_TIMEOUT, r_v ); }
        inline int getProgram( std::string& r_v ) { return get( STEP_PROGRAM, r_v ); }
        inline int getReversed( bool& r_v ) { return get( STEP_REVERSED, r_v ); }

        inline int optionalSpeed( double& r_v ) { return optional( STEP_SPEED, r_v ); }
        inline int optionalInches( int& r_v ) { return optional( STEP_INCHES, r_v ); }
        inline int optionalInches( double& r_v ) { return optional( STEP_INCHES, r_v ); }
        inline int optionalFile( std::string& r_v ) { return optional( STEP_FILE, r_v ); }
        inline int optionalState( bool& r_v ) { return optional( STEP_STATE, r_v ); }
        inline int optionalState( int& r_v ) { return optional( STEP_STATE, r_v ); }
        inline int optionalAngle( double& r_v ) { return optional( STEP_ANGLE, r_v ); }
        inline int optionalAngle( int& r_v ) { return optional( STEP_ANGLE, r_v ); }
        inline int optionalDistance( int& r_v ) { return optional( STEP_DISTANCE, r_v ); }
        inline int optionalDistance( double& r_v ) { return optional( STEP_DISTANCE, r_v ); }
        inline int optionalFGain( double& r_v ) { return optional( STEP_F_GAIN, r_v ); }
        inline int optionalMilliseconds( int& r_v ) { return optional( STEP_MILLISECONDS, r_v ); }
        inline int optionalDirection( int& r_v ) { return optional( STEP_DIRECTION, r_v ); }
        inline int optionalDirection( double& r_v ) { return optional( STEP_DIRECTION, r_v ); }
        inline int optionalPosition( int& r_v ) { return optional( STEP_POSITION, r_v ); }
        inline int optionalPosition( double& r_v ) { return optional( STEP_POSITION, r_v ); }
        inline int optionalTimeout( double& r_v ) { return optional( STEP_TIMEOUT, r_v ); }
        inline int optionalProgram( std::string& r_v ) { return optional( STEP_PROGRAM, r_v ); }
        inline int optionalReversed( bool& r_v ) { return optional( STEP_REVERSED, r_v ); }

        inline int setName( const std::string& rc_v ) { return set( STEP_NAME, rc_v ); }
        inline int setSpeed( const double& rc_v ) { return set( STEP_SPEED, rc_v ); }
        inline int setInches( const int& rc_v ) { return set( STEP_INCHES, rc_v ); }
        inline int setInches( const double& rc_v ) { return set( STEP_INCHES, rc_v ); }
        inline int setFile( const std::string& rc_v ) { return set( STEP_FILE, rc_v ); }
        inline int setState( const bool& rc_v ) { return set( STEP_STATE, rc_v ); }
        inline int setState( const int& rc_v ) { return set( STEP_STATE, rc_v ); }
        inline int setAngle( const double& rc_v ) { return set( STEP_ANGLE, rc_v ); }
        inline int setAngle( const int& rc_v ) { return set( STEP_ANGLE, rc_v ); }
        inline int setDistance( const int& rc_v ) { return set( STEP_DISTANCE, rc_v ); }
        inline int setDistance( const double& rc_v ) { return set( STEP_DISTANCE, rc_v ); }
        inline int setFGain( const double& rc_v ) { return set( STEP_F_GAIN, rc_v ); }
        inline int setMilliseconds( const int& rc_v ) { return set( STEP_MILLISECONDS, rc_v ); }
        inline int setDirection( const int& rc_v ) { return set( STEP_DIRECTION, rc_v ); }
        inline int setDirection( const double& rc_v ) { return set( STEP_DIRECTION, rc_v ); }
        inline int setPosition( const int& rc_v ) { return set( STEP_POSITION, rc_v ); }
        inline int setPosition( const double& rc_v ) { return set( STEP_POSITION, rc_v ); }
        inline int setTimeout( const double& rc_v ) { return set( STEP_TIMEOUT, rc_v ); }
        inline int setProgram( const std::string& rc_v ) { return set( STEP_PROGRAM, rc_v ); }
        inline int setReversed( const bool& rc_v ) { return set( STEP_REVERSED, rc_v ); }

        inline bool existsName( void ) { return exists( STEP_NAME ); }
        inline bool existsSpeed( void ) { return exists( STEP_SPEED ); }
        inline bool existsInches( void ) { return exists( STEP_INCHES ); }
        inline bool existsFile( void ) { return exists( STEP_FILE ); }
        inline bool existsState( void ) { return exists( STEP_STATE ); }
        inline bool existsAngle( void ) { return exists( STEP_ANGLE ); }
        inline bool existsDistance( void ) { return exists( STEP_DISTANCE ); }
        inline bool existsFGain( void ) { return exists( STEP_F_GAIN ); }
        inline bool existsMilliseconds( void ) { return exists( STEP_MILLISECONDS ); }
        inline bool existsDirection( void ) { return exists( STEP_DIRECTION ); }
        inline bool existsPosition( void ) { return exists( STEP_POSITION ); }
        inline bool existsTimeout( void ) { return exists( STEP_TIMEOUT ); }
        inline bool existsProgram( void ) { return exists( STEP_PROGRAM ); }
        inline bool existsReversed( void ) { return exists( STEP_REVERSED ); }

        inline bool errorsOccurred( void ) { return a_errors; }
        inline void setErrorFlag( void ) { a_errors = true; }

        std::string generateCode( void );

        std::vector< autoStepFactoryArgTypeT > getTouched( void );
        std::vector< std::string > getTouchedStr( void );
        
    protected:
        std::map< autoStepFactoryArgTypeT, std::string > a_args;
        bool a_touched[ STEP_NUM ];
        bool a_errors;
};

#endif

