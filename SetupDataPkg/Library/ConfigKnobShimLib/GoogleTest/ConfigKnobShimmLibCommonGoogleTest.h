/** @file ConfigKnobShimLibCommonGoogleTest.cpp
  Google tests of the ConfigKnobShimLibCommon instance.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef CONFIG_KNOB_SHIM_LIB_COMMON_GOOGLE_TEST_
#define CONFIG_KNOB_SHIM_LIB_COMMON_GOOGLE_TEST_


#include <gtest/gtest.h>

extern "C" {
  #include <Uefi.h>
  #include <Library/BaseLib.h>
  #include <Library/DebugLib.h>
}

// todo try creating new() to store on heap ??
extern MockUefiRuntimeServicesTableLib RtServicesMock;

// helper function with expect call
//  function to determine which getvariable to use. based on unit test inf inclusion. 
// put th eexpect call into the functions in dxe/pei specific files 
void expectMockGetVariableBuffSmall(
  OUT VOID *VariableData
);


// helper function with expect call
void expectMockGetVariableRet(
  OUT VOID *VariableData
);

#endif
