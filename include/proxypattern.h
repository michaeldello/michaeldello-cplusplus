// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.

#ifndef INCLUDE_PROXYPATTERN_H_
#define INCLUDE_PROXYPATTERN_H_
//------------------------------------------------------------------------------
//
// This header provides the types and class declarations to implement a
// Proxy Design Pattern example.
//
// In this example, the client uses the IDUT interface class to execute a test 
// on a DUT (Device Under Test). A remote DUT proxy class adds the functionality
// to the DUT class in order to communicate with it over a TCP/IP network 
// connection.
//
// In this way, the client need not be concerned about the network communication
// functionality needed to command the DUT.
//
// For simplicity, the IDUT interface blocks.
//
// This design pattern decouples the client logic from the complexities of
// using an object that the proxy implements, in this case, a TCP/IP network
// connection.
//
// Additional design principles, patterns, and modern C++ features used:
//
//    1. Dependency Inversion - Introduce unit test implementations for
//       hardware abstractions. These can be used in place of the
//       Hardware Driver implementations depending on context.
//
//    2. Liskov's Substitution - Allow child class substitutions in client
//       code based on context without the client needing to know
//
//------------------------------------------------------------------------------

#include <atomic>
#include <string>
#include <thread>

namespace DUTProxy
{
    //--------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------

    // Define a single instance of the constant representing the proxy TCP port
    inline constexpr uint16_t DUT_PROXY_TCP_PORT = 42042;

    // Testing result conditions, for both individual tests, and overall
    // assessment (individual results AND'd together)
    enum class eTestResults: uint16_t
    {
        // Individual Test Results
        INCOMPLETE    = 0x0,
        FAIL          = 0x1,
        PASS          = 0x2,
        NUM_POSSIBLE_TEST_RESULTS,
        // Overall Results
        FAILED     = FAIL, // Any single test failed
        PASSED     = PASS, // All single tests passed
        AMBIGUOUS  = INCOMPLETE, // Nothing passed or failed, but not NONE
        NONE       = 0xFF  // Default, no testing yet
    };

    // Convert a test result enum value to a string (literal, safe to return 
    // its address in read-only static memory)
    const char* toString(eTestResults result);

    // Tests that can be run, along with a stopping condition that provides an 
    // overall result based on testing since the start of testing, or last stop
    // condition
    enum class eTests: uint16_t
    {
        TEST_INCOMPLETEFEATURE,
        TEST_FAILINGFEATURE,
        TEST_PASSINGFEATURE,
        STOP_TESTING = 0xFFFF
    };

    // Convert a test enum value to a string (literal, safe to return
    // its address in read-only static memory)
    const char* toString(eTests test);

    //--------------------------------------------------------------------------
    // PODs
    //--------------------------------------------------------------------------
    struct sDUTConfig_t
    {
        std::string sName;
    };

    struct sRemoteDUTConfig_t: public sDUTConfig_t
    {
        std::string sIPAddr;
    };

    //--------------------------------------------------------------------------
    // Classes
    //--------------------------------------------------------------------------
    // Class: IDUT
    //
    // Description:
    //    Abstract Interface for a Device Under Test (DUT) abstraction. Can't be
    //    instantiated. Must be inherited and implemented.
    //
    class IDUT
    {
    public:
        virtual ~IDUT() = default;
        virtual eTestResults execute(eTests test) = 0;
    };

    //--------------------------------------------------------------------------
    // Class: DUT
    //
    // Description:
    //    A concrete DUT which can either be in the local memory space, or a 
    //    remote memory space, in which case, a proxy is needed.
    //
    class DUT: public IDUT
    {
    public:
        DUT(sDUTConfig_t sConfig);
        // Make this method available for override in a subclass, for unit test
        // stubbing
        //
        // Implement this in a predictable way since this is just an example:
        //     The result of running a test is the numerical value of the
        //     requested test, modulo the number of possible individual test
        //     result values
        //
        // A "stop" condition returns and resets the overal test result
        virtual eTestResults execute(eTests test) override;
    private:
        eTestResults runningResult;
        std::string sName;
    };

    //--------------------------------------------------------------------------
    // Socket Class
    //--------------------------------------------------------------------------
    //
    // Abstract a C-style socket object for use in the DUT proxy classes
    //
    //--------------------------------------------------------------------------
    class Socket
    {
    public:
        // Force explicit contruction to avoid implicit conversions
        // From C-style socket specs
        explicit Socket(int domain, int type, int protocol);
        // From socket file descriptors (adopting)
        explicit Socket(int fd);
        ~Socket();

        // Disable standard copy and assignment:
        // underlying data (file descriptor) is a system resource
        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

        // Allow ownership transfer as it will maintain system resource
        // coherence
        Socket(Socket&& owned) noexcept;
        Socket& operator=(Socket&& owned) noexcept;

        //----------------------------------------------------------------------
        int get() const;

        //----------------------------------------------------------------------
        void setReceiveTimeout(int seconds);

    private:
        // Underlying socket file descriptor
        int fd;
    };

    //--------------------------------------------------------------------------
    // Class: DUTProxyClient
    //
    // Description:
    //    A concrete DUT with the additional functionality of a Proxy, in this
    //    case, a TCP client that connects to a remote server to execute tests
    //    on a remote DUT object.
    //
    class DUTProxyClient: public IDUT
    {
    public:
        DUTProxyClient(sRemoteDUTConfig_t sConfig);

        eTestResults execute(eTests test) override;

    private:
        // Data members
        Socket socket;
        std::string sDUTName;
        std::string sDUTIPAddr;
        // Methods
        void connectToServer();
    };

    //--------------------------------------------------------------------------
    // Class: DUTProxyServer
    //
    // Description:
    //    A concrete DUT with the additional functionality of a Proxy, in this
    //    case, a TCP server that services requests from a connecting client to
    //    execute tests on a DUT object with which it has an association.
    //
    class DUTProxyServer
    {
    public:
        // Constructor will start server thread
        DUTProxyServer(DUT &targetDUT);
        // Destructor will end server thread
        ~DUTProxyServer();
    private:
        // Methods
        void ServerEntry();
        void handleClient(Socket&& clientSocket);

        // Data Members
        DUT &dut;
        std::thread serverThread;
        // Use a thread-safe variable to coordinate stopping the server thread
        // from higher level context (destructor call)
        std::atomic<bool> running;
        Socket serverSocket;
    };

} // namespace DUTProxy

#endif // INCLUDE_PROXYPATTERN_H_