//------------------------------------------------------------------------------
//
// Description:
//    Function definitions and constants decoupled from the main() loop.
//
// Author:
//    Michael Dello
//
//------------------------------------------------------------------------------
#include <iostream>
#include <memory>
#include <ranges>
#include "mainhelp.h"
#include "adapterpattern.h"
#include "facadepattern.h"

namespace // anonymous
{
    //--------------------------------------------------------------------------
    // Local Functions
    //--------------------------------------------------------------------------
    // The function_name input parameter is typed per the __func__ C++ built-in
    void echoSelection(
        mainhelp::eSelections selection,
        const char* const function_name)
    {
        // Give the user some time to observe the output before continuing
        std::cout << "You selected Option " 
                << selection 
                << ": " 
                << function_name 
                << std::endl;
    }

} // namespace anonymous

namespace mainhelp
{
    //--------------------------------------------------------------------------
    // Function Definitions
    //--------------------------------------------------------------------------
    void printMenu()
    {
        std::cout << std::endl;
        std::cout << "=======================================" << std::endl;
        std::cout << "Select A C++ Example to Run (0 to Quit)" << std::endl;
        std::cout << "=======================================" << std::endl;
        std::cout << std::endl;
        std::cout << SELECTION_QUIT << ". Quit" << std::endl;
        std::cout << SELECTION_ADAPTER 
                  << ". Adapter Design Pattern" 
                  << std::endl;
        std::cout << SELECTION_FACADE 
                  << ". Facade Design Pattern" 
                  << std::endl;
         std::cout << std::endl;
        std::cout << "=======================================" << std::endl;
        std::cout << std::endl;
        std::cout << "Enter your selection:  ";
    }

    //--------------------------------------------------------------------------
    // Functions mapping to valid choices
    //--------------------------------------------------------------------------
    void adapter()
    { 
        echoSelection(SELECTION_ADAPTER, __func__);
        // Call the example's module class method(s)
        std::cout << "-------------------------------------------" << std::endl;
        std::cout << "Executing Adapter Design Pattern Example..." << std::endl;
        std::cout << "-------------------------------------------" << std::endl;
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
        std::cout << "-------------------------------------------" << std::endl;
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
        std::cout << "-------------------------------------------" << std::endl;
    }

    //--------------------------------------------------------------------------
    void facade()
    { 
        echoSelection(SELECTION_FACADE, __func__);
        // Call the example's module class method(s)
        std::cout << "-------------------------------------------" << std::endl;
        std::cout << "Executing Facade Design Pattern Example..." << std::endl;
        std::cout << "-------------------------------------------" << std::endl;
        std::cout << "Data Acqusition Subsytem Parts:" <<std::endl;
        auto upAdcDrv = std::make_unique<SignalDataFacade::ADCDrv>();
        auto upGpioDrv = std::make_unique<SignalDataFacade::GPIODrv>();
        auto upA2dConverterHAL = 
            std::make_unique<SignalDataFacade::A2DConverterHAL>(
                std::move(upAdcDrv));
        auto upGpioHAL = 
            std::make_unique<SignalDataFacade::GPIOHAL>(std::move(upGpioDrv));
        std::cout << "Data Acquisition Subsystem Facade:" <<std::endl;
        auto& facade = 
            SignalDataFacade::SignalData::getInstance(
                std::move(upA2dConverterHAL), 
                std::move(upGpioHAL));
        std::cout << "Exercise Facade Acquisition:" <<std::endl;
        SignalDataFacade::SignalData::sAggregateData acquiredData(0,0);
        for (int i : std::views::iota(0,5))
        {
            acquiredData = facade.acquire();
            std::cout << "New Data Acquired:" << std::endl;
            std::cout << "    Analog Value  : " 
                      << acquiredData.analog 
                      << std::endl;
            std::cout << "    Digital Value : " 
                      << acquiredData.digital
                      << std::endl;
        }
        std::cout << "-------------------------------------------" << std::endl;
    }

    //--------------------------------------------------------------------------
    void quit() 
    { 
        echoSelection(SELECTION_QUIT, __func__);
    }

} // namespace mainhelp