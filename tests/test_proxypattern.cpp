// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.

//-----------------------------------------------------------------------------
// Proxy Pattern Unit Tests
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

#include "proxypattern.h"

//=============================================================================
// DUT Unit Tests
//=============================================================================

//-----------------------------------------------------------------------------
// execute() Unit Tests
//-----------------------------------------------------------------------------

TEST_CASE("Test execute() pass", "[dut-execute-pass]")
{
    auto expectedValue = DUTProxy::eTestResults::PASS;
    DUTProxy::DUT dut{{"EX-DUT-1"}};
    REQUIRE(dut.execute(DUTProxy::eTests::TEST_PASSINGFEATURE) == expectedValue);
}