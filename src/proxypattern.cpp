// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.

//------------------------------------------------------------------------------
//
// Proxy Design Pattern Implementation
//
//------------------------------------------------------------------------------

#include "proxypattern.h"

// Socket libraries
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

#include "common/st_enum_ops.h"

namespace DUTProxy
{
    using namespace StronglyTypedEnumOps;

    //--------------------------------------------------------------------------
    // Functions
    //--------------------------------------------------------------------------
    const char* toString(eTestResults result)
    {
        switch (result)
        {
            case eTestResults::AMBIGUOUS:  return "AMBIGUOUS";
            case eTestResults::FAILED:     return "FAILED";
            case eTestResults::PASSED:     return "PASSED";
            default:                       return "UNKNOWN TEST RESULT";
        }
    }

    //--------------------------------------------------------------------------
    const char* toString(eTests test)
    {
        switch (test)
        {
            case eTests::TEST_INCOMPLETEFEATURE: return "INCOMPLETE";
            case eTests::TEST_FAILINGFEATURE:    return "FAILING";
            case eTests::TEST_PASSINGFEATURE:    return "PASSING";
            case eTests::STOP_TESTING:           return "STOP_TESTING";
            default:                             return "UNKNOWN TEST";
        }
    }

    //--------------------------------------------------------------------------
    // DUT Implementation
    //--------------------------------------------------------------------------
    DUT::DUT(sDUTConfig_t sConfig)
    : runningResult{eTestResults::NONE}, sName{sConfig.sName}
    {
        std::cout << "Creating new DUT object with name: "
                  << sName
                  << std::endl;
    }

    //--------------------------------------------------------------------------
    eTestResults DUT::execute(eTests test)
    {
        // Simulate test execution as result value is the test value, modulo
        // number of valid tests
        // Use explicit cast here, acknowledging the underlying types and
        // enum domains are interchangeable due specifically to this simulation
        // implementation
        eTestResults result{
            enum_cast<eTests, eTestResults>(test) %
                eTestResults::NUM_POSSIBLE_TEST_RESULTS};

        if (eTests::STOP_TESTING == test)
        {
            result = runningResult;
            // Reset running result
            runningResult = eTestResults::NONE;
        }
        else
        {
            if (eTestResults::NONE == runningResult)
            {
                // Start
                runningResult = result;
            }
            else if (eTestResults::FAIL == result)
            {
                // Latch failure
                runningResult = eTestResults::FAILED;
            }
            else
            {
                // Accumulate
                runningResult |= result;

                // Pass running result through filter to persist failures
                runningResult =
                    (eTestResults::FAILED ==
                        (eTestResults::FAILED & runningResult)) ?
                    eTestResults::FAILED : (runningResult | result);
            }
        }

        return result;
    }

    //--------------------------------------------------------------------------
    // Socket Class Implementation
    //--------------------------------------------------------------------------
    Socket::Socket(int domain, int type, int protocol)
    {
        // Use globally available socket function
        fd = ::socket(domain, type, protocol);
        if (fd < 0)
        {
            throw std::runtime_error(
                "Failed to create socket" +
                std::string(std::strerror(errno)));
        }
    }

    //--------------------------------------------------------------------------
    Socket::Socket(int fd): fd(fd)
    {
        if (fd < 0)
        {
            throw std::invalid_argument("Invalid socket file descriptor");
        }
    }

    //--------------------------------------------------------------------------
    Socket::~Socket()
    {
        if (fd >= 0)
        {
            // Use globally available close() function
            ::close(fd);
        }
    }

    //--------------------------------------------------------------------------
    Socket::Socket(Socket&& owned) noexcept: fd(owned.fd)
    {
        owned.fd = -1;
    }

    //--------------------------------------------------------------------------
    Socket& Socket::operator=(Socket&& owned) noexcept
    {
        if (this != &owned)
        {
            if (fd >= 0)
            {
                // Use globally available socket close
                ::close(fd);
            }
            fd = owned.fd;
            owned.fd = -1;
        }

        return *this;
    }

    //--------------------------------------------------------------------------
    int Socket::get() const
    {
        return fd;
    }

    //--------------------------------------------------------------------------
    void Socket::setReceiveTimeout(int seconds)
    {
        timeval timeout{seconds,0};
        if (setsockopt(
                fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
        {
            throw std::runtime_error(
                "Failed to set timeout" +
                std::string(std::strerror(errno)));
        }
    }

    //---------------------------------------------------------------------------
    // DUTProxyClient Implementation
    //---------------------------------------------------------------------------
    DUTProxyClient::DUTProxyClient(sRemoteDUTConfig_t sConfig)
    // Avoid extra copies, move instead
    : sDUTName(std::move(sConfig.sName)),
      sDUTIPAddr(std::move(sConfig.sIPAddr)),
      socket(AF_INET, SOCK_STREAM, 0)
    {
        std::cout << "Creating new DUTProxyClient for DUT: ("
                  << sDUTName
                  << ", "
                  << sDUTIPAddr
                  << ")"
                  << std::endl;

        connectToServer();
    }

    //---------------------------------------------------------------------------
    void DUTProxyClient::connectToServer()
    {
        // Define the server connect address
        sockaddr_in serverAddr
        {
            .sin_family = AF_INET,
            .sin_port = htons(DUT_PROXY_TCP_PORT),
            .sin_addr = {.s_addr = inet_addr(sDUTIPAddr.c_str())}
        };

        // Connect to the server using the globally available connect()
        // function
        if (::connect(
                socket.get(),
                reinterpret_cast<sockaddr*>(&serverAddr),
                sizeof(serverAddr)) < 0)
        {
            throw std::runtime_error(
                "Connection failed on: " +
                std::string(std::strerror(errno)));
        }

        socket.setReceiveTimeout(5);

        std::cout << "Connected to server at "
                    << sDUTIPAddr
                    << ":"
                    << DUT_PROXY_TCP_PORT
                    << std::endl;
    }

    //---------------------------------------------------------------------------
    eTestResults DUTProxyClient::execute(eTests test)
    {
        eTestResults result{eTestResults::INCOMPLETE};

        uint16_t request = static_cast<uint16_t>(test);

        // Send request to server, return result
        ssize_t sent = send(socket.get(), &request, sizeof(request), 0);

        if (sent != sizeof(request))
        {
            std::cerr << "Unexpected: Failed to send Request" << std::endl;
        }
        else
        {
            // Receive the result
            uint16_t rawRsp;
            ssize_t received = recv(socket.get(), &rawRsp, sizeof(rawRsp), 0);
            // Evaluate the result
            if (received == 0)
            {
                std::cerr << "Server closed connection" << std::endl;
            }
            else if (received < 0)
            {
                std::cerr << "Receive error: " << strerror(errno) << std::endl;
            }
            else
            {
                result = static_cast<eTestResults>(rawRsp);
            }
        }

        return result;
    }

    //---------------------------------------------------------------------------
    // DUTProxyServer Implementation
    //---------------------------------------------------------------------------
    DUTProxyServer::DUTProxyServer(DUT &targetDUT)
    : dut(targetDUT), running(false), serverSocket(AF_INET, SOCK_STREAM, 0)
    {
        // Set and bind socket to this host
        sockaddr_in addr
        {
            .sin_family = AF_INET,
            .sin_port = htons(DUT_PROXY_TCP_PORT),
            .sin_addr = {.s_addr = INADDR_ANY}
        };
        int opt = 1;
        // Use globally available socket option function
        ::setsockopt(
            serverSocket.get(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        // Use globally available socket bind function
        if (::bind(
                serverSocket.get(),
                reinterpret_cast<sockaddr*>(&addr),
                sizeof(addr)) < 0)
        {
            throw std::runtime_error(
                "Bind failed" + std::string(std::strerror(errno)));
        }

        // Start listening using the globally available function
        if (::listen(serverSocket.get(), SOMAXCONN) < 0)
        {
            throw std::runtime_error(
                "Listen failed" + std::string(std::strerror(errno)));
        }

        // Start the server loop
        running = true;
        serverThread = std::thread(&DUTProxyServer::ServerEntry, this);
    }

    //---------------------------------------------------------------------------
    DUTProxyServer::~DUTProxyServer()
    {
        std::cout << "Shutting down DUTProxyServer" << std::endl;

        // Discontinue server thread's loop
        running = false;

        // Force-close the socket to unblock accept() using the globally
        // available function
        ::shutdown(serverSocket.get(), SHUT_RDWR);

        // Wait for server thread to exit
        if (serverThread.joinable())
        {
            serverThread.join();
        }

        std::cout << "DUTProxyServer is shut down" << std::endl;
    }

    //---------------------------------------------------------------------------
    void DUTProxyServer::ServerEntry()
    {
        while (running)
        {
            sockaddr_in clientAddr{};
            socklen_t clientLen = sizeof(clientAddr);

            // Accept incoming connection using globally available function
            int clientSocket =
                ::accept(
                    serverSocket.get(),
                    reinterpret_cast<sockaddr*>(&clientAddr),
                    &clientLen);
            if (clientSocket < 0)
            {
                // Stop if shutting down
                if (!running) break;
                std::cerr << "Accept failed" << std::endl;
                continue;
            }

            // Handle client inline, client will close the connection when it
            // is done
            handleClient(Socket(clientSocket));

            close(clientSocket);
        }
    }

    //---------------------------------------------------------------------------
    void DUTProxyServer::handleClient(Socket&& clientSocket)
    {
        while (running)
        {
            std::cout << "Processing client requests" << std::endl;
            uint16_t rawRequest{0};
            // Receive request using globally available recv()
            ssize_t bytes =
                ::recv(clientSocket.get(), &rawRequest, sizeof(rawRequest), 0);
            if (bytes <= 0)
            {
                std::cout << "Socket Receive: no data" << std::endl;
                // Assume this condition means the socket has been shut down by
                // the client and stop processing
                break;
            }

            eTests testToRun = static_cast<eTests>(rawRequest);
            std::cout << "Running test: " << toString(testToRun) << std::endl;

            eTestResults result = dut.execute(testToRun);
            std::cout << "Result: " << toString(result) << std::endl;

            uint16_t rawResult = static_cast<uint16_t>(result);
            // Send using globally available send()
            ::send(clientSocket.get(), &rawResult, sizeof(rawResult), 0);
        }
    }

} // DUTProxy