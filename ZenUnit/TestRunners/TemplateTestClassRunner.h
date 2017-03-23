#pragma once
#include <numeric>
#include "ZenUnit/TestRunners/TestClassRunner.h"
#include "ZenUnit/Results/TestResult.h"
#include "ZenUnit/Tests/Test.h"
#include "ZenUnit/Tests/NewDeleteTest.h"
#include "ZenUnit/Console/Console.h"
#include "ZenUnit/Utils/Iteration/MemberForEacherExtraArg.h"

namespace ZenUnit
{
   template<typename TestClassType>
   class TemplateTestClassRunner : public TestClassRunner
   {
      friend class TemplateTestClassRunnerTests;
   private:
      std::unique_ptr<const Console> _console;
      using TestsMemberForEacherExtraArgType = MemberForEacherExtraArg<
         std::vector<std::unique_ptr<Test>>,
         TemplateTestClassRunner,
         void (TemplateTestClassRunner::*)(const std::unique_ptr<Test>& test, TestClassResult*) const, TestClassResult*>;
      std::unique_ptr<const TestsMemberForEacherExtraArgType> _testsMemberForEacherExtraArg;
      const char* _testClassName;
      NewDeleteTest<TestClassType> _newDeleteTest;
      std::vector<std::unique_ptr<Test>> _tests;
      TestClassResult _testClassResult;
   public:
      TemplateTestClassRunner(const char* testClassName)
         : _console(new Console)
         , _testsMemberForEacherExtraArg(new TestsMemberForEacherExtraArgType)
         , _testClassName(testClassName)
         , _newDeleteTest(testClassName)
      {
         _tests = TestClassType::GetTests();
      }

      ~TemplateTestClassRunner()
      {
      }

      virtual const char* TestClassName() const override
      {
         return _testClassName;
      }

      virtual size_t NumberOfTestCases() const override
      {
         size_t totalNumberOfTestCases = std::accumulate(_tests.cbegin(), _tests.cend(), size_t(),
            [](size_t cumulativeNumberOfTestCases, const std::unique_ptr<Test>& test)
            {
               size_t numberOfTestCases = test->NumberOfTestCases();
               return cumulativeNumberOfTestCases + numberOfTestCases;
            });
         return totalNumberOfTestCases;
      }

      TestClassResult RunTests() override
      {
         PrintTestClassNameAndNumberOfTests();
         if (ConfirmNewabilityDeletabilityAndRegisterNXNTests(&_newDeleteTest, &_testClassResult))
         {
            _testsMemberForEacherExtraArg->ForEach(
               &_tests, this, &TemplateTestClassRunner::RunTest, &_testClassResult);
         }
         PrintTestClassResultLine(&_testClassResult);
         _console->WriteNewline();
         return _testClassResult;
      }
   private:
      virtual void PrintTestClassNameAndNumberOfTests() const
      {
         _console->WriteColor("@", Color::Green);
         _console->WriteColor(_testClassName, Color::Green);
         _console->WriteLine(String::Concat(
            " | ", _tests.size(), _tests.size() == 1 ? " named test" : " named tests"));
      }

      virtual bool ConfirmNewabilityDeletabilityAndRegisterNXNTests(
         Test* newDeleteTest, TestClassResult* outTestClassResult) const
      {
         std::vector<TestResult> newDeleteTestResult = newDeleteTest->Run();
         assert_true(newDeleteTestResult.size() == 1);
         outTestClassResult->AddTestResults(newDeleteTestResult);
         bool testClassTypeIsNewableAndDeletable = newDeleteTestResult[0].testOutcome == TestOutcome::Success;
         return testClassTypeIsNewableAndDeletable;
      }

      void RunTest(const std::unique_ptr<Test>& test, TestClassResult* outTestClassResult) const
      {
         _console->WriteColor("|", Color::Green);
         const char* testName = test->Name();
         _console->Write(testName);
         test->PrintPostTestNameMessage(_console.get());
         std::vector<TestResult> testResults = test->Run();
         test->PrintPostTestCompletionMessage(_console.get(), testResults[0]);
         outTestClassResult->AddTestResults(testResults);
      }

      virtual void PrintTestClassResultLine(const TestClassResult* testClassResult) const
      {
         testClassResult->PrintResultLine(_console.get());
      }
   };
}
