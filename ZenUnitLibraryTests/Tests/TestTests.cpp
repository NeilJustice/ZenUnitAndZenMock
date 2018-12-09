#include "pch.h"
#include "ZenUnitLibraryTests/Results/Mock/TestResultFactoryMock.h"
#include "ZenUnitLibraryTests/Results/Mock/TestResultMock.h"
#include "ZenUnitLibraryTests/TestRunners/Mock/TryCatchCallerMock.h"
#include "ZenUnitLibraryTests/Tests/Mock/TestMock.h"
#include "ZenUnitTestUtils/Equalizers/CallResultEqualizer.h"
#include "ZenUnitTestUtils/Equalizers/FileLineEqualizer.h"
#include "ZenUnitTestUtils/Equalizers/FullTestNameEqualizer.h"
#include "ZenUnitTestUtils/Equalizers/TestResultEqualizer.h"

namespace ZenUnit
{
   TESTS(TestTests)
   AFACT(TwoArgConstructor_NewsComponents_SetsFullName_NameFunctionReturnsTestName)
   FACTS(BaseRunTest_ConstructorFails_DoesNotCallSubsequentTestPhases_ReturnsTestResultConstructorFail)
   FACTS(BaseRunTest_ConstructorSucceeds_StartupFails_DoesNotCallTest_DoesNotCallCleanup_CallsDestructor_ReturnsTestResultStartupFail)
   AFACT(BaseRunTest_AllTestPhasesSucceed_ReturnsExpectedTestResult)
   AFACT(WritePostTestNameMessage_DoesNothing)
   AFACT(PrintPostTestCompletionMessage_DoesNothing)
   AFACT(StaticCallNewTestClass_CallsNewTestClass)
   AFACT(StaticCallStartup_CallsStartup)
   AFACT(StaticTestBody_CallsTestBody)
   AFACT(StaticCallCleanup_CallsCleanup)
   AFACT(StaticCallDeleteTestClass_CallsDeleteTestClass)
   AFACT(PseudoAbstractFunctions_DoNothingOrReturn0)
   EVIDENCE

   unique_ptr<Test> _test;
   TryCatchCallerMock* _tryCatchCallerMock = nullptr;
   TestResultFactoryMock* _testResultFactoryMock = nullptr;
   TestMock _testMock;

   STARTUP
   {
      _test = make_unique<Test>("", "", static_cast<unsigned char>(0));
      _test->_tryCatchCaller.reset(_tryCatchCallerMock = new TryCatchCallerMock);
      _test->_testResultFactory.reset(_testResultFactoryMock = new TestResultFactoryMock);
   }

   TEST(TwoArgConstructor_NewsComponents_SetsFullName_NameFunctionReturnsTestName)
   {
      const string testClassName = Random<string>();
      const string testName = Random<string>();
      //
      Test test(testClassName.c_str(), testName.c_str(), 0);
      POINTER_WAS_NEWED(test._tryCatchCaller);
      POINTER_WAS_NEWED(test._testResultFactory);
      ARE_EQUAL(FileLine(), test.p_fileLine);

      const char* const testNameValue = test.Name();
      ARE_EQUAL(testName.c_str(), testNameValue);

      const string fullTestName = test.FullTestNameValue();
      ARE_EQUAL(fullTestName, test.p_fullTestName.Value());

      test.p_fileLine = FileLine("FilePath", 1);
      ARE_EQUAL(test.p_fileLine.ToString(), test.FileLineString());
   }

   TEST(WritePostTestNameMessage_DoesNothing)
   {
      _test->WritePostTestNameMessage(nullptr);
      _test->WritePostTestNameMessage(nullptr);
   }

   TEST(PrintPostTestCompletionMessage_DoesNothing)
   {
      TestResultMock testResultMock;
      _test->WritePostTestCompletionMessage(nullptr, testResultMock);
      _test->WritePostTestCompletionMessage(nullptr, testResultMock);
   }

   static CallResult CallResultWithOutcome(TestOutcome testOutcome)
   {
      CallResult callResult;
      callResult.testOutcome = testOutcome;
      return callResult;
   }

   TEST1X1(BaseRunTest_ConstructorFails_DoesNotCallSubsequentTestPhases_ReturnsTestResultConstructorFail,
      TestOutcome constructorOutcome, TestOutcome::Anomaly, TestOutcome::Exception)
   {
      const CallResult constructorFailCallResult = CallResultWithOutcome(constructorOutcome);
      _tryCatchCallerMock->RunTestPhaseMock.Return(constructorFailCallResult);

      const TestResult constructorFailTestResult = TestResult::TestingNonDefault();
      _testResultFactoryMock->MakeConstructorFailMock.Return(constructorFailTestResult);
      //
      const TestResult testResult = _test->BaseRunTest();
      //
      ZEN(_tryCatchCallerMock->RunTestPhaseMock.CalledOnceWith(&Test::CallNewTestClass, _test.get(), TestPhase::Constructor));
      ZEN(_testResultFactoryMock->MakeConstructorFailMock.CalledOnceWith(_test->p_fullTestName, constructorFailCallResult));
      ARE_EQUAL(constructorFailTestResult, testResult);
   }

   TEST1X1(BaseRunTest_ConstructorSucceeds_StartupFails_DoesNotCallTest_DoesNotCallCleanup_CallsDestructor_ReturnsTestResultStartupFail,
      TestOutcome startupOutcome,
      TestOutcome::Anomaly,
      TestOutcome::Exception)
   {
      const CallResult constructorSuccessCallResult = CallResultWithOutcome(TestOutcome::Success);
      const CallResult startupFailCallResult = CallResultWithOutcome(startupOutcome);
      const CallResult destructorCallResult = CallResultWithOutcome(TestOutcome::Success);
      _tryCatchCallerMock->RunTestPhaseMock.ReturnValues(constructorSuccessCallResult, startupFailCallResult, destructorCallResult);

      const TestResult startupFailTestResult = TestResult::TestingNonDefault();
      _testResultFactoryMock->MakeStartupFailMock.Return(startupFailTestResult);
      const string testClassName = Random<string>();
      const string testName = Random<string>();
      _test->p_fullTestName = FullTestName(testClassName.c_str(), testName.c_str(), 0);
      //
      const TestResult testResult = _test->BaseRunTest();
      //
      ZEN(_tryCatchCallerMock->RunTestPhaseMock.CalledAsFollows(
      {
         { &Test::CallNewTestClass, _test.get(), TestPhase::Constructor },
         { &Test::CallStartup, _test.get(), TestPhase::Startup },
         { &Test::CallDeleteTestClass, _test.get(), TestPhase::Destructor }
      }));
      ZEN(_testResultFactoryMock->MakeStartupFailMock.CalledOnceWith(
         _test->p_fullTestName, constructorSuccessCallResult, startupFailCallResult, destructorCallResult));
      ARE_EQUAL(startupFailTestResult, testResult);
   }

   TEST(BaseRunTest_AllTestPhasesSucceed_ReturnsExpectedTestResult)
   {
      const CallResult successCallResult = CallResultWithOutcome(TestOutcome::Success);
      _tryCatchCallerMock->RunTestPhaseMock.Return(successCallResult);

      const TestResult sixArgTestResult = TestResult::TestingNonDefault();
      _testResultFactoryMock->MakeFullTestResultMock.Return(sixArgTestResult);
      const string testClassName = Random<string>();
      const string testName = Random<string>();
      _test->p_fullTestName = FullTestName(testClassName.c_str(), testName.c_str(), 0);
      //
      const TestResult testResult = _test->BaseRunTest();
      //
      ZEN(_tryCatchCallerMock->RunTestPhaseMock.CalledAsFollows(
      {
         { &Test::CallNewTestClass, _test.get(), TestPhase::Constructor },
         { &Test::CallStartup, _test.get(), TestPhase::Startup },
         { &Test::CallTestBody, _test.get(), TestPhase::TestBody },
         { &Test::CallCleanup, _test.get(), TestPhase::Cleanup },
         { &Test::CallDeleteTestClass, _test.get(), TestPhase::Destructor }
      }));
      ZEN(_testResultFactoryMock->MakeFullTestResultMock.CalledOnceWith(
         _test->p_fullTestName,
         CallResultWithOutcome(TestOutcome::Success),
         CallResultWithOutcome(TestOutcome::Success),
         CallResultWithOutcome(TestOutcome::Success),
         CallResultWithOutcome(TestOutcome::Success),
         CallResultWithOutcome(TestOutcome::Success)));
      ARE_EQUAL(testResult, sixArgTestResult);
   }

   TEST(StaticCallNewTestClass_CallsNewTestClass)
   {
      _testMock.NewTestClassMock.Expect();
      //
      Test::CallNewTestClass(&_testMock);
      //
      ZEN(_testMock.NewTestClassMock.CalledOnce());
   }

   TEST(StaticCallStartup_CallsStartup)
   {
      _testMock.StartupMock.Expect();
      //
      Test::CallStartup(&_testMock);
      //
      ZEN(_testMock.StartupMock.CalledOnce());
   }

   TEST(StaticTestBody_CallsTestBody)
   {
      _testMock.TestBodyMock.Expect();
      //
      Test::CallTestBody(&_testMock);
      //
      ZEN(_testMock.TestBodyMock.CalledOnce());
   }

   TEST(StaticCallCleanup_CallsCleanup)
   {
      _testMock.CleanupMock.Expect();
      //
      Test::CallCleanup(&_testMock);
      //
      ZEN(_testMock.CleanupMock.CalledOnce());
   }

   TEST(StaticCallDeleteTestClass_CallsDeleteTestClass)
   {
      _testMock.DeleteTestClassMock.Expect();
      //
      Test::CallDeleteTestClass(&_testMock);
      //
      ZEN(_testMock.DeleteTestClassMock.CalledOnce());
   }

   TEST(PseudoAbstractFunctions_DoNothingOrReturn0)
   {
      Test test("", "", 0);
      ARE_EQUAL(0, test.NumberOfTestCases());
      IS_EMPTY(test.RunTest());
      test.NewTestClass();
      test.Startup();
      test.TestBody();
      test.Cleanup();
      test.DeleteTestClass();
   }

   RUN_TESTS(TestTests)
}
