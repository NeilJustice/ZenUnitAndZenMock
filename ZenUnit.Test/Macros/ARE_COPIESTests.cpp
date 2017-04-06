#include "pch.h"

namespace ZenUnit
{
   TESTS(ARE_COPIESTests)
   SPEC(ObjectsHaveSameAddress_Throws)
   SPEC(ObjectsHaveSameAddress_Throws_MessagesTestCase)
   SPEC(ObjectsHaveDifferentAddresses_ObjectsAreNotEqual_Throws)
   SPEC(ObjectsHaveDifferentAddresses_ObjectsAreNotEqual_Throws_UserTypeTestCase)
   SPEC(ObjectsHaveDifferentAddresses_ObjectsAreNotEqual_Throws_MessagesTestCase)
   SPEC(ObjectsHaveDifferentAddresses_ObjectsAreEqual_DoesNotThrow)
   SPECEND

   TEST(ObjectsHaveSameAddress_Throws)
   {
      const int x = 0;
      const string expectedAreNotSameWhatText = MakeExpectedARE_NOT_SAME_WhatText(&x, &x);
      const string expectedWhat = R"(
  Failed: ARE_COPIES(x, x)
Expected: 0
  Actual: 0
)" + expectedAreNotSameWhatText + R"(
File.cpp(1)
File.cpp(1))";
      THROWS(ARE_COPIES(x, x), Anomaly, expectedWhat);
   }

   TEST(ObjectsHaveSameAddress_Throws_MessagesTestCase)
   {
      const int x = 0;
      const string messageA = "A", messageB = "B";
      const string expectedAreNotSameWhatText = MakeExpectedARE_NOT_SAME_WhatText(&x, &x);
      const string expectedWhat = R"(
  Failed: ARE_COPIES(x, x, messageA, messageB)
Expected: 0
  Actual: 0
)" + expectedAreNotSameWhatText + R"(
 Message: "A", "B"
File.cpp(1)
File.cpp(1))";
      THROWS(ARE_COPIES(x, x, messageA, messageB), Anomaly, expectedWhat);
   }

   static string MakeExpectedARE_NOT_SAME_WhatText(const void* notExpectedAddress, const void* actualAddress)
   {
      ostringstream expectedWhatPatternBuilder;
      expectedWhatPatternBuilder << 
         " Because: ARE_NOT_SAME(expectedObject, actualObject) failed\n" <<
"Expected: Not ";
#ifdef __linux__
     expectedWhatPatternBuilder << notExpectedAddress << R"(
  Actual:     )" << actualAddress;
#elif _WIN32
      expectedWhatPatternBuilder << "0x" << notExpectedAddress << R"(
  Actual:     0x)" << actualAddress;
#endif
      const string expectedWhatPattern = expectedWhatPatternBuilder.str();
      return expectedWhatPattern;
   }

   TEST(ObjectsHaveDifferentAddresses_ObjectsAreNotEqual_Throws)
   {
      const int x = 1;
      int y = 2;
      THROWS(ARE_COPIES(x, y), Anomaly, R"(
  Failed: ARE_COPIES(x, y)
Expected: 1
  Actual: 2
 Because: ARE_EQUAL(expectedObject, actualObject) failed
Expected: 1
  Actual: 2
File.cpp(1)
File.cpp(1))");
   }

   TEST(ObjectsHaveDifferentAddresses_ObjectsAreNotEqual_Throws_UserTypeTestCase)
   {
      UserType x(1);
      const UserType y(2);
      THROWS(ARE_COPIES(x, y), Anomaly, R"(
  Failed: ARE_COPIES(x, y)
Expected: UserType@1
  Actual: UserType@2
 Because: ARE_EQUAL(expectedObject, actualObject) failed
Expected: UserType@1
  Actual: UserType@2
File.cpp(1)
File.cpp(1))");
   }

   TEST(ObjectsHaveDifferentAddresses_ObjectsAreNotEqual_Throws_MessagesTestCase)
   {
      UserType x(1);
      UserType y(2);
      const string messageA = "A", messageB = "B";
      THROWS(ARE_COPIES(x, y, messageA, messageB), Anomaly, R"(
  Failed: ARE_COPIES(x, y, messageA, messageB)
Expected: UserType@1
  Actual: UserType@2
 Because: ARE_EQUAL(expectedObject, actualObject) failed
Expected: UserType@1
  Actual: UserType@2
 Message: "A", "B"
File.cpp(1)
File.cpp(1))");
   }

   TEST(ObjectsHaveDifferentAddresses_ObjectsAreEqual_DoesNotThrow)
   {
      int x = 1;
      int y = 1;
      ARE_COPIES(x, y);
   }

   }; RUN(ARE_COPIESTests)
}
