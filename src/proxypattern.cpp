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
#include <iostream>

namespace DUTProxy
{
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
        return eTestResults::NONE;
    }

} // DUTProxy