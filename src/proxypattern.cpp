// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.

//-----------------------------------------------------------------------------
//
// Proxy Design Pattern Implementation
//
//-----------------------------------------------------------------------------

#include "proxypattern.h"
#include "common/st_enum_ops.h"
#include <iostream>
#include <stdexcept>

// Socket libraries
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

namespace DUTProxy
{
    using namespace StronglyTypedEnumOps;

    //---------------------------------------------------------------------------
    // DUT Implementation
    //---------------------------------------------------------------------------
    DUT::DUT(sDUTConfig_t sConfig)
    : runningResult{eTestResults::NONE}, sName{sConfig.sName}
    {
        std::cout << "Creating new DUT object with name: " << sName << std::endl;
    }

    //---------------------------------------------------------------------------
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

    //---------------------------------------------------------------------------
    // DUTProxyClient Implementation
    //---------------------------------------------------------------------------
    DUTProxyClient::DUTProxyClient(sRemoteDUTConfig_t sConfig)
    : sDUTName{sConfig.sName}, sDUTIPAddr{sConfig.sIPAddr}
    {
        std::cout << "Creating new DUTProxyClient for DUT: (" 
                  << sDUTName 
                  << ", " 
                  << sDUTIPAddr 
                  << ")" 
                  << std::endl;
    }

    //---------------------------------------------------------------------------
    eTestResults DUTProxyClient::execute(eTests test)
    {
        // Send request to server, return result

        eTestResults result{eTestResults::NONE};

        return result;
    }

    //---------------------------------------------------------------------------
    // DUTProxyServer Implementation
    //---------------------------------------------------------------------------
    DUTProxyServer::DUTProxyServer(DUT &targetDUT)
    : dut{targetDUT}, running{false}, serverSocket{-1}
    {
        // Create socket
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0)
        {
            throw std::runtime_error("Failed to create DUTProxyServer socket");
        }

        // Set socket options
        int opt = 1;
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        // Bind socket
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(DUT_PROXY_TCP_PORT);
        if (bind(serverSocket, (sockaddr*)&addr, sizeof(addr)) < 0)
        {
            close(serverSocket);
            throw std::runtime_error("Bind failed");
        }

        // Start listening
        if (listen(serverSocket, SOMAXCONN) < 0)
        {
            close(serverSocket);
            throw std::runtime_error("Listen failed");
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

        // Force-close the socket to unblock accept()
        shutdown(serverSocket, SHUT_RDWR);
        close(serverSocket);

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
            
            // Accept incoming connection
            int clientSocket =
                accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
            if (clientSocket < 0)
            {
                // Stop if shutting down
                if (!running) break;
                std::cerr << "Accept failed" << std::endl;
                continue;
            }

            // Handle client inline, client will close the connection when it
            // is done
            handleClient(clientSocket);

            close(clientSocket);
        }
    }

    //---------------------------------------------------------------------------
    void DUTProxyServer::handleClient(int clientSocket)
    {
        std::cout << "Processing client requests" << std::endl;
    }

} // DUTProxy