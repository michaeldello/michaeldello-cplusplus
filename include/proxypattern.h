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

namespace DUTProxy
{
    //--------------------------------------------------------------------------
    // Classes
    //--------------------------------------------------------------------------

} // namespace DUTProxy

#endif // INCLUDE_PROXYPATTERN_H_