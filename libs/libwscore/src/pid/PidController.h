#ifndef __PIDCONTROLLER_H__
#define __PIDCONTROLLER_H__

#include <string>
#include <cstdio>

#include "utils/WsTimer.h"
#include "core/WildstangModule.h"
#include "logger/Probe.h"

class PidInput;
class PidOutput;

typedef enum
{
    PID_STATE_NONE = 0,

    PID_STATE_DISABLED,
    PID_STATE_INITIALIZE,
    PID_STATE_BELOW_TARGET,
    PID_STATE_ON_TARGET,
    PID_STATE_STABILIZED,
    PID_STATE_ABOVE_TARGET,

    PID_STATE_NUM
} pidStateT;

std::string toString( pidStateT state );

class PidController : public WildstangModule
{
    public:
        PidController( PidInput* p_source,
                       PidOutput* p_output,
                       std::string pid_controller_name,
                       WildstangModule* p_parent );
        virtual ~PidController( void );

        int initProbes( void );

        void setSetPoint( double set_point );
        virtual void calcPid( void );

        bool isOnTarget( void );
        bool isStabilized( void );
        bool isEnabled( void );
        bool isStalled( void );

        void enable( void );
        void disable( void );
        void reset( void );

        int enableFileLogging( std::string filename );
        int disableFileLogging( void );

    protected:
        double calcProportionalTerm( void );
        double calcIntegralTerm( void );
        double calcDerivativeTerm( void );

        double getMaxErrorIncrement( void );
        double getErrorIncrement( void );

        double getP( void );
        double getI( void );
        double getD( void );

        void updateStallState( void );
        void checkIntegralFighting( void );

    private:
        double limitOutput( double output );
        double limitInput( double input );

        double a_p;
        double a_i;
        double a_d;
        double a_pNeg;
        double a_iNeg;
        double a_dNeg;
        bool a_biDirectionalPid;

        double a_currentError;
        double a_previousError;
        double a_velocity;
        double a_setPoint;
        double a_errorSum;          // Sum of previous errors (for I calculation)
        double a_errorIncrement;    // Max increment to error sum each call
        double a_errorEpsilon;      // Allowable error in determining when done
        double a_deadZoneEpsilon;   // Allowable error in steady state. Used to
        double a_stallSpeed;        // account for small acceptable motor
                                    // backdrive
        double a_maxIntegral;
        double a_integralErrorThresh;
        double a_maxDifferential;
        double a_maxOutput;         // Ceiling on calculation output
        double a_maxInput;          // Ceiling on calculation input
        pidStateT a_currentState;   // State of the pid for calculating I
        double a_minOnTargetTime;   // Minimum number of cycles in epsilon range
                                    // to be done

        WsTimer a_stabilizationTimer;
        WsTimer a_stallTimer;
        PidInput* ap_pidSource;
        PidOutput* ap_pidOutput;
        std::string a_controllerName;

        double a_output;
        bool a_stalled;

        bool a_reverseOutput;

        ProbeValue< double > a_sourceValueProbe;
        ProbeValue< double > a_setPointProbe;
        ProbeValue< double > a_errorProbe;
        ProbeValue< std::string > a_pidStateProbe;
        ProbeValue< double > a_errorSumProbe;
        ProbeValue< double > a_outputProbe;
        ProbeValue< double > a_proportionalTermProbe;
        ProbeValue< double > a_integralTermProbe;
        ProbeValue< double > a_differentialTermProbe;
        ProbeValue< double > a_velocityProbe;
        ProbeValue< bool > a_stalledProbe;

        FILE* ap_logFile;
};

#endif
