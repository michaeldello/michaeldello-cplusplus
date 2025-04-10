// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.

#ifndef INCLUDE_FACADEPATTERN_H_
#define INCLUDE_FACADEPATTERN_H_
//------------------------------------------------------------------------------
//
// This header provides the types and class declarations to implement a
// Facade Design Pattern example.
//
// In this example, the client uses the SignalData class to retrieve various
// forms of signal data in real-time.
//
// The SignalData class simplifies the interaction between the client and
// the signal data sources. This design pattern decouples the complexity of
// a multi-class subsytem from the client who needs its services. The SignalData
// class also implements the Singleton design pattern to enforce a single entry
// point to the signal data sources.
//
// Additional design principles, patterns, and modern C++ features used:
//
//    1. Dependency Inversion - Introduce unit test implementations for
//       hardware abstractions. These can be used in place of the
//       Hardware Driver implementations depending on context.
//
//    2. Singleton - Ensure only a single instance of the Facade is used
//
//    3. Liskov's Substitution - Allow child class substitutions in client
//       code based on context without the client needing to know
//
//    4. C++ Smart Pointers - Ensure a single pointer to an object is managed
//       throughout the program and used in a safe manner
//
// This design pattern decouples the client logic from the complexities of
// using a complex subsytem, simplifying the client responsibilities, reducing
// the risk of breakage if the subsystem requires changes in the future, and
// improving maintainability.
//
//------------------------------------------------------------------------------

#include <cstdint>
#include <memory>
#include <optional>

namespace SignalDataFacade
{
    //==========================================================================
    // ADC
    //==========================================================================

    //--------------------------------------------------------------------------
    // Classes
    //--------------------------------------------------------------------------
    // Class: IA2DConverter
    //
    // Description:
    //    Abstract Interface for an ADC abstraction. Can't be instantiated.
    //    Must be inherited and overridden.
    //
    class IA2DConverter
    {
    public:
        virtual ~IA2DConverter() = default;
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual std::optional<uint16_t> read() = 0;
    };

    //--------------------------------------------------------------------------
    // Class: A2DConverterHAL
    //
    // Description:
    //    Abstracts the ADC in a HAL. This decouples the Application layer from
    //    the Driver interface.
    //
    class A2DConverterHAL
    {
    private:
        std::unique_ptr<IA2DConverter> upADC;
    public:
        // Avoid implicit conversion by using explicit
        explicit A2DConverterHAL(std::unique_ptr<IA2DConverter> adcImpl);
        std::optional<uint16_t> read() const;
    };

    //--------------------------------------------------------------------------
    // Class: ADCDrv
    //
    // Description:
    //    This abstracts the ADC driver code. This decouples the HAL
    //    layer from the Driver interface and OS code.
    //
    class ADCDrv: public IA2DConverter
    {
    private:
        bool bStarted;
    public:
        ADCDrv();
        void start() override;
        void stop() override;
        std::optional<uint16_t> read() override;
    };

    //==========================================================================
    // GPIO
    //==========================================================================

    //--------------------------------------------------------------------------
    // Classes
    //--------------------------------------------------------------------------
    // Class: IGPIO
    //
    // Description:
    //    Abstract Interface for an GPIO abstraction. Can't be instantiated. 
    //    Must be inherited and overridden.
    //
    class IGPIO
    {
    public:
        virtual ~IGPIO() = default;
        virtual std::optional<uint16_t> read() = 0;
    };

    //--------------------------------------------------------------------------
    // Class: GPIOHAL
    //
    // Description:
    //    Abstracts the GPIO interface in a HAL. This decouples the Application 
    //    layer from the Driver interface.
    //
    class GPIOHAL
    {
    private:
        std::unique_ptr<IGPIO> upGPIO;
    public:
        // Avoid implicit conversion by using explicit
        explicit GPIOHAL(std::unique_ptr<IGPIO> gpioImpl);
        std::optional<uint16_t> read() const;
    };

    //--------------------------------------------------------------------------
    // Class: GPIODrv
    //
    // Description:
    //    This abstracts the GPIO driver code. This decouples the HAL
    //    layer from the Driver interface and OS code.
    //
    class GPIODrv: public IGPIO
    {
    public:
        std::optional<uint16_t> read() override;
    };

    //=========================================================================
    // Signal Data Facade
    //=========================================================================

    //--------------------------------------------------------------------------
    // Classes
    //--------------------------------------------------------------------------
    // Class: SignalData
    //
    // Description:
    //    This provides a Facade for a client to access underlying signal data
    //    classes. This simplifies and decouples the client code from the 
    //    complexities of the underlying subsystem implementation.
    //
    class SignalData
    {
    private:
        std::unique_ptr<A2DConverterHAL> adc;
        std::unique_ptr<GPIOHAL> gpio;

        //----------------------------------------------------------------------
        // Singleton pattern:
        // Disallow clients to create an instance of this class
        SignalData(
            std::unique_ptr<A2DConverterHAL> adcImpl,
            std::unique_ptr<GPIOHAL> gpioImpl);
        //----------------------------------------------------------------------
        // Delete copy and assignment operators
        SignalData(const SignalData&) = delete;
        SignalData& operator=(const SignalData&) = delete;
        //----------------------------------------------------------------------

    public:
        //----------------------------------------------------------------------
        // Singleton pattern:
        // Single Instance Accessor
        static SignalData& getInstance(
            std::unique_ptr<A2DConverterHAL> adcImpl = nullptr,
            std::unique_ptr<GPIOHAL> gpioImpl = nullptr);

        // Data Accessor
        struct sAggregateData
        {
            uint16_t analog;
            uint16_t digital;
            sAggregateData(uint16_t analogData, uint16_t digitalData);
        };

        // Client's interface for obtaining data acquisition results
        sAggregateData acquire() const;
    };

} // namespace SignalDataFacade

#endif // INCLUDE_FACADEPATTERN_H_