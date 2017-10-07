#include "pch.h"
#include "ZenUnit/TestRunners/MultiTestClassRunner.h"
#include "ZenUnit/TestRunners/TestClassRunner.h"
#include "ZenUnit/Utils/Iteration/Transformer.h"
#include "ZenUnit/Utils/Sorter.h"
#include "ZenUnit/Utils/Time/Watch.h"

namespace ZenUnit
{
   MultiTestClassRunner::MultiTestClassRunner()
      : _sorter(new Sorter<std::vector<std::unique_ptr<TestClassRunner>>>)
      , _transformer(new Transformer<std::unique_ptr<TestClassRunner>, TestClassResult>)
      , _watch(new Watch)
   {
   }

   MultiTestClassRunner::~MultiTestClassRunner() = default;

   size_t MultiTestClassRunner::NumberOfTestClasses() const
   {
      const size_t numberOfTestClasses = _testClassRunners.size();
      return numberOfTestClasses;
   }

   size_t MultiTestClassRunner::NumberOfTestCases() const
   {
      const size_t numberOfTestCases = std::accumulate(_testClassRunners.cbegin(), _testClassRunners.cend(),
         size_t(0), [](size_t cumulativeNumberOfTestCases, const std::unique_ptr<TestClassRunner>& testClassRunner)
      {
         return cumulativeNumberOfTestCases + testClassRunner->NumberOfTestCases();
      });
      return numberOfTestCases;
   }

   void MultiTestClassRunner::AddTestClassRunner(TestClassRunner* testClassRunner)
   {
      _testClassRunners.emplace_back(testClassRunner);
   }

   std::vector<TestClassResult> MultiTestClassRunner::RunTestClasses(ZenUnitArgs& zenUnitArgs)
   {
      _testClassResults.resize(_testClassRunners.size());
      if (zenUnitArgs.random)
      {
         if (!zenUnitArgs.randomseedsetbyuser)
         {
            zenUnitArgs.randomseed = _watch->SecondsSince1970CastToUnsignedShort();
         }
         _transformer->RandomTransform(
            &_testClassRunners, &_testClassResults, &MultiTestClassRunner::RunTestClassRunner, zenUnitArgs.randomseed);
      }
      else
      {
         _sorter->Sort(&_testClassRunners); // Sorts test class runners by test class name
         _transformer->Transform(
            &_testClassRunners, &_testClassResults, &MultiTestClassRunner::RunTestClassRunner);
      }
      return std::move(_testClassResults);
   }

   TestClassResult MultiTestClassRunner::RunTestClassRunner(
      const std::unique_ptr<TestClassRunner>& testClassRunner)
   {
      TestClassResult testClassResult = testClassRunner->RunTests();
      return testClassResult;
   }
}
