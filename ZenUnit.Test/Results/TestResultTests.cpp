#include "pch.h"
#include "ZenUnit/Results/TestResult.h"
#include "Console/Mock/ConsoleMock.h"
#include "Results/Mock/TestFailureNumbererMock.h"

namespace ZenUnit
{
   TESTS(TestResultTests)
   SPEC(DefaultConstructor_SetsFieldsTo0)
   SPECX(ConstructorFail_ReturnsExpectedTestResult)
   SPECX(StartupFail_ReturnsExpectedTestResult)
   SPEC(CtorDtorSuccess_ReturnsExpectedTestResult);
   SPECX(SixArgConstructor_SetsFields)
   SPECX(PrintTestOutcome_PrintsOutcome)
   SPEC(PrintTestOutcome_InvalidTestOutcome_TriggersAssertion)
   SPEC(PrintIfFailure_Success_PrintsNothing)
   SPECX(PrintIfFailure_Anomaly_PrintsExpected)
   SPECX(PrintIfFailure_Exception_PrintsExpected)
   SPEC(PrintIfFailure_SuccessButMissedDeadline_PrintsExpected)
   SPEC(PrintIfFailure_InvalidOutcome_Throws)
   SPECX(WriteTestCaseNumberIfAny_WritesToConsoleTestCaseIndexPlus1IfTestCaseIndexNotNegative1)
   SPEC(ZenUnitEqualizer_ThrowsIfAnyFieldNotEqual)
   SPECEND

   TestResult _testResult;
   const FullName FullNameValue = FullName("ClassName", "TestClassName");
   CallResult ConstructorCallResult;
   CallResult StartupCallResult;
   CallResult DestructorCallResult;
   ConsoleMock _consoleMock;
   TestFailureNumbererMock _testFailureNumbererMock;
   const long long ExpectedMilliseconds = 1 + 2 + 3 + 4 + 5;

   ZENMOCK_NONVOID0_STATIC(ZenUnitArgs, ZenUnit::TestRunner, GetArgs)

   struct TestResult_WriteTestCaseNumberIfAnyMocked : public Zen::Mock<TestResult>
   {
      ZENMOCK_VOID2_CONST(WriteTestCaseNumberIfAny, const Console*, int)
   } _testResult_WriteTestCaseNumberIfAnyMocked;

   STARTUP
   {
      ConstructorCallResult = CallResult(TestPhase::Constructor);
      ConstructorCallResult.milliseconds = 1;
      StartupCallResult = CallResult(TestPhase::Startup);
      StartupCallResult.milliseconds = 2;
      DestructorCallResult = CallResult(TestPhase::Destructor);
      DestructorCallResult.milliseconds = 3;
      _testResult.fullName = FullNameValue;
   }

   TEST(DefaultConstructor_SetsFieldsTo0)
   {
      TestResult defaultTestResult;
      TestResult expectedDefaultTestResult;
      expectedDefaultTestResult.fullName = FullName();
      expectedDefaultTestResult.constructorCallResult = CallResult();
      expectedDefaultTestResult.startupCallResult = CallResult();
      expectedDefaultTestResult.testBodyCallResult = CallResult();
      expectedDefaultTestResult.cleanupCallResult = CallResult();
      expectedDefaultTestResult.destructorCallResult = CallResult();
      expectedDefaultTestResult.responsibleCallResultField = nullptr;
      expectedDefaultTestResult.testOutcome = TestOutcome::Unset;
      expectedDefaultTestResult.testCaseIndex = -1;
      expectedDefaultTestResult.milliseconds = 0;
      ARE_EQUAL(expectedDefaultTestResult, defaultTestResult);
   }

   TEST2X2(ConstructorFail_ReturnsExpectedTestResult,
      TestOutcome constructorOutcome, TestOutcome expectedTestResultOutcome,
      TestOutcome::Anomaly, TestOutcome::Anomaly,
      TestOutcome::Exception, TestOutcome::Exception)
   {
      ConstructorCallResult.testOutcome = constructorOutcome;
      //
      TestResult constructorFailTestResult = TestResult::ConstructorFail(FullNameValue, ConstructorCallResult);
      //
      TestResult expectedTestResult;
      expectedTestResult.fullName = FullNameValue;
      expectedTestResult.constructorCallResult = ConstructorCallResult;
      expectedTestResult.responsibleCallResultField = &TestResult::constructorCallResult;
      expectedTestResult.testOutcome = expectedTestResultOutcome;
      expectedTestResult.milliseconds = ConstructorCallResult.milliseconds;
      ARE_EQUAL(expectedTestResult, constructorFailTestResult);
   }

   TEST2X2(StartupFail_ReturnsExpectedTestResult,
      TestOutcome startupOutcome, TestOutcome expectedTestResultOutcome,
      TestOutcome::Anomaly, TestOutcome::Anomaly,
      TestOutcome::Exception, TestOutcome::Exception)
   {
      StartupCallResult.testOutcome = startupOutcome;
      //
      TestResult startupFailTestResult = TestResult::StartupFail(
         FullNameValue, ConstructorCallResult, StartupCallResult, DestructorCallResult);
      //
      TestResult expectedTestResult;
      expectedTestResult.fullName = FullNameValue;
      expectedTestResult.constructorCallResult = ConstructorCallResult;
      expectedTestResult.startupCallResult = StartupCallResult;
      expectedTestResult.destructorCallResult = DestructorCallResult;
      expectedTestResult.responsibleCallResultField = &TestResult::startupCallResult;
      expectedTestResult.testOutcome = expectedTestResultOutcome;
      expectedTestResult.milliseconds =
         ConstructorCallResult.milliseconds + StartupCallResult.milliseconds + DestructorCallResult.milliseconds;
      ARE_EQUAL(expectedTestResult, startupFailTestResult);
   }

   TEST(CtorDtorSuccess_ReturnsExpectedTestResult)
   {
      CallResult constructorCallResult(TestPhase::Constructor);
      constructorCallResult.milliseconds = 10;
      CallResult destructorCallResult(TestPhase::Destructor);
      destructorCallResult.milliseconds = 20;
      //
      TestResult testResult = TestResult::CtorDtorSuccess(FullNameValue, constructorCallResult, destructorCallResult);
      //
      TestResult expectedTestResult;
      expectedTestResult.fullName = FullNameValue;
      expectedTestResult.testOutcome = TestOutcome::Success;
      expectedTestResult.constructorCallResult = constructorCallResult;
      expectedTestResult.destructorCallResult = destructorCallResult;
      expectedTestResult.milliseconds = constructorCallResult.milliseconds + destructorCallResult.milliseconds;
      expectedTestResult.responsibleCallResultField = nullptr;
      ARE_EQUAL(expectedTestResult, testResult);
   }

   TEST6X6(SixArgConstructor_SetsFields,
      TestOutcome testBodyOutcome, TestOutcome cleanupOutcome, long long maxtestmilliseconds, int relativeMilliseconds, TestOutcome expectedOverallOutcome, CallResult TestResult::* expectedResponsibleCallResultField,
      TestOutcome::Success, TestOutcome::Success, 0ll, 0, TestOutcome::Success, nullptr,
      TestOutcome::Success, TestOutcome::Success, 0ll, 1, TestOutcome::Success, nullptr,
      TestOutcome::Success, TestOutcome::Success, ExpectedMilliseconds, -1, TestOutcome::Success, nullptr,
      TestOutcome::Success, TestOutcome::Success, ExpectedMilliseconds, 0, TestOutcome::Success, nullptr,
      TestOutcome::Success, TestOutcome::Success, ExpectedMilliseconds, 1, TestOutcome::SuccessButMissedDeadline, nullptr,
      TestOutcome::Success, TestOutcome::Success, ExpectedMilliseconds, 2, TestOutcome::SuccessButMissedDeadline, nullptr,
      TestOutcome::Exception, TestOutcome::Success, ExpectedMilliseconds, 1, TestOutcome::Exception, &TestResult::testBodyCallResult,
      TestOutcome::Exception, TestOutcome::Exception, ExpectedMilliseconds, 1, TestOutcome::Exception, &TestResult::testBodyCallResult,
      TestOutcome::Exception, TestOutcome::Anomaly, ExpectedMilliseconds, 1, TestOutcome::Exception, &TestResult::testBodyCallResult,
      TestOutcome::Success, TestOutcome::Exception, ExpectedMilliseconds, 1, TestOutcome::Exception, &TestResult::cleanupCallResult,
      TestOutcome::Anomaly, TestOutcome::Exception, ExpectedMilliseconds, 1, TestOutcome::Exception, &TestResult::cleanupCallResult,
      TestOutcome::Anomaly, TestOutcome::Success, ExpectedMilliseconds, 1, TestOutcome::Anomaly, &TestResult::testBodyCallResult,
      TestOutcome::Success, TestOutcome::Anomaly, ExpectedMilliseconds, 1, TestOutcome::Anomaly, &TestResult::cleanupCallResult,
      TestOutcome::Anomaly, TestOutcome::Anomaly, ExpectedMilliseconds, 1, TestOutcome::Anomaly, &TestResult::testBodyCallResult)
   {
      CallResult testBodyCallResult(TestPhase::TestBody);
      testBodyCallResult.testOutcome = testBodyOutcome;
      testBodyCallResult.milliseconds = 4ll;
      CallResult cleanupCallResult(TestPhase::Cleanup);
      cleanupCallResult.testOutcome = cleanupOutcome;
      cleanupCallResult.milliseconds = static_cast<long long>(5 + relativeMilliseconds);

      ZenUnitArgs zenUnitArgs;
      zenUnitArgs.maxtestmilliseconds = maxtestmilliseconds;
      if (expectedOverallOutcome == TestOutcome::Success ||
          expectedOverallOutcome == TestOutcome::SuccessButMissedDeadline)
      {
         GetArgs_ZenMock.ExpectAndReturn(zenUnitArgs);
      }
      function<ZenUnitArgs()> boundMockGetArgs = ZENBIND0(GetArgs_ZenMock);
      //
      TestResult testResult(
         FullNameValue,
         ConstructorCallResult,
         StartupCallResult,
         testBodyCallResult,
         cleanupCallResult,
         DestructorCallResult,
         boundMockGetArgs);
      //
      if (expectedOverallOutcome == TestOutcome::Success ||
          expectedOverallOutcome == TestOutcome::SuccessButMissedDeadline)
      {
         GetArgs_ZenMock.AssertCalledOnce();
      }
      TestResult expectedTestResult;
      expectedTestResult.fullName = FullNameValue;
      expectedTestResult.constructorCallResult = ConstructorCallResult;
      expectedTestResult.startupCallResult = StartupCallResult;
      expectedTestResult.testBodyCallResult = testBodyCallResult;
      expectedTestResult.cleanupCallResult = cleanupCallResult;
      expectedTestResult.destructorCallResult = DestructorCallResult;
      expectedTestResult.responsibleCallResultField = expectedResponsibleCallResultField;
      expectedTestResult.testOutcome = expectedOverallOutcome;
      expectedTestResult.testCaseIndex = -1;
      expectedTestResult.milliseconds = ExpectedMilliseconds + relativeMilliseconds;
      ARE_EQUAL(expectedTestResult, testResult);
   }

   TEST3X3(PrintTestOutcome_PrintsOutcome,
      TestOutcome testOutcome, string expectedTestCaseResultConsoleWrite, Color expectedColor,
      TestOutcome::Success, "OK", Color::White,
      TestOutcome::Anomaly, "Anomaly", Color::Red,
      TestOutcome::Exception, "Exception", Color::Red,
      TestOutcome::SuccessButMissedDeadline, "SuccessButMissedDeadline", Color::Red)
   {
      _consoleMock.WriteLineColorMock.Expect();
      _testResult.testOutcome = testOutcome;
      //
      _testResult.PrintTestOutcome(&_consoleMock);
      //
      ZEN(_consoleMock.WriteLineColorMock.AssertCalledOnceWith(expectedTestCaseResultConsoleWrite, expectedColor));
   }

   TEST(PrintTestOutcome_InvalidTestOutcome_TriggersAssertion)
   {
      ConsoleMock consoleMock;
      _testResult.testOutcome = TestOutcome::Unset;
      //
      THROWS(_testResult.PrintTestOutcome(&consoleMock), logic_error,
         R"(assert_true(testOutcome == TestOutcome::SuccessButMissedDeadline) failed in PrintTestOutcome()
File.cpp(1))");
   }

   TEST(PrintIfFailure_Success_PrintsNothing)
   {
      _testResult.testOutcome = TestOutcome::Success;
      _testResult.PrintIfFailure(&_consoleMock, &_testFailureNumbererMock);
   }

   TEST3X3(PrintIfFailure_Anomaly_PrintsExpected,
      CallResult TestResult::* expectedResponsibleCallResultField, TestPhase testPhase, string expectedResponsibleTestPhaseSuffix,
      &TestResult::constructorCallResult, TestPhase::Constructor, " in test class constructor",
      &TestResult::startupCallResult, TestPhase::Startup, " in STARTUP",
      &TestResult::testBodyCallResult, TestPhase::TestBody, "",
      &TestResult::cleanupCallResult, TestPhase::Cleanup, " in CLEANUP")
   {
      _testResult_WriteTestCaseNumberIfAnyMocked.fullName.testClassTestNameLines = "TestClassTestNameLines";
      _testResult_WriteTestCaseNumberIfAnyMocked.testOutcome = TestOutcome::Anomaly;

      const string AnomalyWhy = "AnomalyWhy";
      (_testResult_WriteTestCaseNumberIfAnyMocked.*expectedResponsibleCallResultField).anomaly.why = AnomalyWhy;
      (_testResult_WriteTestCaseNumberIfAnyMocked.*expectedResponsibleCallResultField).testPhase = testPhase;
      _testResult_WriteTestCaseNumberIfAnyMocked.responsibleCallResultField = expectedResponsibleCallResultField;

      const string TestFailureNumber = "<10>";
      _testFailureNumbererMock.NextMock.ExpectAndReturn(TestFailureNumber);

      _testResult_WriteTestCaseNumberIfAnyMocked.testCaseIndex = 1;
      _testResult_WriteTestCaseNumberIfAnyMocked.WriteTestCaseNumberIfAnyMock.Expect();

      _consoleMock.WriteMock.Expect();
      _consoleMock.WriteLineMock.Expect();
      _consoleMock.WriteNewlineMock.Expect();
      //
      _testResult_WriteTestCaseNumberIfAnyMocked.PrintIfFailure(&_consoleMock, &_testFailureNumbererMock);
      //
      ZEN(_testFailureNumbererMock.NextMock.AssertCalledOnce());
      ZEN(_testResult_WriteTestCaseNumberIfAnyMocked.WriteTestCaseNumberIfAnyMock.
         AssertCalledOnceWith(&_consoleMock, _testResult_WriteTestCaseNumberIfAnyMocked.testCaseIndex));
      ZEN(_consoleMock.WriteMock.AssertCalls(
      {
         _testResult_WriteTestCaseNumberIfAnyMocked.fullName.testClassTestNameLines,
         expectedResponsibleTestPhaseSuffix
      }));
      ZEN(_consoleMock.WriteLineMock.AssertCalls(
      {
         TestFailureNumber,
         AnomalyWhy
      }));
      ZEN(_consoleMock.WriteNewlineMock.AssertCalledOnce());
   }

   TEST3X3(PrintIfFailure_Exception_PrintsExpected,
      CallResult TestResult::* expectedResponsibleCallResultField, TestPhase testPhase, string expectedTestPhaseSuffix,
      &TestResult::constructorCallResult, TestPhase::Constructor, " in test class constructor",
      &TestResult::startupCallResult, TestPhase::Startup, " in STARTUP",
      &TestResult::testBodyCallResult, TestPhase::TestBody, "",
      &TestResult::cleanupCallResult, TestPhase::Cleanup, " in CLEANUP")
   {
      _testResult_WriteTestCaseNumberIfAnyMocked.fullName.testClassTestNameLines = "TestClassTestNameLines";
      _testResult_WriteTestCaseNumberIfAnyMocked.testOutcome = TestOutcome::Exception;

      const string ExceptionTypeName = "ExceptionType";
      const string ExceptionWhat = "ExceptionWhat";
      (_testResult_WriteTestCaseNumberIfAnyMocked.*expectedResponsibleCallResultField).exceptionTypeName = &ExceptionTypeName;
      (_testResult_WriteTestCaseNumberIfAnyMocked.*expectedResponsibleCallResultField).exceptionWhat = ExceptionWhat;
      (_testResult_WriteTestCaseNumberIfAnyMocked.*expectedResponsibleCallResultField).testPhase = testPhase;
      _testResult_WriteTestCaseNumberIfAnyMocked.responsibleCallResultField = expectedResponsibleCallResultField;

      const string TestFailureNumber = "<20>";
      _testFailureNumbererMock.NextMock.ExpectAndReturn(TestFailureNumber);

      _testResult_WriteTestCaseNumberIfAnyMocked.testCaseIndex = 1;
      _testResult_WriteTestCaseNumberIfAnyMocked.WriteTestCaseNumberIfAnyMock.Expect();

      _consoleMock.WriteMock.Expect();
      _consoleMock.WriteLineMock.Expect();
      _consoleMock.WriteNewlineMock.Expect();
      //
      _testResult_WriteTestCaseNumberIfAnyMocked.PrintIfFailure(&_consoleMock, &_testFailureNumbererMock);
      //
      ZEN(_testFailureNumbererMock.NextMock.AssertCalledOnce());
      ZEN(_testResult_WriteTestCaseNumberIfAnyMocked.WriteTestCaseNumberIfAnyMock.
         AssertCalledOnceWith(&_consoleMock, _testResult_WriteTestCaseNumberIfAnyMocked.testCaseIndex));
      ZEN(_consoleMock.WriteMock.AssertCalledOnceWith(_testResult_WriteTestCaseNumberIfAnyMocked.fullName.testClassTestNameLines));
      ZEN(_consoleMock.WriteLineMock.AssertCalls(
      {
         TestFailureNumber,
         expectedTestPhaseSuffix,
         "Threw exception: " + ExceptionTypeName,
         "what(): \"" + ExceptionWhat + "\""
      }));
      ZEN(_consoleMock.WriteNewlineMock.AssertCalledOnce());
   }

   TEST(PrintIfFailure_SuccessButMissedDeadline_PrintsExpected)
   {
      _testResult_WriteTestCaseNumberIfAnyMocked.fullName.testClassTestNameLines = "TestClassTestNameLines";
      _testResult_WriteTestCaseNumberIfAnyMocked.testOutcome = TestOutcome::SuccessButMissedDeadline;
      _testResult_WriteTestCaseNumberIfAnyMocked.milliseconds = 10;

      const string TestFailureNumber = "<30>";
      _testFailureNumbererMock.NextMock.ExpectAndReturn(TestFailureNumber);

      _consoleMock.WriteLineMock.Expect();
      _consoleMock.WriteNewlineMock.Expect();

      _testResult_WriteTestCaseNumberIfAnyMocked.testCaseIndex = 1;
      _testResult_WriteTestCaseNumberIfAnyMocked.WriteTestCaseNumberIfAnyMock.Expect();
      //
      _testResult_WriteTestCaseNumberIfAnyMocked.PrintIfFailure(&_consoleMock, &_testFailureNumbererMock);
      //
      ZEN(_testFailureNumbererMock.NextMock.AssertCalledOnce());
      ZEN(_testResult_WriteTestCaseNumberIfAnyMocked.WriteTestCaseNumberIfAnyMock.
         AssertCalledOnceWith(&_consoleMock, _testResult_WriteTestCaseNumberIfAnyMocked.testCaseIndex));
      ZEN(_consoleMock.WriteLineMock.AssertCalls(
      {
         TestFailureNumber,
         _testResult_WriteTestCaseNumberIfAnyMocked.fullName.testClassTestNameLines,
         "Failed because test took longer than maxtestmilliseconds= (10 ms)"s
      }));
      ZEN(_consoleMock.WriteNewlineMock.AssertCalledOnce());
   }

   TEST(PrintIfFailure_InvalidOutcome_Throws)
   {
      _testResult.testOutcome = TestOutcome::Unset;
      THROWS(_testResult.PrintIfFailure(nullptr, nullptr), logic_error,
         R"(assert_true(testOutcome == TestOutcome::SuccessButMissedDeadline) failed in PrintIfFailure()
File.cpp(1))");
   }

   TEST3X3(WriteTestCaseNumberIfAny_WritesToConsoleTestCaseIndexPlus1IfTestCaseIndexNotNegative1,
      int testCaseIndex, bool expectConsoleWrite, int expectedTestCaseNumberWritten,
      -1, false, NA<int>(),
      0, true, 1,
      1, true, 2,
      2, true, 3)
   {
      if (expectConsoleWrite)
      {
         _consoleMock.WriteMock.Expect();
      }
      //
      _testResult.WriteTestCaseNumberIfAny(&_consoleMock, testCaseIndex);
      //
      if (expectConsoleWrite)
      {
         ZEN(_consoleMock.WriteMock.AssertCalledOnceWith(" test case " + to_string(expectedTestCaseNumberWritten)));
      }
   }

   TEST(ZenUnitEqualizer_ThrowsIfAnyFieldNotEqual)
   {
      EQUALIZER_THROWS_INIT(TestResult);
      EQUALIZER_THROWS(TestResult, fullName, FullName("ClassName", "TestName"));

      CallResult nonDefaultConstructorCallResult;
      nonDefaultConstructorCallResult.testPhase = TestPhase::Constructor;
      EQUALIZER_THROWS(TestResult, constructorCallResult, nonDefaultConstructorCallResult);

      CallResult nonDefaultStartupCallResult;
      nonDefaultStartupCallResult.testPhase = TestPhase::Startup;
      EQUALIZER_THROWS(TestResult, startupCallResult, nonDefaultStartupCallResult);

      CallResult nonDefaultTestBodyCallResult;
      nonDefaultTestBodyCallResult.testPhase = TestPhase::TestBody;
      EQUALIZER_THROWS(TestResult, testBodyCallResult, nonDefaultTestBodyCallResult);

      CallResult nonDefaultCleanupCallResult;
      nonDefaultCleanupCallResult.testPhase = TestPhase::Cleanup;
      EQUALIZER_THROWS(TestResult, cleanupCallResult, nonDefaultCleanupCallResult);

      CallResult nonDefaultDestructorCallResult;
      nonDefaultDestructorCallResult.testPhase = TestPhase::Destructor;
      EQUALIZER_THROWS(TestResult, destructorCallResult, nonDefaultDestructorCallResult);

      EQUALIZER_THROWS(TestResult, responsibleCallResultField, &TestResult::constructorCallResult);
      EQUALIZER_THROWS(TestResult, testOutcome, TestOutcome::Anomaly);
      EQUALIZER_THROWS(TestResult, testCaseIndex, 10);
      EQUALIZER_THROWS(TestResult, milliseconds, 20u);
   }

   }; RUN(TestResultTests)
}
