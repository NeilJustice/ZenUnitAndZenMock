#include "pch.h"
#include "ZenUnit/Macros/IS_EMPTY.h"

namespace ZenUnit
{
   TESTS(IS_EMPTYTests)
   SPEC(ContainerIsEmpty_DoesNotThrow)
   SPEC(VectorIsNotEmpty_Throws)
   SPEC(StringIsNotEmpty_Throws_MessagesTestCase)
   SPECEND

   TEST(ContainerIsEmpty_DoesNotThrow)
   {
      IS_EMPTY(string());
      IS_EMPTY(vector<int>());
      IS_EMPTY((unordered_map<int, string>()));
      IS_EMPTY(unordered_set<int>());
   }

   TEST(VectorIsNotEmpty_Throws)
   {
      THROWS(IS_EMPTY(vector<int>(1)), Anomaly, R"(
  Failed: IS_EMPTY(vector<int>(1))
Expected: empty() == true
  Actual: empty() == false (size() == 1)
File.cpp(1))");
   }

   TEST(StringIsNotEmpty_Throws_MessagesTestCase)
   {
      string messageA = "A", messageB = "B";
      THROWS(IS_EMPTY(string(10, 'a'), messageA, messageB), Anomaly, R"(
  Failed: IS_EMPTY(string(10, 'a'), messageA, messageB)
Expected: empty() == true
  Actual: empty() == false (size() == 10)
 Message: "A", "B"
File.cpp(1))");
   }

   }; RUN(IS_EMPTYTests)
}
