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

    // Tests that can be run, along with a stopping condition that provides an 
    // overall result based on testing since the start of testing, or last stop
    // condition
    enum class eTests: uint16_t
    {
        ETESTS_FUNCTION1,
        ETESTS_FUNCTION2,
        ETESTS_FUNCTION3,
        ETESTS_STOP_TESTING = 0xFFFF
    };

    // Testing result conditions, for both individual tests, and overall
    // assessment (individual results AND'd together)
    enum eTestResults: uint8_t
    {
        ETESTRESULTS_FAIL       = 0x0,
        ETESTRESULTS_PASS       = 0x1,
        ETESTRESULTS_INCOMPLETE = 0x2,
        ETESTRESULTS_AMBIGUOUS  = 0x3
    };

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
        virtual eTestResults execute(eTests test) override;
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
    private:
        int socketFD;
    public:
        DUTProxyClient(sRemoteDUTConfig_t sConfig);
        eTestResults execute(eTests test) override;
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
    private:
        // Methods
        void ServerEntry();
        int handleClient(int clientSocket);

        // Data Members
        DUT &dut;
        std::thread serverThread;
        // Use a thread-safe variable to coordinate stopping the server thread
        // from higher level context (destructor call)
        std::atomic<bool> running;
        int serverSocket;
    public:
        DUTProxyServer(DUT &targetDUT);
        // Destructor will end server thread
        ~DUTProxyServer();
        void Start();
    };

} // namespace DUTProxy

#endif // INCLUDE_PROXYPATTERN_H_