#pragma once
#include <iostream>
#include "ZenUnit/Tests/Test.h"

namespace ZenUnit
{
   template<typename TestClassType>
   class NormalTest : public Test
   {
      friend class NormalTestTests;
   private:
      std::unique_ptr<TestClassType> _testClass;
      void (TestClassType::*_testMemberFunction)();
   public:
      template<typename TestMemberFunctionType>
      NormalTest(const char* testClassName, const char* testName, TestMemberFunctionType testMemberFunction)
         : Test(testClassName, testName)
         , _testMemberFunction(testMemberFunction)
      {
      }

      virtual size_t NumberOfTestCases() const override
      {
         return 1;
      }

      virtual void PrintPostTestNameMessage(const Console* console) const override
      {
         console->Write(" -> ");
      }

      virtual void PrintPostTestCompletionMessage(
         const Console* console, const TestResult& testResult) const override
      {
         testResult.PrintTestOutcome(console);
      }

      virtual std::vector<TestResult> Run() override
      {
         const TestResult testResult = RunTestCase();
         return { testResult };
      }

      virtual void NewTestClass() override
      {
         assert_true(_testClass == nullptr);
         _testClass.reset(new TestClassType);
      }

      virtual void Startup() override
      {
         _testClass->Startup();
      }

      virtual void TestBody() override
      {
         (_testClass.get()->*_testMemberFunction)();
      }

      virtual void Cleanup() override
      {
         _testClass->Cleanup();
      }

      virtual void DeleteTestClass() override
      {
         _testClass.reset();
      }
   };
}
