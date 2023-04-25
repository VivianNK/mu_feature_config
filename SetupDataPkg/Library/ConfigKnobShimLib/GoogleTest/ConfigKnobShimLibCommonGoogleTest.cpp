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

#define CONFIG_KNOB_GUID  {0x52d39693, 0x4f64, 0x4ee6, {0x81, 0xde, 0x45, 0x89, 0x37, 0x72, 0x78, 0x55}}

using namespace testing;

class GetConfigKnobOverrideTest : public Test {
  protected:
    EFI_GUID    ConfigKnobGuid;
    EFI_STATUS  Status;
    CHAR16      *ConfigKnobName;
    UINTN       ProfileDefaultSize;
    UINT64      ConfigKnobData;

    void SetUp() override {
      ConfigKnobGuid      = CONFIG_KNOB_GUID;
      ConfigKnobName      = L"MyDeadBeefDelivery";
      ProfileDefaultSize  = sizeof (ConfigKnobData);
    }
};

/**
 * todo - all these tests are testing, what happens when the getvariable call returns EFI_INVALID_PARAMETER
 * @ Oliver is there anything else in GetConfigKnobbOverride you wanted to test? that the data is not changed if the params are invalid?
 * if want them all, should break out into separate TEST_Fs
*/
TEST_F(GetConfigKnobOverrideTest, InvalidParamError) {

  // Variable name is NULL
  Status = GetConfigKnobOverride (&ConfigKnobGuid, NULL, (VOID *)&ConfigKnobData, ProfileDefaultSize);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);

  // Vendor GUID is NULL
  Status = GetConfigKnobOverride (NULL, ConfigKnobName, (VOID *)&ConfigKnobData, ProfileDefaultSize);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);

  // DataSize is NULL
  Status = GetConfigKnobOverride (&ConfigKnobGuid, ConfigKnobName, (VOID *) &ConfigKnobData, 0);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);

  // DataSize is not too small and the Data is NULL
  Status = GetConfigKnobOverride (&ConfigKnobGuid, ConfigKnobName, NULL, ProfileDefaultSize);
  ASSERT_EQ (Status, EFI_INVALID_PARAMETER);

}

class GetConfigKnobOverrideFromVariableStorageTest : public Test {
  protected:
    MockUefiRuntimeServicesTableLib RtServicesMock;
    
    EFI_STATUS  Status;
    EFI_GUID    ConfigKnobGuid;
    CHAR16      *ConfigKnobName;
    UINTN       ProfileDefaultSize;
    UINT64      ProfileDefaultValue;
    UINTN       ProfileDefaultSize;
    UINT64      VariableData;
    PPI_STATUS  PpiStatus;
    UINT64      ConfigKnobData;

    void SetUp() override {
      ConfigKnobGuid      = CONFIG_KNOB_GUID;
      ConfigKnobName      = L"MyDeadBeefDelivery";
      ProfileDefaultSize  = sizeof (ConfigKnobData);
      ProfileDefaultValue = 0xDEADBEEFDEADBEEF;
      ProfileDefaultSize  = sizeof (ProfileDefaultValue);
      VariableData        = 0xBEEF7777BEEF7777;
      PpiStatus           = { .Ppi = &MockVariablePpi, .Status = EFI_SUCCESS };

    }
};

TEST_F(GetConfigKnobOverrideTest, VariableStorageSuccess) {
  EXPECT_CALL(RtServicesMock, gRT_GetVariable)
    .WillRepeatedly(Return(EFI_INVALID_PARAMETER));

  // TODO need mock of PPI services from mdemodule
  // temorarily can cvreate local mock?
  Status = GetConfigKnobOverride (&ConfigKnobGuid, NULL, (VOID *)&ConfigKnobData, ProfileDefaultSize);
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}