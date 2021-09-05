#include "PidController.h"

#include <cmath>

#include "pid/PidInput.h"
#include "pid/PidOutput.h"
#include "logger/WsLogger.h"
#include "core/RobotCore.h"
#include "config/ConfigValueRef.h"

PidController::PidController( PidInput* p_source,
                              PidOutput* p_output,
                              std::string pid_controller_name,
                              WildstangModule* p_parent )
    : WildstangModule( pid_controller_name, p_parent )
    , a_p( 1.0 )
    , a_i( 0.0 )
    , a_d( 0.0 )
    , a_pNeg( 1.0 )
    , a_iNeg( 0.0 )
    , a_dNeg( 0.0 )
    , a_biDirectionalPid( false )
    , a_currentError( 0.0 )
    , a_previousError( 0.0 )
    , a_setPoint( 0.0 )
    , a_errorSum( 0.0 )
    , a_errorIncrement( 1.0 )
    , a_errorEpsilon( 1.0 )
    , a_deadZoneEpsilon( 0.0 )
    , a_stallSpeed( 0.0 )
    , a_maxIntegral( 0.5 )
    , a_integralErrorThresh( -1.0 )
    , a_maxDifferential( 0.5 )
    , a_maxOutput( 1.0 )
    , a_maxInput( 10000.0 )
    , a_currentState( PID_STATE_INITIALIZE )
    , a_minOnTargetTime( 0.2 )
    , a_stabilizationTimer()
    , a_stallTimer()
    , ap_pidSource( p_source )
    , ap_pidOutput( p_output )
    , a_controllerName( pid_controller_name )
    , a_output( 0.0 )
    , a_stalled( false )
    , a_reverseOutput( false )
    , a_sourceValueProbe( "source_value", 0.0 )
    , a_setPointProbe( "set_point", 0.0 )
    , a_errorProbe( "error", 0.0 )
    , a_pidStateProbe( "pid_state", std::string( "" ) )
    , a_errorSumProbe( "error_sum", 0.0 )
    , a_outputProbe( "output_value", 0.0 )
    , a_proportionalTermProbe( "proportional", 0.0 )
    , a_integralTermProbe( "integral", 0.0 )
    , a_differentialTermProbe( "differential", 0.0 )
    , a_velocityProbe( "velocity", 0.0 )
    , a_stalledProbe( "stalled", false )
    , ap_logFile( NULL )
{
    CONFIG_VAR( "p", a_p, double );
    CONFIG_VAR( "i", a_i, double );
    CONFIG_VAR( "d", a_d, double );
    CONFIG_VAR( "p_neg", a_pNeg, double );
    CONFIG_VAR( "i_neg", a_iNeg, double );
    CONFIG_VAR( "d_neg", a_dNeg, double );
    CONFIG_VAR( "bi_directional_pid", a_biDirectionalPid, bool );
    CONFIG_VAR( "error_increment_percent", a_errorIncrement, double );
    CONFIG_VAR( "error_epsilon", a_errorEpsilon, double );
    CONFIG_VAR( "dead_zone_epsilon", a_deadZoneEpsilon, double );
    CONFIG_VAR( "stall_speed", a_stallSpeed, double );
    CONFIG_VAR( "max_integral_term", a_maxIntegral, double );
    CONFIG_VAR( "integral_error_thresh", a_integralErrorThresh, double );
    CONFIG_VAR( "max_differential_term", a_maxDifferential, double );
    CONFIG_VAR( "input_clip", a_maxInput, double );
    CONFIG_VAR( "output_clip", a_maxOutput, double );
    CONFIG_VAR( "min_on_target_time", a_minOnTargetTime, double );
    CONFIG_VAR( "invert_output", a_reverseOutput, bool );

    if( ap_pidSource == NULL )
    {
        WS_CRITICAL( "You didn't give us a pid source, %s",
                     a_controllerName.c_str() );
    }

    if( ap_pidOutput == NULL )
    {
        WS_CRITICAL( "You didn't give us a pid output, %s",
                     a_controllerName.c_str() );
    }

    WS_INFO( "PidController constructor %s",
             pid_controller_name.c_str() );

    this->initProbes();

    a_stallTimer.reset();
    a_stallTimer.start();
    a_stalled = false;
}

PidController::~PidController( void )
{
    WS_INFO( "PidController destructor %s", this->getModuleName().c_str() );

    if( ( void* )ap_pidSource == ( void* )ap_pidOutput )
    {
        delete ap_pidSource;
        ap_pidSource = NULL;
        ap_pidOutput = NULL;
    }
    else
    {
        delete ap_pidSource;
        ap_pidSource = NULL;

        delete ap_pidOutput;
        ap_pidOutput = NULL;
    }
}

double PidController::getMaxErrorIncrement( void )
{
    return ( ( 2.0 * a_maxOutput ) *
             ( a_errorIncrement / 100.0 ) );
}

void PidController::calcPid( void )
{
    if( a_currentState == PID_STATE_DISABLED )
    {
        WS_WARNING( "%s is DISABLED", this->a_controllerName.c_str() );
        this->reset();
        return;
    }

    pidStateT new_state = a_currentState;

    double current_pv = ap_pidSource->pidRead();

    a_currentError = a_setPoint - current_pv;
    a_currentError = this->limitInput( a_currentError );

    switch( a_currentState )
    {
        case PID_STATE_INITIALIZE:
            // Neutralize error difference for first iteration
            a_previousError = a_currentError;

            if( fabs( a_currentError ) <= a_errorEpsilon )
            {
                new_state = PID_STATE_ON_TARGET;
                a_stabilizationTimer.reset();
                a_stabilizationTimer.start();
            }
            else if( a_currentError <= 0.0 )
            {
                // Negative errors mean we are above our setpoint
                new_state = PID_STATE_ABOVE_TARGET;

                this->checkIntegralFighting();
            }
            else if( a_currentError >= 0.0 )
            {
                // Positive Errors mean we are below our setpoint
                new_state = PID_STATE_BELOW_TARGET;

                this->checkIntegralFighting();
            }
            break;

        case PID_STATE_BELOW_TARGET:
            a_errorSum += this->getErrorIncrement();

            if( fabs( a_currentError ) <= a_errorEpsilon )
            {
                new_state = PID_STATE_ON_TARGET;
                a_stabilizationTimer.reset();
                a_stabilizationTimer.start();
            }
            else if( a_currentError <= 0.0 )
            {
                new_state = PID_STATE_ABOVE_TARGET;

                this->checkIntegralFighting();
            }
            break;

        case PID_STATE_ON_TARGET:
            if( a_stabilizationTimer.hasPeriodPassed( a_minOnTargetTime ) )
            {
                new_state = PID_STATE_STABILIZED;
                a_stabilizationTimer.stop();
                a_stabilizationTimer.reset();
            }

            //
            // intentional fall through
            //

        case PID_STATE_STABILIZED:
            if( a_currentError >= a_errorEpsilon )
            {
                new_state = PID_STATE_BELOW_TARGET;
                a_stabilizationTimer.stop();
                a_stabilizationTimer.reset();

                this->checkIntegralFighting();
            }
            else if( a_currentError <= ( -1.0 * a_errorEpsilon ) )
            {
                new_state = PID_STATE_ABOVE_TARGET;
                a_stabilizationTimer.stop();
                a_stabilizationTimer.reset();

                this->checkIntegralFighting();
            }
            break;

        case PID_STATE_ABOVE_TARGET:
            a_errorSum -= this->getErrorIncrement();

            if( fabs( a_currentError ) <= a_errorEpsilon )
            {
                new_state = PID_STATE_ON_TARGET;
                a_stabilizationTimer.reset();
                a_stabilizationTimer.start();
            }
            else if( a_currentError >= 0.0 )
            {
                new_state = PID_STATE_BELOW_TARGET;

                this->checkIntegralFighting();
            }
            break;

        default:
            WS_ERROR( "invalid state in pid controller %s",
                      this->a_controllerName.c_str() );

            new_state = PID_STATE_DISABLED;
            break;
    }

    double p_term = this->calcProportionalTerm();
    double i_term = this->calcIntegralTerm();
    double d_term = this->calcDerivativeTerm();
    a_output = p_term + i_term + d_term;
    // TODO: d term is supposed to be negative. I think I'm missing something
    // with how the order of previous and current work sign wise

    if( fabs( a_currentError ) < fabs( a_deadZoneEpsilon ) )
    {
        a_output = 0.0;
        //WS_INFO( "resetting the error sum: %f, dead zone", a_errorSum );
        a_errorSum = 0.0;
    }

    a_output = this->limitOutput( a_output );
    if( a_reverseOutput ) a_output = -1.0 * a_output;
    ap_pidOutput->pidWrite( a_output );

    if( ap_logFile )
    {
        fprintf( ap_logFile, "%s: error: %.2f set_point %.2f "
                             "output: %.4f p: %.4f i: %.4f d: %.4f\n",
                 a_controllerName.c_str(), a_currentError,
                 a_setPoint, a_output, p_term, i_term, d_term );
    }

    a_currentState = new_state;
    a_velocity = a_currentError - a_previousError;
    a_previousError = a_currentError;

    this->updateStallState();

    a_sourceValueProbe.update( current_pv );
    a_setPointProbe.update( a_setPoint );
    a_errorProbe.update( a_currentError );
    a_pidStateProbe.update( toString( a_currentState ) );
    a_proportionalTermProbe.update( p_term );
    a_integralTermProbe.update( i_term );
    a_differentialTermProbe.update( d_term );
    a_errorSumProbe.update( a_errorSum );
    a_outputProbe.update( a_output );
    a_velocityProbe.update( a_velocity );
    a_stalledProbe.update( a_stalled );
}

bool PidController::isOnTarget( void )
{
    bool on_target = ( ( PID_STATE_ON_TARGET == a_currentState ) ||
                       ( PID_STATE_STABILIZED == a_currentState ) );
    return on_target;
}

bool PidController::isStabilized( void )
{
    bool stabilized = ( PID_STATE_STABILIZED == a_currentState );
    return stabilized;
}

bool PidController::isStalled( void )
{
    return a_stalled;
}

bool PidController::isEnabled( void )
{
    return ( PID_STATE_DISABLED != a_currentState );
}

void PidController::enable( void )
{
    if( a_currentState == PID_STATE_DISABLED )
    {
        a_currentState = PID_STATE_INITIALIZE;
    }
}

void PidController::disable( void )
{
    this->reset();
    a_currentState = PID_STATE_DISABLED;
}

void PidController::reset( void )
{
    //WS_INFO( "reset called, resetting error sum %f", a_errorSum );
    a_currentState = PID_STATE_INITIALIZE;
    a_errorSum = 0.0;
    a_currentError = 0.0;
    a_previousError = 0.0;
    a_stallTimer.reset();
    a_stallTimer.start();
    a_stalled = false;
}

void PidController::setSetPoint( double set_point )
{
    a_setPoint = set_point;
}

double PidController::calcProportionalTerm( void )
{
    double p_term = this->getP() * a_currentError;
    return p_term;
}

double PidController::calcIntegralTerm( void )
{
    // Prevent Integral Wind Up.
    if( a_integralErrorThresh > 0.0 )
    {
        if( fabs( a_currentError ) > fabs( a_integralErrorThresh ) )
        {
            //WS_INFO( "resetting error sum, calc integral term %f", a_errorSum );
            a_errorSum = 0.0;
        }
    }

    double i_term = this->getI() * a_errorSum;

    if( a_maxIntegral > 0.0 )
    {
        if( i_term > a_maxIntegral )
        {
            i_term = a_maxIntegral;
        }
        else if( i_term < ( -1.0 * a_maxIntegral ) )
        {
            i_term = ( -1 * a_maxIntegral );
        }
    }

    return i_term;
}

double PidController::calcDerivativeTerm( void )
{
    double d_term = this->getD() * ( a_currentError - a_previousError );

    if( a_maxDifferential > 0.0 )
    {
        if( fabs( d_term ) > fabs( a_maxDifferential ) )
        {
            d_term = ( d_term > 0.0 ) ? a_maxDifferential :
                                        ( -1.0 * a_maxDifferential );
        }
    }

    return d_term;
}

double PidController::limitOutput( double output )
{
    if( output > a_maxOutput )
    {
        output = a_maxOutput;
    }
    else if( output < ( -1.0 * a_maxOutput ) )
    {
        output = -1.0 * a_maxOutput;
    }

    return output;
}

double PidController::limitInput( double input )
{
    if( input > a_maxInput )
    {
        input = a_maxInput;
    }
    else if( input < ( -1.0 * a_maxInput ) )
    {
        input = -1.0 * a_maxInput;
    }

    return input;
}

std::string toString( pidStateT state )
{
    std::string str;

    switch( state )
    {
        case PID_STATE_DISABLED:
            str = std::string( "pid_state_disabled" );
            break;
        case PID_STATE_INITIALIZE:
            str = std::string( "pid_state_initialize" );
            break;
        case PID_STATE_BELOW_TARGET:
            str = std::string( "pid_state_below_target" );
            break;
        case PID_STATE_ON_TARGET:
            str = std::string( "pid_state_on_target" );
            break;
        case PID_STATE_STABILIZED:
            str = std::string( "pid_state_stabilized" );
            break;
        case PID_STATE_ABOVE_TARGET:
            str = std::string( "pid_state_above_target" );
            break;
        case PID_STATE_NUM:
        case PID_STATE_NONE:
        default:
            str = std::string( "invalid_state" );
            break;
    }

    return str;
}

int PidController::initProbes( void )
{
    this->addProbe( &a_sourceValueProbe );
    this->addProbe( &a_setPointProbe );
    this->addProbe( &a_errorProbe );
    this->addProbe( &a_pidStateProbe );
    this->addProbe( &a_errorSumProbe );
    this->addProbe( &a_outputProbe );
    this->addProbe( &a_proportionalTermProbe );
    this->addProbe( &a_integralTermProbe );
    this->addProbe( &a_differentialTermProbe );
    this->addProbe( &a_velocityProbe );
    this->addProbe( &a_stalledProbe );

    return 0;
}

int PidController::enableFileLogging( std::string filename )
{
    if( ap_logFile != NULL ) fclose( ap_logFile );
    ap_logFile = fopen( filename.c_str(), "w" );

    if( ap_logFile == NULL )
    {
        WS_ERROR( "couldn't open the file %s for write", filename.c_str() );
    }

    return ( ap_logFile != NULL ) ? 0 : 1;
}

int PidController::disableFileLogging( void )
{
    if( ap_logFile != NULL )
    {
        fclose( ap_logFile );
        ap_logFile = NULL;
    }
    return 0;
}

double PidController::getErrorIncrement( void )
{
    double error_increment = this->getMaxErrorIncrement();

    if( fabs( a_currentError ) < fabs( error_increment ) )
    {
        error_increment = fabs( a_currentError );
    }

    return error_increment;
}

void PidController::updateStallState( void )
{
    if( fabs( a_velocity ) > fabs( a_stallSpeed ) )
    {
        a_stallTimer.reset();
        a_stallTimer.start();
    }

    a_stalled = a_stallTimer.hasPeriodPassed( a_minOnTargetTime );
}

void PidController::checkIntegralFighting( void )
{
    switch( a_currentState )
    {
        case PID_STATE_BELOW_TARGET:
            if( a_errorSum < 0.0 )
            {
                // If we are fighting away from the point, reset the error.
                //WS_INFO( "resetting the error sum: %f, below target", a_errorSum );
                a_errorSum = 0.0;
            }
            break;

        case PID_STATE_ABOVE_TARGET:
            if( a_errorSum > 0.0 )
            {
                // If we are fighting away from the point, reset the error.
                //WS_INFO( "resetting the error sum: %f, above target", a_errorSum );
                a_errorSum = 0.0;
            }
            break;

        default:
            break;
    }
}

double PidController::getP( void )
{
    if( a_biDirectionalPid )
    {
        if( a_currentError < 0.0 )
        {
            return a_p;
        }
        else
        {
            return a_pNeg;
        }
    }
    else
    {
        return a_p;
    }
}

double PidController::getI( void )
{
    if( a_biDirectionalPid )
    {
        if( a_currentError < 0.0 )
        {
            return a_i;
        }
        else
        {
            return a_iNeg;
        }
    }
    else
    {
        return a_i;
    }
}

double PidController::getD( void )
{
    if( a_biDirectionalPid )
    {
        if( a_currentError < 0.0 )
        {
            return a_d;
        }
        else
        {
            return a_dNeg;
        }
    }
    else
    {
        return a_d;
    }
}
