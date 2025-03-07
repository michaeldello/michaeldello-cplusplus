// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.

//-----------------------------------------------------------------------------
//
// Description:
//    Run one of any C++ Code Examples I am maintaining for future reference.
//
//    Each example demonstrates a worthwhile concept beyond basic C++.
//
// Author:
//    Michael Dello
//
//-----------------------------------------------------------------------------

#include <chrono>
#include <functional>
#include <iostream>
#include <limits>
#include <thread>
#include <unordered_map>
#include "mainhelp.h"

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

constexpr long int MAX_CHARACTERS_ALLOWED =
    std::numeric_limits<std::streamsize>::max();

constexpr unsigned int SECONDS_TO_PAUSE = 3;

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
void allowObservation()
{
    // Give the user some time to observe the output before continuing
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(SECONDS_TO_PAUSE));
}

//-----------------------------------------------------------------------------
int main()
{
    // Map valid choices to functions
    const std::unordered_map<int, std::function<void()>> menuActions
    {
        {mainhelp::SELECTION_QUIT, mainhelp::quit},
        {mainhelp::SELECTION_ADAPTER, mainhelp::adapter},
        {mainhelp::SELECTION_FACADE, mainhelp::facade}
    };

    auto choice = 0;

    while (true)
    {
        mainhelp::printMenu();
        std::cin >> choice;

        // Handle invalid input (non-numeric)
        if (std::cin.fail())
        {
            // Clear cin to avoid infinite loop
            std::cin.clear();
            std::cin.ignore(MAX_CHARACTERS_ALLOWED, '\n');
            std::cout << "Invalid input! Please enter a number." << std::endl;
            allowObservation();
            continue;
        }

        // Validate and process choice
        if (menuActions.count(choice))
        {
            menuActions.at(choice)();
        }
        else
        {
            std::cout << "Invalid choice: " << choice << std::endl;
        }
        allowObservation();
        if (choice == 0) break;
    }

    return 0;
}