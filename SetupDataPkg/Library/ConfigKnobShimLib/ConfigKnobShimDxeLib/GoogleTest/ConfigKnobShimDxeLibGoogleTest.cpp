/** @file ConfigKnobShimDxeLibCommonGoogleTest.cpp
  Google tests of the ConfigKnobShimLibCommon instance.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <gtest/gtest.h>
#include <GoogleTest/Library/MockUefiRuntimeServicesTableLib.h>
#include "../../GoogleTest/ConfigKnobShimmLibCommonGoogleTest.h"

extern "C" {
  #include <Uefi.h>
  #include <Library/BaseLib.h>
  #include <Library/DebugLib.h>
  // include the c file to be able to unit test static function
  #include "../../ConfigKnobShimLibCommon.h"
}

using namespace testing;

// TODO maaybe need RtServicesMock in header file ?then it's initialized in common?
// helper function with expect call to return EFI_NOT_FOUND
//todo need to know variable size to return.. that's okay, right? this is a specific test...
void expectMockGetVariableBuffSmall(VOID *VarData) {
  UINT64 VariableData = (UINT64) VarData;

  EXPECT_CALL(RtServicesMock, gRT_GetVariable)
    .WillOnce(DoAll(
      SetArgPointee<3>(sizeof(VariableData)), 
      Return(EFI_BUFFER_TOO_SMALL)));
}

// void expectMockGetVariableRet(VOID *VarData) {
//   UINT64 VariableData = (UINT64) VarData;
//   EXPECT_CALL(RtServicesMock, gRT_GetVariable)
//     .WillOnce(DoAll(
//       SetArgPointee<3>(sizeof(VariableData)), 
//       SetArgBuffer<4>(&VariableData, sizeof(VariableData)), 
//       Return(EFI_SUCCESS))); 
// }