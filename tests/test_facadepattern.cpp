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

#include <memory>
#include "facadepattern.h"

//-----------------------------------------------------------------------------
//
// Unit Test Stub Implementations
//
//-----------------------------------------------------------------------------

#include "facadepattern_stubs.h"

namespace SignalDataFacade
{
    //-------------------------------------------------------------------------
    // ADCDrv Stub Implementation
    //-------------------------------------------------------------------------
    ADCDrvStub::ADCDrvStub(): bStarted(false), bFail(false), valueToReturn(0)
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
    std::optional<uint16_t> ADCDrvStub::read() const
    {
        // Read only if started
        if (bStarted && !bFail)
            return valueToReturn;
        // Default, return nothing
        return {};
    }

    //-------------------------------------------------------------------------
    void ADCDrvStub::setValue(uint16_t value)
    {
        valueToReturn = value;
    }

    //-------------------------------------------------------------------------
    void ADCDrvStub::setFail(bool bFailSet)
    {
        bFail = bFailSet;
    }

    //-------------------------------------------------------------------------
    // GPIODrv Stub Implementation
    //-------------------------------------------------------------------------
    GPIODrvStub::GPIODrvStub(): bFail(false), valueToReturn(0)
    {
        // No Body
    }

    //-------------------------------------------------------------------------
    std::optional<uint16_t> GPIODrvStub::read() const
    {
        // Read only if started
        if (bFail)
            return {};
        else
            return valueToReturn;
    }

    //-------------------------------------------------------------------------
    void GPIODrvStub::setValue(uint16_t value)
    {
        valueToReturn = value;
    }

    //-------------------------------------------------------------------------
    void GPIODrvStub::setFail(bool bFailSet)
    {
        bFail = bFailSet;
    }

} // SignalDataFacade

//-----------------------------------------------------------------------------
// Test Variables
//-----------------------------------------------------------------------------

// Stubs used instead of real hardware interaction

// ADC Stub
static auto upAdcDrvStub = std::make_unique<SignalDataFacade::ADCDrvStub>();

// GPIO Stub
static auto upGpioDrvStub = std::make_unique<SignalDataFacade::GPIODrvStub>();

// This is a bit of an uncomfortable way to do this for testing - to still be 
// able to maintain access to the Stub object, as it is implemented with a 
// smart pointer and will be moved to the HAL object
//
// This allows Stub behavior manipulation after it has been moved to the HAL
// object
//
// I am partly doing this just to see if it will work
//
// A better method would be to decouple this logic into a function that both the
// underlying objects and the main program can access
static auto pRawADCStub = upAdcDrvStub.get();
static auto pRawGPIOStub = upGpioDrvStub.get();

// These classes are testable in this context
// Transfer ownership of unique pointers using move semantics

// Testable ADC HAL
static auto upA2dConverterHAL = 
    std::make_unique<SignalDataFacade::A2DConverterHAL>(
        std::move(upAdcDrvStub));

// Testable GPIO HAL
static auto upGpioHAL = 
    std::make_unique<SignalDataFacade::GPIOHAL>(
        std::move(upGpioDrvStub));

// Testable Signal Data Facade
/* static auto& signalDataFacade = 
    SignalDataFacade::SignalData::getInstance(
        std::move(upA2dConverterHAL), std::move(upGpioHAL));
*/

// Delay initialization of this to ensure proper initialization and usage with 
// respect to its parts
SignalDataFacade::SignalData& getSignalDataFacade() {
    static auto& instance = SignalDataFacade::SignalData::getInstance(
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
    constexpr auto valueToCheck = 1;
    pRawADCStub->setValue(valueToCheck);
    REQUIRE(upA2dConverterHAL->read() == valueToCheck);
    // Reset object under test
    pRawADCStub->setValue(0);
}

TEST_CASE("Test A2D HAL read() fail", "[adc-hal-read-fail]")
{
    constexpr auto valueToCheck = 3;
    pRawADCStub->setValue(valueToCheck);
    pRawADCStub->setFail(true);
    // Fail will result in no signal
    REQUIRE(upA2dConverterHAL->read() == 0);
    // Reset object under test
    pRawADCStub->setFail(false);
    pRawADCStub->setValue(0);
}

//=============================================================================
// GPIO HAL Unit Tests
//=============================================================================

//-----------------------------------------------------------------------------
// read() Unit Tests
//-----------------------------------------------------------------------------

TEST_CASE("Test GPIO HAL read() pass", "[gpio-hal-read-pass]")
{
    constexpr auto valueToCheck = 7;
    pRawGPIOStub->setValue(valueToCheck);
    REQUIRE(upGpioHAL->read() == valueToCheck);
    // Reset object under test
    pRawGPIOStub->setValue(0);
}

TEST_CASE("Test GPIO HAL read() fail", "[gpio-hal-read-fail]")
{
    constexpr auto valueToCheck = 15;
    pRawGPIOStub->setValue(valueToCheck);
    pRawGPIOStub->setFail(true);
    // Fail will result in no signal
    REQUIRE(upGpioHAL->read() == 0);
    // Reset object under test
    pRawGPIOStub->setFail(false);
    pRawGPIOStub->setValue(0);
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
    constexpr auto adcValueToCheck = 31;
    pRawADCStub->setValue(adcValueToCheck);
    constexpr auto gpioValueToCheck = 63;
    pRawGPIOStub->setValue(gpioValueToCheck);
    auto acquiredData = getSignalDataFacade().acquire();
    REQUIRE(acquiredData.analog == adcValueToCheck);
    REQUIRE(acquiredData.digital == gpioValueToCheck);
    // Reset objects under test
    pRawADCStub->setValue(0);
    pRawGPIOStub->setValue(0);
}

TEST_CASE(
    "Test Signal Data Facade acquire() ADC fail", 
    "[signaldata-facade-acquire-adc-fail]")
{
    constexpr auto adcValueToCheck = 127;
    pRawADCStub->setValue(adcValueToCheck);
    pRawADCStub->setFail(true);
    constexpr auto gpioValueToCheck = 255;
    pRawGPIOStub->setValue(gpioValueToCheck);
    auto acquiredData = getSignalDataFacade().acquire();
    REQUIRE(acquiredData.analog == 0);
    REQUIRE(acquiredData.digital == gpioValueToCheck);
    // Reset objects under test
    pRawADCStub->setFail(false);
    pRawADCStub->setValue(0);
    // Reset object under test
    pRawGPIOStub->setValue(0);
}

TEST_CASE(
    "Test Signal Data Facade acquire() GPIO fail", 
    "[signaldata-facade-acquire-gpio-fail]")
{
    constexpr auto adcValueToCheck = 511;
    pRawADCStub->setValue(adcValueToCheck);
    constexpr auto gpioValueToCheck = 1023;
    pRawGPIOStub->setValue(gpioValueToCheck);
    pRawGPIOStub->setFail(true);
    auto acquiredData = getSignalDataFacade().acquire();
    REQUIRE(acquiredData.analog == adcValueToCheck);
    REQUIRE(acquiredData.digital == 0);
    // Reset objects under test
    pRawADCStub->setValue(0);
    // Reset object under test
    pRawGPIOStub->setValue(0);
    pRawGPIOStub->setFail(false);
}

TEST_CASE(
    "Test Signal Data Facade acquire() both fail", 
    "[signaldata-facade-acquire-both-fail]")
{
    constexpr auto adcValueToCheck = 2047;
    pRawADCStub->setValue(adcValueToCheck);
    pRawADCStub->setFail(true);
    constexpr auto gpioValueToCheck = 4095;
    pRawGPIOStub->setValue(gpioValueToCheck);
    pRawGPIOStub->setFail(true);
    auto acquiredData = getSignalDataFacade().acquire();
    REQUIRE(acquiredData.analog == 0);
    REQUIRE(acquiredData.digital == 0);
    // Reset objects under test
    pRawADCStub->setValue(0);
    pRawADCStub->setFail(false);
    // Reset object under test
    pRawGPIOStub->setValue(0);
    pRawGPIOStub->setFail(false);
}