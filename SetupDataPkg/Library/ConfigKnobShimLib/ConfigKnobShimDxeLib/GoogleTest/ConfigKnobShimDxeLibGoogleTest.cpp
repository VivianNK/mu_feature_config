/** @file ConfigKnobShimLibCommonGoogleTest.cpp
  Google tests of the ConfigKnobShimLibCommon instance.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <gtest/gtest.h>
#include <GoogleTest/Library/MockUefiRuntimeServicesTableLib.h>

extern "C" {
  #include <Uefi.h>
  #include <Library/BaseLib.h>
  #include <Library/DebugLib.h>
  // include the c file to be able to unit test static function
  #include "../../ConfigKnobShimLibCommon.h"
}

#define CONFIG_KNOB_GUID  {0x52d39693, 0x4f64, 0x4ee6, {0x81, 0xde, 0x45, 0x89, 0x37, 0x72, 0x78, 0x55}}

using namespace testing;

extern MockUefiRuntimeServicesTableLib RtServicesMock;

// TODO maaybe need RtSeervicesMock in header file ?then it's initialized in common?
// helper function with expect call to return EFI_NOT_FOUND
//todo need to know variable size to return.. that's okay, right? this is a specific test...
void expectMockGetVariableNotFound(UINT64 VariableData) {
  // cast void * var data to uint64
  EXPECT_CALL(RtServicesMock, gRT_GetVariable)
    .WillOnce(DoAll(
      SetArgPointee<3>(sizeof(VariableData)), 
      Return(EFI_NOT_FOUND)));
}

void expectMockGetVariableRet(UINT64 VariableData) {
  EXPECT_CALL(RtServicesMock, gRT_GetVariable)
    .WillOnce(DoAll(
      SetArgPointee<3>(sizeof(VariableData)), 
      SetArgBuffer<4>(&VariableData, sizeof(VariableData)), 
      Return(EFI_SUCCESS))); 
}