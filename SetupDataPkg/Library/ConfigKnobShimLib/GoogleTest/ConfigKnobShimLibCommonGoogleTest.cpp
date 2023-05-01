/** @file ConfigKnobShimLibCommonGoogleTest.cpp
  Google tests of the ConfigKnobShimLibCommon instance.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <gtest/gtest.h>
#include <GoogleTest/Library/MockUefiRuntimeServicesTableLib.h>
#include <GoogleTest/Library/MockPeiServicesLib.h>
#include <ConfigKnobShimmLibCommonGoogleTest.h>

extern "C" {
  #include <Uefi.h>
  #include <Library/BaseLib.h>
  #include <Library/DebugLib.h>
  // include the c file to be able to unit test static function
  #include "../ConfigKnobShimLibCommon.c"
}

#define CONFIG_KNOB_GUID  {0x52d39693, 0x4f64, 0x4ee6, {0x81, 0xde, 0x45, 0x89, 0x37, 0x72, 0x78, 0x55}}
MockUefiRuntimeServicesTableLib RtServicesMock;
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
      ConfigKnobName      = (CHAR16 *)L"MyDeadBeefDelivery";
      ProfileDefaultSize  = sizeof (ConfigKnobData);
    }
};

/**
 * todo - all these tests are testing, what happens when the getvariable call returns EFI_INVALID_PARAMETER
 * @ Oliver is there anything else in GetConfigKnobbOverride you wanted to test? that the data is not changed if the params are invalid?
 * if want them all, should break out into separate TEST_Fs
 * What if calling the function changed the params> Should start with a clean slate every time?
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

///////////////////////////////////////////////////////////////////////////////
class GetConfigKnobOverrideFromVariableStorageTest : public Test {
  protected:
    // MockPeiServicesLib PPIVariableServices; // EFI_PEI_READ_ONLY_VARIABLE2_PPI
    EFI_STATUS  Status;
    EFI_GUID    ConfigKnobGuid;
    CHAR16      *ConfigKnobName;
    UINT64      ProfileDefaultValue;
    UINTN       ProfileDefaultSize;
    UINT64      VariableData;
    PPI_STATUS  PpiStatus;
    UINT64      ConfigKnobData;

    void SetUp() override {
      ConfigKnobGuid      = CONFIG_KNOB_GUID;
      ConfigKnobName      = (CHAR16 *)L"MyDeadBeefDelivery";
      ProfileDefaultValue = 0xDEADBEEFDEADBEEF;
      ProfileDefaultSize  = sizeof (ProfileDefaultValue);
      VariableData        = 0xBEEF7777BEEF7777;
    }
};

TEST_F(GetConfigKnobOverrideFromVariableStorageTest, VariableStorageSuccess) {

  EXPECT_CALL(RtServicesMock, gRT_GetVariable)
    .WillOnce(Return(EFI_INVALID_PARAMETER));

  // // If doing Pei test, will need to mock pei services
  // ON_CALL(PPIVariableServices, PeiServiesLocatePpi())
  //   .WillByDefault(Return(PpiStatus));


  // TODO  function to determine which getvariable to use. based on unit test inf inclusion. 
  // put th eexpect call into the functions in dxe/pei specific files 
  
  // expect the first GetVariable call to get size
  //expectMockGetVariableNotFound();

  // expect the second getVariable call to update data
  //expectMockGetVariableRet();


  Status = GetConfigKnobOverride (&ConfigKnobGuid, ConfigKnobName, (VOID *)&ConfigKnobData, ProfileDefaultSize);
  ASSERT_EQ (Status, EFI_SUCCESS);
  ASSERT_EQ(VariableData, ConfigKnobData);
}



int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}