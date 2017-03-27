#include "pch.h"
#include "ZenUnit/Utils/assert_true.h"

namespace ZenUnit
{
   TESTS(assert_trueTests)
   SPEC(assert_true_IsTrue_DoesNothing)
   SPEC(assert_true_IsFalse_FromOperatorParentheses_ThrowsLogicError)
   SPEC(assert_true_IsFalse_FromFunction_ThrowsLogicError)
   SPECEND

   TEST(assert_true_IsTrue_DoesNothing)
   {
      assert_true(true);
      const bool trueBool = true;
      assert_true(trueBool);
      assert_true(1 == 1);
   }

   void f()
   {
      try
      {
         assert_true(1 == 0);
      }
      catch (const logic_error& e)
      {
         REGEX_MATCHES(R"(assert_true\(1 == 0\) failed in f\(\)
.*?File.cpp\(1\))", e.what());
      }
   }

   TEST(assert_true_IsFalse_FromOperatorParentheses_ThrowsLogicError)
   {
      string expectedOperatorParenthesesName;
   #ifdef __linux__
      expectedOperatorParenthesesName = R"(operator\(\)\(\))";
   #elif _WIN32
      expectedOperatorParenthesesName = R"(operator \(\)\(\))";
   #endif
      try
      {
         struct X
         {
            void operator()() const
            {
               assert_true(false);
            }
         };
         X x;
         x();
         FAIL("assert_true did not throw");
      }
      catch (const logic_error& e)
      {
         const string expectedWhatPattern = String::Concat(
            "assert_true\\(false\\) failed in ", expectedOperatorParenthesesName, '\n',
            ".*File.cpp\\(1\\)");
         const char* const what = e.what();
         REGEX_MATCHES(expectedWhatPattern, what);
      }
   }

   TEST(assert_true_IsFalse_FromFunction_ThrowsLogicError)
   {
      f();
   }

   }; RUN(assert_trueTests)
}
