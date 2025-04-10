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
    REQUIRE(
        dut.execute(DUTProxy::eTests::TEST_PASSINGFEATURE) == expectedValue);
}

TEST_CASE("Test execute() fail", "[dut-execute-fail]")
{
    auto expectedValue = DUTProxy::eTestResults::FAIL;
    DUTProxy::DUT dut{{"EX-DUT-1"}};
    REQUIRE(
        dut.execute(DUTProxy::eTests::TEST_FAILINGFEATURE) == expectedValue);
}

TEST_CASE("Test execute() incomplete", "[dut-execute-incomplete]")
{
    auto expectedValue = DUTProxy::eTestResults::AMBIGUOUS;
    DUTProxy::DUT dut{{"EX-DUT-1"}};
    REQUIRE(
        dut.execute(DUTProxy::eTests::TEST_INCOMPLETEFEATURE) == expectedValue);
}

TEST_CASE("Test overall results", "[dut-overall-results]")
{
    // Test Default/Initialization
    auto expectedValue = DUTProxy::eTestResults::NONE;
    DUTProxy::DUT dut{{"EX-DUT-1"}};
    REQUIRE(dut.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);

    // Test Ambiguous
    expectedValue = DUTProxy::eTestResults::AMBIGUOUS;
    dut.execute(DUTProxy::eTests::TEST_INCOMPLETEFEATURE);
    REQUIRE(dut.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    expectedValue = DUTProxy::eTestResults::NONE;
    REQUIRE(dut.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);

    // Test Passed
    expectedValue = DUTProxy::eTestResults::PASSED;
    dut.execute(DUTProxy::eTests::TEST_PASSINGFEATURE);
    REQUIRE(dut.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    expectedValue = DUTProxy::eTestResults::NONE;
    REQUIRE(dut.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    //
    expectedValue = DUTProxy::eTestResults::PASSED;
    dut.execute(DUTProxy::eTests::TEST_PASSINGFEATURE);
    dut.execute(DUTProxy::eTests::TEST_INCOMPLETEFEATURE);
    REQUIRE(dut.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    expectedValue = DUTProxy::eTestResults::NONE;
    REQUIRE(dut.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);

    // Test Failed
    expectedValue = DUTProxy::eTestResults::FAILED;
    dut.execute(DUTProxy::eTests::TEST_FAILINGFEATURE);
    REQUIRE(dut.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    expectedValue = DUTProxy::eTestResults::NONE;
    REQUIRE(dut.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    //
    expectedValue = DUTProxy::eTestResults::FAILED;
    dut.execute(DUTProxy::eTests::TEST_FAILINGFEATURE);
    dut.execute(DUTProxy::eTests::TEST_PASSINGFEATURE);
    REQUIRE(dut.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    expectedValue = DUTProxy::eTestResults::NONE;
    REQUIRE(dut.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    //
    expectedValue = DUTProxy::eTestResults::FAILED;
    dut.execute(DUTProxy::eTests::TEST_FAILINGFEATURE);
    dut.execute(DUTProxy::eTests::TEST_INCOMPLETEFEATURE);
    REQUIRE(dut.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    expectedValue = DUTProxy::eTestResults::NONE;
    REQUIRE(dut.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
}

//=============================================================================
// Proxy Unit Tests
//=============================================================================

//-----------------------------------------------------------------------------
// execute() Unit Tests
//-----------------------------------------------------------------------------

TEST_CASE("Test proxy execute() pass", "[proxy-execute-pass]")
{
    // DUT params
    std::string sDutName{"EX-DUT-1"};
    std::string sDutIpAddr{"127.0.0.1"};

    DUTProxy::DUT localDut{{sDutName}};
    DUTProxy::DUTProxyServer proxyServer{localDut};

    auto expectedValue = DUTProxy::eTestResults::PASS;

    DUTProxy::DUTProxyClient dutProxy{{sDutName, sDutIpAddr}};
    REQUIRE(
        dutProxy.execute(
            DUTProxy::eTests::TEST_PASSINGFEATURE) == expectedValue);
}

TEST_CASE("Test proxy execute() fail", "[proxy-execute-fail]")
{
    // DUT params
    std::string sDutName{"EX-DUT-1"};
    std::string sDutIpAddr{"127.0.0.1"};

    DUTProxy::DUT localDut{{sDutName}};
    DUTProxy::DUTProxyServer proxyServer{localDut};

    auto expectedValue = DUTProxy::eTestResults::FAIL;

    DUTProxy::DUTProxyClient dutProxy{{sDutName, sDutIpAddr}};
    REQUIRE(
        dutProxy.execute(
            DUTProxy::eTests::TEST_FAILINGFEATURE) == expectedValue);
}

TEST_CASE("Test proxy execute() incomplete", "[proxy-execute-incomplete]")
{
    // DUT params
    std::string sDutName{"EX-DUT-1"};
    std::string sDutIpAddr{"127.0.0.1"};

    DUTProxy::DUT localDut{{sDutName}};
    DUTProxy::DUTProxyServer proxyServer{localDut};

    auto expectedValue = DUTProxy::eTestResults::AMBIGUOUS;

    DUTProxy::DUTProxyClient dutProxy{{sDutName, sDutIpAddr}};
    REQUIRE(
        dutProxy.execute(
            DUTProxy::eTests::TEST_INCOMPLETEFEATURE) == expectedValue);
}

TEST_CASE("Test proxy overall results", "[proxy-overall-results]")
{
    // DUT params
    std::string sDutName{"EX-DUT-1"};
    std::string sDutIpAddr{"127.0.0.1"};

    DUTProxy::DUT localDut{{sDutName}};
    DUTProxy::DUTProxyServer proxyServer{localDut};
    DUTProxy::DUTProxyClient dutProxy{{sDutName, sDutIpAddr}};

    // Test Default/Initialization
    auto expectedValue = DUTProxy::eTestResults::NONE;
    
    REQUIRE(dutProxy.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);

    // Test Ambiguous
    expectedValue = DUTProxy::eTestResults::AMBIGUOUS;
    dutProxy.execute(DUTProxy::eTests::TEST_INCOMPLETEFEATURE);
    REQUIRE(dutProxy.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    expectedValue = DUTProxy::eTestResults::NONE;
    REQUIRE(dutProxy.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);

    // Test Passed
    expectedValue = DUTProxy::eTestResults::PASSED;
    dutProxy.execute(DUTProxy::eTests::TEST_PASSINGFEATURE);
    REQUIRE(dutProxy.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    expectedValue = DUTProxy::eTestResults::NONE;
    REQUIRE(dutProxy.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    //
    expectedValue = DUTProxy::eTestResults::PASSED;
    dutProxy.execute(DUTProxy::eTests::TEST_PASSINGFEATURE);
    dutProxy.execute(DUTProxy::eTests::TEST_INCOMPLETEFEATURE);
    REQUIRE(dutProxy.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    expectedValue = DUTProxy::eTestResults::NONE;
    REQUIRE(dutProxy.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);

    // Test Failed
    expectedValue = DUTProxy::eTestResults::FAILED;
    dutProxy.execute(DUTProxy::eTests::TEST_FAILINGFEATURE);
    REQUIRE(dutProxy.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    expectedValue = DUTProxy::eTestResults::NONE;
    REQUIRE(dutProxy.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    //
    expectedValue = DUTProxy::eTestResults::FAILED;
    dutProxy.execute(DUTProxy::eTests::TEST_FAILINGFEATURE);
    dutProxy.execute(DUTProxy::eTests::TEST_PASSINGFEATURE);
    REQUIRE(dutProxy.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    expectedValue = DUTProxy::eTestResults::NONE;
    REQUIRE(dutProxy.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    //
    expectedValue = DUTProxy::eTestResults::FAILED;
    dutProxy.execute(DUTProxy::eTests::TEST_FAILINGFEATURE);
    dutProxy.execute(DUTProxy::eTests::TEST_INCOMPLETEFEATURE);
    REQUIRE(dutProxy.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
    expectedValue = DUTProxy::eTestResults::NONE;
    REQUIRE(dutProxy.execute(DUTProxy::eTests::STOP_TESTING) == expectedValue);
}
