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

// helper function with expect call to return EFI_NOT_FOUND
// todo need to know variable size to return.. that's okay, right? this is a specific test...
void
expectMockGetVariable (
  UINT64                           *VarData,
  MockUefiRuntimeServicesTableLib  *RtServicesMockPtr
  )
{
  DEBUG ((DEBUG_INFO, "%a: expectMockGetVariable \n", __FUNCTION__));

  // expect the first GetVariable call to get size
  Expectation  firstCall = EXPECT_CALL (
                             *RtServicesMockPtr,
                             gRT_GetVariable
                             )
                             .WillOnce (
                                DoAll (
                                  SetArgPointee<3>(sizeof (VarData)),
                                  Return (EFI_BUFFER_TOO_SMALL)
                                  )
                                );

  // expect the second getVariable call to update data
  EXPECT_CALL (
    *RtServicesMockPtr,
    gRT_GetVariable
    )
    .After (firstCall)
    .WillOnce (
       DoAll (
         SetArgPointee<3>(sizeof (VarData)),
         SetArgBuffer<4>(&VarData, sizeof (VarData)),
         Return (EFI_SUCCESS)
         )
       );
}

// helper function with expect call to return EFI_NOT_FOUND
// todo need to know variable size to return.. that's okay, right? this is a specific test...
void
expectMockGetVariableBuffSmall (
  VOID                             *VarData,
  MockUefiRuntimeServicesTableLib  *RtServicesMockPtr
  )
{
  UINT64  VariableData = (UINT64)VarData;

  EXPECT_CALL (*RtServicesMockPtr, gRT_GetVariable)
    .WillOnce (
       DoAll (
         SetArgPointee<3>(sizeof (VariableData)),
         Return (EFI_BUFFER_TOO_SMALL)
         )
       );
}

// BROKEN because variable data is void ptr ahhhh
void
expectMockGetVariableSuccess (
  VOID                             *VarData,
  MockUefiRuntimeServicesTableLib  *RtServicesMockPtr
  )
{
  // UINT64 *VariableData = (UINT64 *) VarData;
  DEBUG ((DEBUG_INFO, "%a: expectMockGetVariableSuccess \n", __FUNCTION__));

  EXPECT_CALL (
    *RtServicesMockPtr,
    gRT_GetVariable (
      _,
      _,
      _,
      _,
      NotNull ()
      )
    )
    .WillOnce (
       DoAll (
         SetArgPointee<3>(sizeof (UINT64)),
         SetArgBuffer<4>((UINT64 *)VarData, sizeof (UINT64)),
         Return (EFI_SUCCESS)
         )
       );
}

// EXPECT_CALL(RtServicesMock,
//   gRT_GetVariable(
//     Char16StrEq(ConfigKnobName),
//     BufferEq(&ConfigKnobGuid, sizeof(EFI_GUID)),
//     _,
//     Pointee(Eq(sizeof(ConfigKnobData))),
//     NotNull()))
//   .WillOnce(DoAll(
//     SetArgPointee<3>(sizeof(VariableData)),
//     SetArgBuffer<4>(&VariableData, sizeof(VariableData)),
//     Return(EFI_SUCCESS)));
