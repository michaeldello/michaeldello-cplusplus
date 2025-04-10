// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.

#ifndef INCLUDE_MAINHELP_H_
#define INCLUDE_MAINHELP_H_
//-----------------------------------------------------------------------------
//
// This helper module decouples the User interface from the main() loop.
//
//-----------------------------------------------------------------------------

namespace mainhelp
{

    //-------------------------------------------------------------------------
    // Constants
    //-------------------------------------------------------------------------

    enum eSelections
    {
        SELECTION_QUIT = 0,
        SELECTION_ADAPTER,
        SELECTION_FACADE,
        SELECTION_PROXY
    };

    //-------------------------------------------------------------------------
    // Function Declarations
    //-------------------------------------------------------------------------
    void printMenu();

    //-------------------------------------------------------------------------
    // Functions mapping to valid choices
    //-------------------------------------------------------------------------
    void adapter();

    //-------------------------------------------------------------------------
    void facade();

    //-------------------------------------------------------------------------
    void proxy();

    //-------------------------------------------------------------------------
    void quit();

}

#endif // INCLUDE_MAINHELP_H_