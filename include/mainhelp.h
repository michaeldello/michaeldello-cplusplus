#ifndef MAINHELP_H
#define MAINHELP_H

namespace mainhelp {

    //-------------------------------------------------------------------------
    // Constants
    //-------------------------------------------------------------------------

    enum eSelections
    {
        SELECTION_QUIT = 0,
        SELECTION_ADAPTER
    };

    //-------------------------------------------------------------------------
    // Function Declarations
    //-------------------------------------------------------------------------
    void printMenu();

    //-------------------------------------------------------------------------
    void echoSelection(eSelections selection, const char* function_name);

    //-------------------------------------------------------------------------
    // Functions to map to valid choices
    //-------------------------------------------------------------------------
    //
    //-------------------------------------------------------------------------
    void adapter();
    //
    //-------------------------------------------------------------------------
    void quit();

}

#endif // MAINHELP_H