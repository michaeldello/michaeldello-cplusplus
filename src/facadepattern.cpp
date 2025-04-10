// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.

//-----------------------------------------------------------------------------
//
// Facade Design Pattern Implementation
//
//-----------------------------------------------------------------------------

#include "facadepattern.h"
#include <iostream>
#include <memory>
#include <random>
#include <utility>

namespace SignalDataFacade
{
    //===========================================================================
    // ADC Implementation
    //===========================================================================

    //---------------------------------------------------------------------------
    // ADC HAL Implementation
    //---------------------------------------------------------------------------
    A2DConverterHAL::A2DConverterHAL(std::unique_ptr<IA2DConverter> adcImpl)
    : upADC{std::move(adcImpl)}
    {
        std::cout << "Creating new ADC HAL object" << std::endl;
    }

    //---------------------------------------------------------------------------
    std::optional<uint16_t> A2DConverterHAL::read() const
    {
        // Collect ADC data
        upADC->start();
        // If no value, simulate no signal by returning 0
        std::optional<uint16_t> currentADCSignalRead = upADC->read().value_or(0);
        upADC->stop();

        return currentADCSignalRead;
    }

    //-------------------------------------------------------------------------
    // ADC Driver Implementation
    //-------------------------------------------------------------------------
    ADCDrv::ADCDrv(): bStarted{false}
    {
        std::cout << "Creating new ADC Driver object" << std::endl;
    }

    //-------------------------------------------------------------------------
    void ADCDrv::start()
    {
        bStarted = true;
    }

    //-------------------------------------------------------------------------
    void ADCDrv::stop()
    {
        bStarted = false;
    }

    //-------------------------------------------------------------------------
    std::optional<uint16_t> ADCDrv::read()
    {
        // Read only if started
        if (bStarted)
        {
            // Return a random value simulating real hardware
            static std::random_device rd;
            static std::mt19937 rng(rd());
            static std::uniform_int_distribution<uint16_t> dist(0, 65535);
            return dist(rng);
        }
        // Default is something is wrong, return nothing
        return {};
    }

    //===========================================================================
    // GPIO Implementation
    //===========================================================================

    //---------------------------------------------------------------------------
    // GPIO HAL Implementation
    //---------------------------------------------------------------------------
    GPIOHAL::GPIOHAL(std::unique_ptr<IGPIO> gpioImpl)
    : upGPIO{std::move(gpioImpl)}
    {
        std::cout << "Creating new GPIO HAL object" << std::endl;
    }

    //---------------------------------------------------------------------------
    std::optional<uint16_t> GPIOHAL::read() const
    {
        // Collect GPIO data
        // If no value, simulate no signal by returning 0
        std::optional<uint16_t> currentGPIOSignalRead =
            upGPIO->read().value_or(0);

        return currentGPIOSignalRead;
    }

    //-------------------------------------------------------------------------
    // GPIO Driver Implementation
    //-------------------------------------------------------------------------
    std::optional<uint16_t> GPIODrv::read()
    {
        // Return a random value simulating real hardware
        static std::random_device rd;
        static std::mt19937 rng(rd());
        static std::uniform_int_distribution<uint16_t> dist(0, 65535);
        return dist(rng);
    }

    //===========================================================================
    // Facade Implementation
    //===========================================================================

    //---------------------------------------------------------------------------
    // Signal Data Implementation
    //---------------------------------------------------------------------------
    SignalData::SignalData(
        std::unique_ptr<A2DConverterHAL> adcImpl,
        std::unique_ptr<GPIOHAL> gpioImpl)
    : adc{std::move(adcImpl)}, gpio{std::move(gpioImpl)}
    {
        std::cout << "Creating new Signal Data object" << std::endl;
    }

    //---------------------------------------------------------------------------
    // Singleton Instance
    SignalData& SignalData::getInstance(
        std::unique_ptr<A2DConverterHAL> adcImpl,
        std::unique_ptr<GPIOHAL> gpioImpl)
    {
        // Transfer ownership if implementations provided,
        // otherwise create new unique implementations
        static SignalData instance(
            adcImpl ? std::move(adcImpl) :
                std::make_unique<A2DConverterHAL>(std::make_unique<ADCDrv>()),
            gpioImpl ? std::move(gpioImpl) :
                std::make_unique<GPIOHAL>(std::make_unique<GPIODrv>()));

        return instance;
    }

    //---------------------------------------------------------------------------
    SignalData::sAggregateData::sAggregateData(
        uint16_t analogData,
        uint16_t digitalData)
    : analog{analogData}, digital{digitalData}
    {
        std::cout << "Creating new Aggregate Data object" << std::endl;
    }

    //---------------------------------------------------------------------------
    SignalData::sAggregateData SignalData::acquire() const
    {
        // Collect Analog data component
        // If no value, simulate no signal by returning 0
        uint16_t analogDataValue = adc->read().value_or(0);

        // Collect digital data component
        // If no value, simulate no signal by returning 0
        uint16_t digitalDataValue = gpio->read().value_or(0);

        return sAggregateData(analogDataValue,digitalDataValue);
    }

} // SignalDataFacade