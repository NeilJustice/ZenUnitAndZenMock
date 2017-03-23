#pragma once
#include "ZenUnit/Results/TestClassResult.h"

namespace ZenUnit
{
   class TestClassRunner
   {
   public:
      virtual const char* TestClassName() const = 0;
      virtual size_t NumberOfTestCases() const = 0;
      virtual TestClassResult RunTests() = 0;
      virtual ~TestClassRunner() {}

      friend bool operator<(
         const std::unique_ptr<TestClassRunner>& leftTestClassRunner,
         const std::unique_ptr<TestClassRunner>& rightTestClassRunner);
   };
}
