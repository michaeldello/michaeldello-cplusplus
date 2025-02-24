//-----------------------------------------------------------------------------
//
// Description:
//    Helper types and functions and constants for main().
//
// Author:
//    Michael Dello
//
//-----------------------------------------------------------------------------
#include <iostream>
#include "mainhelp.h"
#include "adapterpattern.h"

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
void mainhelp::printMenu()
{
    std::cout << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "Select A C++ Example to Run (0 to Quit)" << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << std::endl;
    std::cout << mainhelp::SELECTION_QUIT << ". Quit" << std::endl;
    std::cout << mainhelp::SELECTION_ADAPTER 
              << ". Adapter Design Pattern" 
              << std::endl;
    std::cout << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter your selection:  ";
}

//-----------------------------------------------------------------------------
void mainhelp::echoSelection(
    mainhelp::eSelections selection, 
    const char* function_name)
{
    // Give the user some time to observe the output before continuing
    std::cout << "You selected Option " 
              << selection 
              << ": " 
              << function_name 
              << std::endl;
}

// Functions to map to valid choices

//-----------------------------------------------------------------------------
void mainhelp::adapter()
{ 
    echoSelection(mainhelp::SELECTION_ADAPTER, __func__);
    // Call the example's module class method(s)
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "Executing Adapter Design Pattern Example ..." << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "Existing Robot Arm:" <<std::endl;
    RobotAdapter::RobotArmInterface* pRobotArm = 
        new RobotAdapter::ExistingRobotArm();
    auto zeroed = pRobotArm->zero();
    if (zeroed)
        std::cout << "Position zeroed" << std::endl;
    else
        std::cout << "!!!Unable to zero position!!!" << std::endl;
    auto moved = 
        pRobotArm->up(10) && pRobotArm->down(10) && pRobotArm->back(10) && 
        pRobotArm->forth(10) && pRobotArm->right(10) && pRobotArm->left(10);
    if (moved)
        std::cout << "Able to move Robot Arm" << std::endl;
    else
        std::cout << "!!!Unable to move Robot Arm!!!" << std::endl;
    delete(pRobotArm);
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "New Robot Arm:" <<std::endl;
    pRobotArm = new RobotAdapter::NewRobotArmAdapter();
    zeroed = pRobotArm->zero();
    if (zeroed)
        std::cout << "Position zeroed" << std::endl;
    else
        std::cout << "!!!Unable to zero position!!!" << std::endl;
    moved = 
        pRobotArm->up(10) && pRobotArm->down(10) && pRobotArm->back(10) && 
        pRobotArm->forth(10) && pRobotArm->right(10) && pRobotArm->left(10);
    if (moved)
        std::cout << "Able to move Robot Arm" << std::endl;
    else
        std::cout << "!!!Unable to move Robot Arm!!!" << std::endl;
    delete(pRobotArm);
    std::cout << "----------------------------------------------" << std::endl;
}

//-----------------------------------------------------------------------------
void mainhelp::quit() 
{ 
    echoSelection(mainhelp::SELECTION_QUIT, __func__);
}
