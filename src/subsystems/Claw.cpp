#include "Claw.h"

#include "utils/ws_string.h"
#include "io/routing/OutputSource.h"
#include "io/routing/InputSlot.h"
#include "config/ConfigValueRef.h"

#include "robot/WsRobot.h"

Claw::Claw( void )
    : Subsystem( std::string( "claw" ) )
    , a_intakeSpeedReverse( 0.0 )
    , a_intakeSpeedReverseSlower( 0.0 )
    , a_intakeSpeedReverseSlowest( 0.0 )
    , a_intakeSpeedReverseBallsToWall( 0.0 )
    , a_intakeSpeedForwardLeft( 0.0 )
    , a_intakeSpeedForwardRight( 0.0 )
    , a_reverseIntakeDirection( false )
    , a_intakeLeft( 0.0 )
    , a_intakeRight( 0.0 )
    , a_solenoidIn( false )
    , a_solenoidOut( false )
    , a_leftSwitch( false )
    , a_rightSwitch( false )
    , a_intakeForwardButton( false )
    , a_intakeReverseButton( false )
    , a_intakePositionButton( false )
    , a_intakeOpenLock( false )
    , a_dropMomentary( false )
    , a_spitMomentary( false )
    , a_closeLock( false )
    , a_outtaking( false )
    , a_outtakingPrev( false )
    , a_intakeDir( INTAKE_DIR_NEU )
    , a_clawState( CLAW_STATE_RAGDOLL )
    , a_clawStateProbe( "claw_state", 0 )
    , a_intakeDirProbe( "intake_dir", 0 )
    , a_oscillationPeriod( 1.0 )
    , a_oscillationFactor( 0.4 )
    , a_oscillationSide( false )
    , a_oscillationTimer()
{
    CONFIG_VAR( "intake_speed_reverse", a_intakeSpeedReverse, double );
    CONFIG_VAR( "intake_speed_reverse_slower", a_intakeSpeedReverseSlower, double );
    CONFIG_VAR( "intake_speed_reverse_slowest", a_intakeSpeedReverseSlowest, double );
    CONFIG_VAR( "intake_speed_reverse_balls_to_wall", a_intakeSpeedReverseBallsToWall, double);
    CONFIG_VAR( "intake_speed_forward_left", a_intakeSpeedForwardLeft, double );
    CONFIG_VAR( "intake_speed_forward_right", a_intakeSpeedForwardRight, double );
    CONFIG_VAR( "reverse_intake_direction", a_reverseIntakeDirection, bool );
    CONFIG_VAR( "oscillation_period", a_oscillationPeriod, double );
    CONFIG_VAR( "oscillation_factor", a_oscillationFactor, double );
}

Claw::~Claw( void )
{
}

void Claw::init( void )
{
    resetState();

    CONNECT_INPUT( "intake_button", inputIntakeButton, Claw, bool );
    CONNECT_INPUT( "outtake_button", inputOuttakeButton, Claw, bool );

    //CONNECT_INPUT( "claw_open_lock_button", inputClawOpen, Claw, bool );
    //CONNECT_INPUT( "claw_un_open_lock_button", inputClawUnOpen, Claw, bool );
    CONNECT_INPUT( "claw_close_lock_button", inputClawCloseLock, Claw, bool );
    CONNECT_INPUT( "claw_drop_button", inputClawDrop, Claw, bool );
    CONNECT_INPUT( "claw_drop_spit_button", inputClawSpit, Claw, bool );

    CONNECT_INPUT( "claw_left_switch", inputLeftSwitch, Claw, bool );
    CONNECT_INPUT( "claw_right_switch", inputRightSwitch, Claw, bool );
    CONNECT_INPUT( "intake_position_button", inputIntakePosition, Claw, bool );

    CONNECT_OUTPUT( "claw_in", a_solenoidIn, bool );
    CONNECT_OUTPUT( "claw_out", a_solenoidOut, bool );
    CONNECT_OUTPUT( "intake_left", a_intakeLeft, double );
    CONNECT_OUTPUT( "intake_right", a_intakeRight, double );

    this->addProbe( &a_clawStateProbe );
    this->addProbe( &a_intakeDirProbe );
}

void Claw::resetState( void )
{
    a_clawState = CLAW_STATE_RAGDOLL;
    a_intakeDir = INTAKE_DIR_NEU;
}

void Claw::update( void )
{
    if( a_intakeDir == INTAKE_DIR_FWD )
    {
        a_intakeLeft = -1.0 * a_intakeSpeedForwardLeft;
        a_intakeRight = a_intakeSpeedForwardRight;

        if( !( a_leftSwitch && a_rightSwitch ) )
        {
            if( a_leftSwitch )
            {
                a_intakeLeft = 0.0;
            }

            if( a_rightSwitch )
            {
                a_intakeRight = 0.0;
            }
        }

        if( a_oscillationPeriod > 0.0 )
        {
            if( !( a_leftSwitch || a_rightSwitch ) )
            {
                if( a_oscillationTimer.hasPeriodPassed( a_oscillationPeriod ) )
                {
                    a_oscillationTimer.reset();
                    a_oscillationTimer.start();

                    a_oscillationSide = !a_oscillationSide;
                }
            }

            a_intakeLeft = a_oscillationSide ? a_intakeLeft : 
                                ( a_intakeLeft * a_oscillationFactor );
            a_intakeRight = a_oscillationSide ? 
                                ( a_intakeRight * a_oscillationFactor ) :
                                a_intakeRight;
        }
    }
    else if( a_intakeDir == INTAKE_DIR_REV )
    {
        a_intakeLeft = a_intakeSpeedReverse;
        a_intakeRight = -1.0 * a_intakeSpeedReverse;
    }
    else if( a_intakeDir == INTAKE_DIR_REV_SLOWER )
    {
        a_intakeLeft = a_intakeSpeedReverseSlower;
        a_intakeRight = -1.0 * a_intakeSpeedReverseSlower;
    }
    else if( a_intakeDir == INTAKE_DIR_REV_SLOWEST )
    {
        a_intakeLeft = a_intakeSpeedReverseSlowest;
        a_intakeRight = -1.0 * a_intakeSpeedReverseSlowest;
    }
    else if( a_intakeDir == INTAKE_DIR_NEU )
    {
        a_intakeLeft = 0.0;
        a_intakeRight = 0.0;
    }
    else if( a_intakeDir == INTAKE_DIR_REV_BALLS_TO_WALL )
    {
        a_intakeLeft = a_intakeSpeedReverseBallsToWall;
        a_intakeRight = -1.0 * a_intakeSpeedReverseBallsToWall;
    }

    a_solenoidIn = false;
    a_solenoidOut = false;

    if( a_clawState == CLAW_STATE_CLOSED )
    {
        a_solenoidIn = true;
    }
    else if( a_clawState == CLAW_STATE_OPEN )
    {
        a_solenoidOut = true;
    }
    else if( a_clawState == CLAW_STATE_RAGDOLL )
    {
        //a_solenoidOut = true;
    }

    double sign = a_reverseIntakeDirection ? -1.0 : 1.0;
    a_intakeLeft *= sign;
    a_intakeRight *= sign;

    a_clawStateProbe.update( ( int )a_clawState );
    a_intakeDirProbe.update( ( int )a_intakeDir );
}

void Claw::selfTest( void )
{
}

void Claw::setClawPositionOpen( void )
{
    a_clawState = CLAW_STATE_OPEN;
}

void Claw::setClawPositionClosed( void )
{
    a_clawState = CLAW_STATE_CLOSED;
}

void Claw::setClawPositionRagdoll( void )
{
    a_clawState = CLAW_STATE_RAGDOLL;
}

void Claw::setIntakeDirectionForward( void )
{
    a_oscillationTimer.stop();
    a_oscillationTimer.reset();
    a_oscillationTimer.start();

    a_intakeDir = INTAKE_DIR_FWD;
}

void Claw::setIntakeDirectionReverseSlower( void )
{
    a_intakeDir = INTAKE_DIR_REV_SLOWER;
}

void Claw::setIntakeDirectionReverseSlowest( void )
{
    a_intakeDir = INTAKE_DIR_REV_SLOWEST;
}

void Claw::setIntakeDirectionReverse( void )
{
    a_intakeDir = INTAKE_DIR_REV;
}

void Claw::setIntakeDirectionNeutral( void )
{
    a_intakeDir = INTAKE_DIR_NEU;
}

void Claw::setIntakeDirectionReverseBallsToWall( void )
{
    a_intakeDir = INTAKE_DIR_REV_BALLS_TO_WALL;
}

int Claw::inputIntakeButton( bool value )
{
    a_intakeForwardButton = value;

    if( value )
    {
        setIntakeDirectionForward();
    }
    else
    {
        if( a_intakeReverseButton )
        {
            setIntakeDirectionReverse();
        }
        else if( a_spitMomentary )
        {
            setIntakeDirectionReverseSlowest();
        }
        else
        {
            setIntakeDirectionNeutral();
        }
    }

    return 0;
}

int Claw::inputOuttakeButton( bool value )
{
    a_intakeReverseButton = value;

    if( value )
    {
        setIntakeDirectionReverse();
    }
    else
    {
        if( a_intakeForwardButton )
        {
            setIntakeDirectionForward();
        }
        else if( a_spitMomentary )
        {
            setIntakeDirectionReverseSlowest();
        }
        else
        {
            setIntakeDirectionNeutral();
        }
    }

    this->updateClawState();

    return 0;
}

int Claw::inputClawOpen( bool value )
{
    if( value )
    {
        a_intakeOpenLock = true;
    }

    this->updateClawState();

    return 0;
}

int Claw::inputClawUnOpen( bool value )
{
    if( value )
    {
        a_intakeOpenLock = false;
    }

    this->updateClawState();

    return 0;
}

int Claw::inputClawDrop( bool value )
{
    a_dropMomentary = value;

    this->updateClawState();

    return 0;
}

int Claw::inputClawSpit( bool value )
{
    a_spitMomentary = value;

    if( value )
    {
        setIntakeDirectionReverseSlowest();
    }
    else
    {
        if( a_intakeReverseButton )
        {
            setIntakeDirectionReverse();
        }
        else if( a_intakeForwardButton )
        {
            setIntakeDirectionForward();
        }
        else
        {
            setIntakeDirectionNeutral();
        }
    }

    this->updateClawState();

    return 0;
}

int Claw::inputClawCloseLock( bool value )
{
    if( value )
    {
        a_closeLock = true;
    }

    this->updateClawState();

    return 0;
}

int Claw::inputIntakePosition( bool value )
{
    if( value )
    {
        a_closeLock = false;
    }
    a_intakePositionButton = value;

    this->updateClawState();

    return 0;
}

int Claw::inputLeftSwitch( bool value )
{
    a_leftSwitch = value;

    this->updateClawState();

    return 0;
}

int Claw::inputRightSwitch( bool value )
{
    a_rightSwitch = value;

    this->updateClawState();

    return 0;
}

bool Claw::clawHasCube( void )
{
    return ( a_rightSwitch || a_leftSwitch );
}

clawStateT Claw::getState( void )
{
    return a_clawState;
}

void Claw::updateClawState( void )
{
    a_outtaking = ( a_spitMomentary || a_intakeReverseButton );

    switch( a_clawState )
    {
        case CLAW_STATE_NONE:
        case CLAW_STATE_NUM:
        default:
            a_clawState = CLAW_STATE_RAGDOLL;
            // intentional fall through
        case CLAW_STATE_RAGDOLL:
            if( a_dropMomentary || a_intakeOpenLock )
            {
                a_clawState = CLAW_STATE_OPEN;
                a_closeLock = false;
            }
            else if( ( a_leftSwitch && a_rightSwitch ) || a_closeLock )
            {
                a_clawState = CLAW_STATE_CLOSED;
            }
            break;

        case CLAW_STATE_CLOSED:
            if( a_dropMomentary || a_intakeOpenLock )
            {
                a_clawState = CLAW_STATE_OPEN;
                a_closeLock = false;
            }
            else if( a_outtakingPrev && !a_outtaking )
            {
                a_clawState = CLAW_STATE_RAGDOLL;
                a_closeLock = false;
            }
            else if( a_intakePositionButton && !( a_leftSwitch || a_rightSwitch ) )
            {
                a_clawState = CLAW_STATE_RAGDOLL;
                a_closeLock = false;
            }
            break;

        case CLAW_STATE_OPEN:
            if( !a_dropMomentary && !a_intakeOpenLock )
            {
                a_clawState = CLAW_STATE_RAGDOLL;

                if( a_leftSwitch && a_rightSwitch )
                {
                    a_clawState = CLAW_STATE_CLOSED;
                }
            }
            else if( a_closeLock )
            {
                a_clawState = CLAW_STATE_CLOSED;
            }
            break;
    }

    a_outtakingPrev = a_outtaking;
}
