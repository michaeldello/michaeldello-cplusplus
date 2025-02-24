//-----------------------------------------------------------------------------
// Main Unit Tests
//-----------------------------------------------------------------------------
#define CATCH_CONFIG_MAIN
// Include EVERYTHING from Catch2 (version 3)
// This avoids the need to create a main() for unit testing
#include <catch2/catch_all.hpp>
// Alternatively, only include what is needed
// #include <catch2/catch_test_macros.hpp> // Basic test macros
// #include <catch2/catch_approx.hpp>  // Approximate floating-point comparisons
// #include <catch2/catch_reporter_console.hpp> // Console reporting
// #define CATCH_CONFIG_MAIN // Generates Catch2's main function
// #include <catch2/catch_test_macros.hpp>

// Create a custom main() instead:
/* int main(int argc, char* argv[]) {
    Catch::Session session; // Create Catch2 session
    return session.run(argc, argv);
} */
#include <iostream>
#include <sstream>
#include "mainhelp.h"

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Generic helper function to capture stdout output for any callable, using 
// forwarding on input parameters to maintain value categories (lvalue/rvalue)
template <typename Func, typename... Args>
std::string captureStdout(Func&& func, Args&&... args)
{
    std::ostringstream buffer;
    
    // Redirect std::cout
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    // Call the function with arguments
    std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);

    // Restore std::cout
    std::cout.rdbuf(old);
    return buffer.str();
}

//-----------------------------------------------------------------------------
// Mock Unit Test
//-----------------------------------------------------------------------------
// Mock a function to test for main
int add(int a, int b)
{
    return a + b;
}

TEST_CASE("Basic Addition", "[math]")
{
    REQUIRE(add(2, 3) == 5);
    REQUIRE(add(-1, 1) == 0);
}

//-----------------------------------------------------------------------------
// Print Menu Unit Test
//-----------------------------------------------------------------------------

TEST_CASE("Test printMenu()", "[printMenu]")
{
    std::string output = captureStdout(mainhelp::printMenu);
    REQUIRE(
        output.find(
            "Select A C++ Example to Run (0 to Quit)") != std::string::npos);
    REQUIRE(output.find("Enter your selection:") != std::string::npos);
}

//-----------------------------------------------------------------------------
// Echo Selection Unit Test
//-----------------------------------------------------------------------------

// Static for internal linkage
static const char* TEST_FUNCTION_NAME = "testfunctionname";

TEST_CASE("Test echoSelection()", "[echoSelection]")
{
    std::string output = 
        captureStdout(
            mainhelp::echoSelection, mainhelp::SELECTION_QUIT, TEST_FUNCTION_NAME);
    REQUIRE(output.find("0") != std::string::npos);
    REQUIRE(output.find(TEST_FUNCTION_NAME) != std::string::npos);
}