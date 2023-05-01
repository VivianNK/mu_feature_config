/** @file ConfigKnobShimLibCommonGoogleTest.cpp
  Google tests of the ConfigKnobShimLibCommon instance.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef CONFIG_KNOB_SHIM_LIB_COMMON_GOOGLE_TEST_
#define CONFIG_KNOB_SHIM_LIB_COMMON_GOOGLE_TEST_


#include <gtest/gtest.h>
#include <ConfigKnobShimmLibCommonGoogleTest.h>


extern "C" {
  #include <Uefi.h>
  #include <Library/BaseLib.h>
  #include <Library/DebugLib.h>
}


// helper function with expect call
void expectMockGetVariableNotFound(
  OUT VOID *VariableData
);


// helper function with expect call
void expectMockGetVariableRet(
  OUT VOID *VariableData
);

#endif
