#include "pch.h"
#include "ZenUnit/Macros/NOTHROWS.h"

namespace ZenUnit
{
   TESTS(NOTHROWSTests)
   SPEC(LambdaCallThrows_Throws)
   SPEC(LambdaCallThrows_Throws_MessagesTestCase)
   SPEC(FunctionCallThrows_Throws)
   SPEC(LambdaCallDoesNotThrow_DoesNotThrow)
   SPEC(FunctionCallDoesNotThrow_DoesNotThrow)
   SPECEND

   TEST(LambdaCallThrows_Throws)
   {
      THROWS(NOTHROWS([] { throw runtime_error("what text"); }()), Anomaly, R"(
  Failed: NOTHROWS([] { throw runtime_error("what text"); }())
Expected: No exception thrown
  Actual: std::runtime_error thrown
  what(): "what text"
File.cpp(1))");
   }

   TEST(LambdaCallThrows_Throws_MessagesTestCase)
   {
      const string MessageA = "A", MessageB = "B";
      THROWS(NOTHROWS([] { throw runtime_error(""); }(), MessageA, MessageB), Anomaly, R"(
  Failed: NOTHROWS([] { throw runtime_error(""); }(), MessageA, MessageB)
Expected: No exception thrown
  Actual: std::runtime_error thrown
  what(): ""
 Message: "A", "B"
File.cpp(1))");
   }

   void Throw()
   {
      throw invalid_argument("what text");
   }

   TEST(FunctionCallThrows_Throws)
   {
      THROWS(NOTHROWS(Throw()), Anomaly, R"(
  Failed: NOTHROWS(Throw())
Expected: No exception thrown
  Actual: std::invalid_argument thrown
  what(): "what text"
File.cpp(1))");
   }

   TEST(LambdaCallDoesNotThrow_DoesNotThrow)
   {
      NOTHROWS([]{}());
   }

   void NoThrow() {}

   TEST(FunctionCallDoesNotThrow_DoesNotThrow)
   {
      NOTHROWS(NoThrow());
   }

   }; RUN(NOTHROWSTests)
}
