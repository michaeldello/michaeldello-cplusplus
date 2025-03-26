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

} // DUTProxy