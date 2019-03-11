#include "pch.h"
#include "ZenUnitLibraryTests/Console/Mock/ConsoleMock.h"
#include "ZenUnitLibraryTests/Tests/Mock/TestMock.h"
#include "ZenUnitTestUtils/Equalizers/TestResultEqualizer.h"

namespace ZenUnit
{
   class TestingTestClass_SpecSectionTestNXNTests
   {
   public:
      static bool didCallTestNXNPmfToTest;
      static unique_ptr<Test> TestNXNPmfToTestReturnValue;

      static const unique_ptr<Test>* TestFromTestNXNPmfToken(const ZenUnit::PmfToken*)
      {
         didCallTestNXNPmfToTest = true;
         return &TestNXNPmfToTestReturnValue;
      }

      void TestFunction(int) {}
      virtual ~TestingTestClass_SpecSectionTestNXNTests() = default;
   };

   bool TestingTestClass_SpecSectionTestNXNTests::didCallTestNXNPmfToTest;
   unique_ptr<Test> TestingTestClass_SpecSectionTestNXNTests::TestNXNPmfToTestReturnValue;

   TESTS(SpecSectionTestNXNTests)
   AFACT(ThreeArgConstructor_SetsTestName_SetsTestNXNPmf)
   FACTS(NumberOfTestCases_GetsTestFromAddress_ReturnsTestNumberOfTestCases)
   AFACT(WritePostTestNameMessage_WritesEllipsis)
   AFACT(RunTest_GetsTestFromAddress_RunsTest_ReturnsTestResults)
   AFACT(PmfTokenToTest_ReturnsTestClassTypeTestNXNPmfToTestReturnValue);
   AFACT(TestFunction_CodeCoverage)
   EVIDENCE

   class SpecSectionTestNXNSelfMocked : public Zen::Mock<SpecSectionTestNXN<TestingTestClass_SpecSectionTestNXNTests>>
   {
   public:
      SpecSectionTestNXNSelfMocked() noexcept
         : Zen::Mock<SpecSectionTestNXN<TestingTestClass_SpecSectionTestNXNTests>>("", "", nullptr) {}
      ZENMOCK_NONVOID0_CONST(const unique_ptr<Test>*, PmfTokenToTest)
   };

   unique_ptr<SpecSectionTestNXN<TestingTestClass_SpecSectionTestNXNTests>> _specSectionTestNXN;
   unique_ptr<SpecSectionTestNXNSelfMocked> _specSectionTestNXNSelfMocked;

   STARTUP
   {
      _specSectionTestNXN = make_unique<SpecSectionTestNXN<TestingTestClass_SpecSectionTestNXNTests>>("", "", nullptr);
      _specSectionTestNXNSelfMocked = make_unique<SpecSectionTestNXNSelfMocked>();
   }

   TEST(ThreeArgConstructor_SetsTestName_SetsTestNXNPmf)
   {
      const string testClassName = Random<string>();
      const string testName = Random<string>();
      //
      const SpecSectionTestNXN<TestingTestClass_SpecSectionTestNXNTests> specifiedTestNXN(
         testClassName.c_str(), testName.c_str(), PmfToken::Instantiate<decltype(
            &TestingTestClass_SpecSectionTestNXNTests::TestFunction),
         &TestingTestClass_SpecSectionTestNXNTests::TestFunction>());
      //
      ARE_EQUAL(testName, specifiedTestNXN.Name());
      ARE_EQUAL("TESTS(" + testClassName + ")\nTEST(" + testName + ")", specifiedTestNXN.FullName());
      ARE_EQUAL("(0)", specifiedTestNXN.FileLineString());
      const PmfToken* const expectedTestNXNPmfToken = ZenUnit::PmfToken::Instantiate<
         decltype(&TestingTestClass_SpecSectionTestNXNTests::TestFunction),
         &TestingTestClass_SpecSectionTestNXNTests::TestFunction>();
      ARE_EQUAL(expectedTestNXNPmfToken, specifiedTestNXN._testNXNPmfToken);
   }

   TEST1X1(NumberOfTestCases_GetsTestFromAddress_ReturnsTestNumberOfTestCases,
      size_t testNumberOfTestCases,
      size_t(1),
      size_t(2))
   {
      TestMock* testMock = new TestMock;
      testMock->NumberOfTestCasesMock.Return(testNumberOfTestCases);
      const unique_ptr<Test> testMockUniquePtr(testMock);
      _specSectionTestNXNSelfMocked->PmfTokenToTestMock.Return(&testMockUniquePtr);
      //
      const size_t numberOfTestCases = _specSectionTestNXNSelfMocked->NumberOfTestCases();
      //
      ZENMOCK(_specSectionTestNXNSelfMocked->PmfTokenToTestMock.CalledOnce());
      ZENMOCK(testMock->NumberOfTestCasesMock.CalledOnce());
      ARE_EQUAL(testNumberOfTestCases, numberOfTestCases);
   }

   TEST(WritePostTestNameMessage_WritesEllipsis)
   {
      ConsoleMock consoleMock;
      consoleMock.WriteLineMock.Expect();
      //
      _specSectionTestNXN->WritePostTestNameMessage(&consoleMock);
      //
      ZENMOCK(consoleMock.WriteLineMock.CalledOnceWith("..."));
   }

   TEST(RunTest_GetsTestFromAddress_RunsTest_ReturnsTestResults)
   {
      TestMock* const testMock = new TestMock;
      const vector<TestResult> testTestResults{ TestResult::TestingNonDefault() };
      testMock->RunTestMock.Return(testTestResults);
      const unique_ptr<Test> testMockUniquePtr(testMock);
      _specSectionTestNXNSelfMocked->PmfTokenToTestMock.Return(&testMockUniquePtr);
      //
      const vector<TestResult> testResults = _specSectionTestNXNSelfMocked->RunTest();
      //
      ZENMOCK(_specSectionTestNXNSelfMocked->PmfTokenToTestMock.CalledOnce());
      ZENMOCK(testMock->RunTestMock.CalledOnce());
      VECTORS_EQUAL(testTestResults, testResults);
   }

   TEST(PmfTokenToTest_ReturnsTestClassTypeTestNXNPmfToTestReturnValue)
   {
      TestingTestClass_SpecSectionTestNXNTests::didCallTestNXNPmfToTest = false;
      //
      const unique_ptr<Test>* testNXN = _specSectionTestNXN->PmfTokenToTest();
      //
      IS_TRUE(TestingTestClass_SpecSectionTestNXNTests::didCallTestNXNPmfToTest);
      ARE_EQUAL(testNXN, &TestingTestClass_SpecSectionTestNXNTests::TestNXNPmfToTestReturnValue);
   }

   TEST(TestFunction_CodeCoverage)
   {
      TestingTestClass_SpecSectionTestNXNTests t;
      t.TestFunction(0);
   }

   RUN_TESTS(SpecSectionTestNXNTests)
}
