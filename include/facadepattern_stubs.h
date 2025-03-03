#ifndef FACADEPATTERNSTUBS_H
#define FACADEPATTERNSTUBS_H
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

namespace SignalDataFacade
{
    //=========================================================================
    // ADC
    //=========================================================================

    //-------------------------------------------------------------------------
    // Classes
    //-------------------------------------------------------------------------
    // Implementation for Unit Test context
    class ADCDrvStub: public IA2DConverter
    {
    private:
        bool bStarted;
        bool bFail;
        uint16_t valueToReturn;
    public:
        ADCDrvStub();
        void start() override;
        void stop() override;
        std::optional<uint16_t> read() const override;
        void setValue(uint16_t value);
        void setFail(bool bFailSet);
    };

    //=========================================================================
    // GPIO
    //=========================================================================

    //-------------------------------------------------------------------------
    // Classes
    //-------------------------------------------------------------------------
    // Implementation for Unit Test context
    class GPIODrvStub: public IGPIO
    {
    private:
        bool bFail;
        uint16_t valueToReturn;
    public:
        GPIODrvStub();
        std::optional<uint16_t> read() const override;
        void setFail(bool fail);
        void setValue(uint16_t value);
    };

} // namespace SignalDataFacade

#endif // FACADEPATTERNSTUBS_H