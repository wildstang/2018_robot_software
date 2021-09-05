#ifndef __LOGICBUTTON_H__
#define __LOGICBUTTON_H__

#include "io/inputs/InputStubs.h"
#include "io/routing/InputListener.h"

#include <string>

typedef enum
{
    LOGIC_OPERATOR_NONE = 0,

    LOGIC_OPERATOR_AND,
    LOGIC_OPERATOR_OR,
    LOGIC_OPERATOR_XOR,

    LOGIC_OPERATOR_NUM
} logicOperatorT;

class LogicButton : public DigitalInput, public InputListener
{
    public:
        LogicButton( std::string name, std::string first_name, 
                     bool first_invert, std::string second_name,
                     bool second_invert, logicOperatorT logic );
        virtual ~LogicButton( void );

        virtual void inputUpdate( Input* p_source, bool value ) override;

    protected:
        virtual bool readRawValue( void ) override;

        virtual int instantiate( void ) override;

    private:
        std::string a_firstName;
        std::string a_secondName;

        Input* ap_firstButton;
        Input* ap_secondButton;

        bool a_firstButtonState;
        bool a_secondButtonState;

        bool a_firstInvert;
        bool a_secondInvert;

        logicOperatorT a_logic;
};

#endif

