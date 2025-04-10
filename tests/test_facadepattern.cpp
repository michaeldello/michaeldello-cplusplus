// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.

//-----------------------------------------------------------------------------
// Facade Pattern Unit Tests
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
#include <memory>
#include "facadepattern.h"

//-----------------------------------------------------------------------------
// Unit Test Stub Implementations
//-----------------------------------------------------------------------------

#include <cstdint>
#include "facadepattern_stubs.h"

namespace SignalDataFacade
{
    //-------------------------------------------------------------------------
    // ADCDrv Stub Implementation
    //-------------------------------------------------------------------------

    // Define static variables
    uint16_t ADCDrvStub::testValue = 0;
    bool ADCDrvStub::bFail = false;

    //-------------------------------------------------------------------------
    ADCDrvStub::ADCDrvStub(): bStarted{false}
    {
        // No Body
    }

    //-------------------------------------------------------------------------
    void ADCDrvStub::start()
    {
        bStarted = bStarted || !bFail;
    }

    //-------------------------------------------------------------------------
    void ADCDrvStub::stop()
    {
        bStarted = bStarted && bFail;
    }

    //-------------------------------------------------------------------------
    std::optional<uint16_t> ADCDrvStub::read()
    {
        // Read only if started
        if (bStarted && !bFail)
            return testValue;
        // Default, return nothing
        return {};
    }

    //-------------------------------------------------------------------------
    // GPIODrv Stub Implementation
    //-------------------------------------------------------------------------

    // Define static variables
    uint16_t GPIODrvStub::testValue = 0;
    bool GPIODrvStub::bFail = false;

    //-------------------------------------------------------------------------
    std::optional<uint16_t> GPIODrvStub::read()
    {
        // Read only if started
        if (bFail)
            return {};
        else
            return testValue;
    }

} // SignalDataFacade

//-----------------------------------------------------------------------------
// Test Variables
//-----------------------------------------------------------------------------

// Stubs used instead of real hardware interaction

// ADC Stub
static std::unique_ptr<SignalDataFacade::ADCDrvStub> upAdcDrvStub =
    std::make_unique<SignalDataFacade::ADCDrvStub>();

// GPIO Stub
static std::unique_ptr<SignalDataFacade::GPIODrvStub> upGpioDrvStub =
    std::make_unique<SignalDataFacade::GPIODrvStub>();

// These classes are testable in this context
// Transfer ownership of unique pointers using move semantics

// Testable ADC HAL
static std::unique_ptr<SignalDataFacade::A2DConverterHAL> upA2dConverterHAL =
    std::make_unique<SignalDataFacade::A2DConverterHAL>(
        std::move(upAdcDrvStub));

// Testable GPIO HAL
static std::unique_ptr<SignalDataFacade::GPIOHAL> upGpioHAL =
    std::make_unique<SignalDataFacade::GPIOHAL>(
        std::move(upGpioDrvStub));

// Testable Signal Data Facade
// Delay access to this object to ensure proper initialization and usage with
// respect to its parts
SignalDataFacade::SignalData& getSignalDataFacade() {
    static SignalDataFacade::SignalData& instance =
        SignalDataFacade::SignalData::getInstance(
            std::move(upA2dConverterHAL),
            std::move(upGpioHAL));
    return instance;
}

//=============================================================================
// A2D Converter HAL Unit Tests
//=============================================================================

//-----------------------------------------------------------------------------
// read() Unit Tests
//-----------------------------------------------------------------------------

TEST_CASE("Test A2D HAL read() pass", "[adc-hal-read-pass]")
{
    SignalDataFacade::ADCDrvStub::testValue = 1;
    REQUIRE(upA2dConverterHAL->read() == 1);
    // Reset object under test
    SignalDataFacade::ADCDrvStub::testValue = 0;
}

TEST_CASE("Test A2D HAL read() fail", "[adc-hal-read-fail]")
{
    SignalDataFacade::ADCDrvStub::testValue = 3;
    SignalDataFacade::ADCDrvStub::bFail = true;
    // Fail will result in no signal
    REQUIRE(upA2dConverterHAL->read() == 0);
    // Reset object under test
    SignalDataFacade::ADCDrvStub::bFail = false;
    SignalDataFacade::ADCDrvStub::testValue = 0;
}

//=============================================================================
// GPIO HAL Unit Tests
//=============================================================================

//-----------------------------------------------------------------------------
// read() Unit Tests
//-----------------------------------------------------------------------------

TEST_CASE("Test GPIO HAL read() pass", "[gpio-hal-read-pass]")
{
    SignalDataFacade::GPIODrvStub::testValue = 7;
    REQUIRE(upGpioHAL->read() == 7);
    // Reset object under test
    SignalDataFacade::GPIODrvStub::testValue = 0;
}

TEST_CASE("Test GPIO HAL read() fail", "[gpio-hal-read-fail]")
{
    SignalDataFacade::GPIODrvStub::testValue = 15;
    SignalDataFacade::GPIODrvStub::bFail = true;
    // Fail will result in no signal
    REQUIRE(upGpioHAL->read() == 0);
    // Reset object under test
    SignalDataFacade::GPIODrvStub::bFail = false;
    SignalDataFacade::GPIODrvStub::testValue = 0;
}

//=============================================================================
// Signal Data Facade Unit Tests
//=============================================================================

//-----------------------------------------------------------------------------
// acquire() Unit Tests
//-----------------------------------------------------------------------------

TEST_CASE(
    "Test Signal Data Facade acquire() pass",
    "[signaldata-facade-acquire-pass]")
{
    SignalDataFacade::ADCDrvStub::testValue = 31;
    SignalDataFacade::GPIODrvStub::testValue = 63;
    auto acquiredData = getSignalDataFacade().acquire();
    REQUIRE(acquiredData.analog == 31);
    REQUIRE(acquiredData.digital == 63);
    // Reset objects under test
    SignalDataFacade::ADCDrvStub::testValue = 0;
    SignalDataFacade::GPIODrvStub::testValue = 0;
}

TEST_CASE(
    "Test Signal Data Facade acquire() ADC fail",
    "[signaldata-facade-acquire-adc-fail]")
{
    SignalDataFacade::ADCDrvStub::testValue = 127;
    SignalDataFacade::ADCDrvStub::bFail = true;
    SignalDataFacade::GPIODrvStub::testValue = 255;
    auto acquiredData = getSignalDataFacade().acquire();
    REQUIRE(acquiredData.analog == 0);
    REQUIRE(acquiredData.digital == 255);
    // Reset objects under test
    SignalDataFacade::ADCDrvStub::bFail = false;
    SignalDataFacade::ADCDrvStub::testValue = 0;
    SignalDataFacade::GPIODrvStub::testValue = 0;
}

TEST_CASE(
    "Test Signal Data Facade acquire() GPIO fail",
    "[signaldata-facade-acquire-gpio-fail]")
{
    SignalDataFacade::ADCDrvStub::testValue = 511;
    SignalDataFacade::GPIODrvStub::testValue = 1023;
    SignalDataFacade::GPIODrvStub::bFail = true;
    auto acquiredData = getSignalDataFacade().acquire();
    REQUIRE(acquiredData.analog == 511);
    REQUIRE(acquiredData.digital == 0);
    // Reset objects under test
    SignalDataFacade::ADCDrvStub::testValue = 0;
    SignalDataFacade::GPIODrvStub::testValue = 0;
    SignalDataFacade::GPIODrvStub::bFail = false;
}

TEST_CASE(
    "Test Signal Data Facade acquire() both fail",
    "[signaldata-facade-acquire-both-fail]")
{
    SignalDataFacade::ADCDrvStub::testValue = 2047;
    SignalDataFacade::ADCDrvStub::bFail = true;
    SignalDataFacade::GPIODrvStub::testValue = 4095;
    SignalDataFacade::GPIODrvStub::bFail = true;
    auto acquiredData = getSignalDataFacade().acquire();
    REQUIRE(acquiredData.analog == 0);
    REQUIRE(acquiredData.digital == 0);
    // Reset objects under test
    SignalDataFacade::ADCDrvStub::testValue = 0;
    SignalDataFacade::ADCDrvStub::bFail = false;
    // Reset object under test
    SignalDataFacade::GPIODrvStub::testValue = 0;
    SignalDataFacade::GPIODrvStub::bFail = false;
}