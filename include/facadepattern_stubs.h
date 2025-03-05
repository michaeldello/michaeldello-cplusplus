// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.

#ifndef INCLUDE_FACADEPATTERN_STUBS_H_
#define INCLUDE_FACADEPATTERN_STUBS_H_
//-----------------------------------------------------------------------------
//
// This header provides stubbed implementations for hardware interfacing
// classes used to implement the Facade Design Pattern.
//
// This decouples build units used for stubs used for unit testing from the
// main build units for cleaner maintenance and build outputs.
//
//-----------------------------------------------------------------------------

#include "facadepattern.h"
#include <cstdint>

namespace SignalDataFacade
{
    //-------------------------------------------------------------------------
    // Classes
    //-------------------------------------------------------------------------

    //=========================================================================
    // ADC
    //=========================================================================

    // Implementation for Unit Test context
    class ADCDrvStub: public IA2DConverter
    {
    private:
        bool bStarted;
    public:
        static uint16_t testValue;
        static bool bFail;
        ADCDrvStub();
        void start() override;
        void stop() override;
        std::optional<uint16_t> read() const override;
    };

    //=========================================================================
    // GPIO
    //=========================================================================

    // Implementation for Unit Test context
    class GPIODrvStub: public IGPIO
    {
    public:
        static uint16_t testValue;
        static bool bFail;
        std::optional<uint16_t> read() const override;
    };

} // namespace SignalDataFacade

#endif // INCLUDE_FACADEPATTERN_STUBS_H_