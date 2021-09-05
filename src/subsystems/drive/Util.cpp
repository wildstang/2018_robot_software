#include "Util.h"

//#include "subsystems/drive/Trajectory.h"

double limit( double v, double limit )
{
    return ( fabs( v ) < limit ) ? v : limit * ( v < 0 ? -1 : 1 );
}

#if 0
Trajectory* readTrajectory( std::string path )
{
    Trajectory* p_trajectory = new Trajectory();

    FILE* p_file = fopen( path.c_str(), "r" );
    if( p_file == NULL )
    {
        fprintf( stderr, "error reading file %s\n", path.c_str() );
        delete p_trajectory;
        return NULL;
    }

    std::vector< trajectoryPointT > data_points;
    std::list< CANTalon::TrajectoryPoint > trajectory_points;

    char buff[ 2048 ];
    memset( buff, 0x00, 2048 * sizeof( char ) );

    while( fgets( buff, 2047, p_file ) )
    {
        if( buff[ 0 ] == '\0' ) continue;
        if( buff[ 0 ] == '\n' ) continue;
        if( buff[ 0 ] == '\r' ) continue;
        if( buff[ 0 ] == '#' ) continue;

        float rot = 0.0f;
        float vel = 0.0f;
        float time = 0.0f;

        trajectoryPointT t;
        int r = sscanf( buff, "%f,%f,%f\n", &rot, &vel, &time );

        t.rotation = ( double )rot;
        t.velocity = ( double )vel;
        t.time = ( double )time;

        if( r != 3 )
        {
            fprintf( stderr, "invalid line in %s: '%s'\n", path.c_str(), buff );
            continue;
        }

        data_points.push_back( t );

        // Create a TrajectoryPoint for the Talon - do this while reading the file
        CANTalon::TrajectoryPoint p;
        p.position = t.rotation;
        p.velocity = t.velocity;
        p.timeDurMs = ( int )t.time;
        // which set of gains would you like to use?
        p.profileSlotSelect = 0;
        // set true to not do any position servo, just velocity feedforward
        p.velocityOnly = false;
        p.zeroPos = false;
        p.isLastPoint = false;

        trajectory_points.push_back( p );
    }

    trajectory_points.begin()->zeroPos = true;
    trajectory_points.rbegin()->isLastPoint = true;

    p_trajectory->setTrajectoryPoints( data_points );
    p_trajectory->setTalonPoints( trajectory_points );

    return p_trajectory;
}
#endif
