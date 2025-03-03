//-----------------------------------------------------------------------------
// Adapter Pattern Unit Tests
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
#include "adapterpattern.h"

//-----------------------------------------------------------------------------
// Test Variables
//-----------------------------------------------------------------------------

static RobotAdapter::NewRobotArmAdapter newRobotArmAdapter;

//-----------------------------------------------------------------------------
// Zero Unit Test
//-----------------------------------------------------------------------------

TEST_CASE("Test zero()", "[zero]")
{
    REQUIRE(newRobotArmAdapter.zero());
}

//-----------------------------------------------------------------------------
// Z-Axis Unit Tests
//-----------------------------------------------------------------------------

TEST_CASE("Test up() In Bounds", "[up-valid]")
{
    REQUIRE(newRobotArmAdapter.up(100));
    newRobotArmAdapter.zero();
}

TEST_CASE("Test up() Out of Bounds", "[up-invalid]")
{
    REQUIRE(!newRobotArmAdapter.up(10000));
    newRobotArmAdapter.zero();
}

TEST_CASE("Test down() In Bounds", "[down-valid]")
{
    newRobotArmAdapter.up(100);
    REQUIRE(newRobotArmAdapter.down(100));
    newRobotArmAdapter.zero();
}

TEST_CASE("Test down() Out of Bounds", "[down-invalid]")
{
    REQUIRE(!newRobotArmAdapter.down(10000));
    newRobotArmAdapter.zero();
}

//-----------------------------------------------------------------------------
// Y-Axis Unit Tests
//-----------------------------------------------------------------------------

TEST_CASE("Test back() In Bounds", "[back-valid]")
{
    REQUIRE(newRobotArmAdapter.back(100));
    newRobotArmAdapter.zero();
}

TEST_CASE("Test back() Out of Bounds", "[back-invalid]")
{
    REQUIRE(!newRobotArmAdapter.back(10000));
    newRobotArmAdapter.zero();
}

TEST_CASE("Test forth() In Bounds", "[forth-valid]")
{
    newRobotArmAdapter.back(100);
    REQUIRE(newRobotArmAdapter.forth(100));
    newRobotArmAdapter.zero();
}

TEST_CASE("Test forth() Out of Bounds", "[forth-invalid]")
{
    REQUIRE(!newRobotArmAdapter.forth(10000));
    newRobotArmAdapter.zero();
}

//-----------------------------------------------------------------------------
// X-Axis Unit Tests
//-----------------------------------------------------------------------------

TEST_CASE("Test right() In Bounds", "[right-valid]")
{
    REQUIRE(newRobotArmAdapter.right(100));
    newRobotArmAdapter.zero();
}

TEST_CASE("Test right() Out of Bounds", "[right-invalid]")
{
    REQUIRE(!newRobotArmAdapter.right(10000));
    newRobotArmAdapter.zero();
}

TEST_CASE("Test left() In Bounds", "[left-valid]")
{
    newRobotArmAdapter.right(100);
    REQUIRE(newRobotArmAdapter.left(100));
    newRobotArmAdapter.zero();
}

TEST_CASE("Test left() Out of Bounds", "[left-invalid]")
{
    REQUIRE(!newRobotArmAdapter.left(10000));
    newRobotArmAdapter.zero();
}