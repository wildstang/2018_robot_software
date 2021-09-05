#ifndef __AUTOPROGRAM_H__
#define __AUTOPROGRAM_H__

#include <vector>
#include <string>

class AutoSerialStepGroup;
class AutoStep;

class AutoProgram
{
    public:
        AutoProgram( std::string program_name );
        AutoProgram( std::string program_name, std::string code );
        virtual ~AutoProgram( void );

        void update( void );
        void initialize( void );
        void cleanup( void );
        bool isFinished( void );

        inline std::string getProgramName( void ) { return ac_programName; }

        //
        // What is the difference between getCode and
        // generateCode?
        //
        // Well, getCode gets the code as it was set in the case
        // of a programmatic auto program. This is desirable because
        // you may wish for it to spit back code with all the comments
        // and formatting that you had when you loaded it onto the
        // robot.
        //
        // Obviously, this does not apply to programs that are
        // hardcoded as per previous versions of the framework.
        // generateCode programmatically generates code for any program
        // be it programmatic or otherwise. This is also nice for
        // programmatic programs because for debugging you can
        // see that all the steps were created properly.
        //
        // In the case of a non-programmatic program, the getCode
        // function fails over to the generateCode function so that
        // something always comes back.
        //
        std::string getCode( void );
        virtual std::string generateCode( void );

        //
        // returns true if there are no mistakes
        // returns false if there are mistakes
        //
        bool checkSyntax( void );

    protected:
        //
        // Loads the steps from code;
        // you can overload this function in order to create
        // a program where the steps are hardcoded to be backward
        // compatible.
        //
        virtual void defineSteps( void ); 

        void addStep( AutoStep* p_step );
        
    private:
        const std::string ac_programName;
        const bool ac_hasCode;
        std::string a_code;

        AutoSerialStepGroup* ap_steps;
};

#endif

