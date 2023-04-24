/** @file ConfigKnobShimLibCommonGoogleTest.cpp
  Google tests of the ConfigKnobShimLibCommon instance.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <gtest/gtest.h>
#include <MockPeiServicesLib.h>
#include <GoogleTest/Library/MockUefiRuntimeServicesTableLib.h>

extern "C" {
  #include <Uefi.h>
  #include <Library/BaseLib.h>
  #include <Library/DebugLib.h>
}

using namespace testing;

class SetSecureBootModeTest : public Test {
  protected:
    MockUefiRuntimeServicesTableLib RtServicesMock;
    EFI_STATUS  Status;

    void SetUp() override {
      // Any random magic number can be used for these tests
      SecureBootMode = 0xAB;
    }
};

TEST_F(SetSecureBootModeTest, SetVarError) {
  EXPECT_CALL(RtServicesMock, gRT_SetVariable)
    .WillOnce(Return(EFI_INVALID_PARAMETER));

  Status = SetSecureBootMode(SecureBootMode);
  EXPECT_EQ(Status, EFI_INVALID_PARAMETER);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}