#ifndef __MODBUTTONINPUT_H__
#define __MODBUTTONINPUT_H__

#include "io/inputs/InputStubs.h"
#include "io/routing/InputListener.h"

#include <string>


class ModButtonInput : public DigitalInput, public InputListener
{
    public:
        ModButtonInput( std::string name, std::string mod_name, 
                        std::string target_name, bool modState );
        virtual ~ModButtonInput( void );

        virtual void inputUpdate( Input* p_source, bool value ) override;

    protected:
        virtual bool readRawValue( void ) override;

        virtual int instantiate( void ) override;

    private:
        std::string a_modName;
        std::string a_targetName;

        Input* ap_modButton;
        Input* ap_targetButton;

        bool a_modState;
        bool a_modButtonPressed;
        bool a_targetButtonPressed;
};

#endif
