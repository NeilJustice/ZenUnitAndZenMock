#include "pch.h"
#include "ZenUnitLibraryTests/ValueTypes/Args/ExpectedCommandLineUsage.h"
#include "ZenUnitLibraryTests/Components/Args/MetalMock/TestNameFilterStringParserMock.h"
#include "ZenUnitLibraryTests/Components/Console/MetalMock/ConsoleMock.h"
#include "ZenUnitTestUtils/EqualizersAndRandoms/TestNameFilterEqualizerAndRandom.h"
#include "ZenUnitTestUtils/EqualizersAndRandoms/ZenUnitArgsEqualizerAndRandom.h"
#include "ZenUnitUtilsAndAssertionTests/Components/FunctionCallers/MetalMock/VoidOneArgMemberFunctionCallerMock.h"
#include "ZenUnitUtilsAndAssertionTests/Components/Time/MetalMock/WatchMock.h"

namespace ZenUnit
{
   TESTS(ArgsParserTests)
   AFACT(DefaultConstructor_NewsComponents_SetsStringToUnsignedFunction)
   AFACT(Parse_ArgsOnlyExePath_ReturnsDefaultZenUnitArgsWithCommandLineAndTestProgramNameSet)
   FACTS(Parse_ArgsSizeGreaterThanOrEqualTo13_PrintsTooManyArgumentsErrorMessageAndCommandLineUsageAndExits1)
   FACTS(Parse_InvalidArgument_PrintsErrorMessageAndCommandLineUsageAndExits1)
   AFACT(Parse_DashDashHelp_PrintsCommandLineUsageAndExits0)
   AFACT(Parse_DashDashVersion_PrintsVersionAndExits0)
   AFACT(Parse_AllArgumentsSpecifiedExpectForTestNameFilter_ReturnsZenUnitArgsWithAllFieldsSet)
   AFACT(Parse_DashDashRun_ReturnsZenUnitArgsWithExpectedTestNameFilters)
   AFACT(Parse_ValidBoolArg_ReturnsExpectedZenUnitArgs)
   AFACT(Parse_ValidBoolArgSpecifiedTwice_ReturnsExpectedZenUnitArgs)
   FACTS(Parse_ArgContainsEqualsSign_ValueIsEmptyString_PrintsErrorMessageAndCommandLineUsageAndExits1)
   AFACT(Parse_TimesEqualsArg_StringToUnsignedThrowsInvalidArgumentWhenProcessingValue_PrintsErrorMessageAndCommandLineUsageAndExits1)
   AFACT(Parse_TimesEqualsArg_ValidUnsignedValue_ReturnsExpectedZenUnitArgs)
   AFACT(Parse_RandomEqualsArg_ValidRandomUnsignedValue_ReturnsExpectedZenUnitArgs)
   AFACT(Parse_UnrecognizedEqualsSignArgName_PrintsCommandLineUsageAndExits1)
   AFACT(SetRandomSeedIfNotSetByUser_RandomSeedSetByUser_DoesNothing)
   AFACT(SetRandomSeedIfNotSetByUser_RandomSeedNotSetByUser_SetsRandomSeedToSecondsSince1970)
   EVIDENCE

   const string _testProgramPath = Random<string>();

   ArgsParser _argsParser;
   // Function Pointers
   METALMOCK_NONVOID1_STATIC(int, ZenUnit::String, ToInt, std::string_view)
   METALMOCK_NONVOID1_STATIC(unsigned, ZenUnit::String, ToUnsigned, std::string_view)
   // Function Callers
   VoidOneArgMemberFunctionCallerMock<ArgsParser, ZenUnitArgs&>* _caller_SetRandomSeedIfNotSetByUserMock = nullptr;
   // Constant Components
   ConsoleMock* _consoleMock = nullptr;
   TestNameFilterStringParserMock* _testNameFilterStringParserMock = nullptr;
   WatchMock* _watchMock = nullptr;

   STARTUP
   {
      // Function Pointers
      _argsParser._call_String_ToInt = BIND_1ARG_METALMOCK_OBJECT(ToIntMock);
      _argsParser._call_String_ToUnsigned = BIND_1ARG_METALMOCK_OBJECT(ToUnsignedMock);
      // Function Callers
      _argsParser._caller_SetRandomSeedIfNotSetByUser.reset(
         _caller_SetRandomSeedIfNotSetByUserMock = new VoidOneArgMemberFunctionCallerMock<ArgsParser, ZenUnitArgs&>);
      // Constant Components
      _argsParser._console.reset(_consoleMock = new ConsoleMock);
      _argsParser._testNameFilterStringParser.reset(_testNameFilterStringParserMock = new TestNameFilterStringParserMock);
      _argsParser._watch.reset(_watchMock = new WatchMock);
   }

   TEST(DefaultConstructor_NewsComponents_SetsStringToUnsignedFunction)
   {
      ArgsParser argsParser;
      // Function Pointers
      STD_FUNCTION_TARGETS(String::ToInt, argsParser._call_String_ToInt);
      STD_FUNCTION_TARGETS(String::ToUnsigned, argsParser._call_String_ToUnsigned);
      // Function Callers
      DELETE_TO_ASSERT_NEWED(argsParser._caller_SetRandomSeedIfNotSetByUser);
      // Constant Components
      DELETE_TO_ASSERT_NEWED(argsParser._console);
      DELETE_TO_ASSERT_NEWED(argsParser._testNameFilterStringParser);
      DELETE_TO_ASSERT_NEWED(argsParser._watch);
   }

   void ExpectCallToSetRandomSeedIfNotSetByUser()
   {
      _caller_SetRandomSeedIfNotSetByUserMock->CallConstMemberFunctionMock.Expect();
   }

   void AssertCallToSetRandomSeedIfNotSetByUser(ZenUnitArgs& expectedZenUnitArgsArg)
   {
      METALMOCK(_caller_SetRandomSeedIfNotSetByUserMock->CallConstMemberFunctionMock.CalledOnceWith(
         &_argsParser, &ArgsParser::SetRandomSeedIfNotSetByUser, expectedZenUnitArgsArg));
   }

   TEST(Parse_ArgsOnlyExePath_ReturnsDefaultZenUnitArgsWithCommandLineAndTestProgramNameSet)
   {
      const string startDateTime = _watchMock->DateTimeNowMock.ReturnRandom();
      ExpectCallToSetRandomSeedIfNotSetByUser();
      vector<string> stringArgs{ _testProgramPath };
      //
      const ZenUnitArgs zenUnitArgs = _argsParser.Parse(stringArgs);
      //
      METALMOCK(_watchMock->DateTimeNowMock.CalledOnce());
      ZenUnitArgs expectedZenUnitArgs;
      expectedZenUnitArgs.commandLine = _testProgramPath;
      expectedZenUnitArgs.startDateTime = startDateTime;
      AssertCallToSetRandomSeedIfNotSetByUser(expectedZenUnitArgs);
      ARE_EQUAL(expectedZenUnitArgs, zenUnitArgs);
   }

   TEST1X1(Parse_ArgsSizeGreaterThanOrEqualTo13_PrintsTooManyArgumentsErrorMessageAndCommandLineUsageAndExits1,
      size_t numberOfStringArgs,
      13ull,
      14ull)
   {
      _consoleMock->WriteLineMock.Expect();
      _consoleMock->WriteLineAndExitMock.ThrowExceptionWhenCalled<WriteLineAndExitException>();
      const vector<string> stringArgs(numberOfStringArgs);
      //
      THROWS_EXCEPTION(_argsParser.Parse(stringArgs), WriteLineAndExitException, "");
      //
      METALMOCK(_consoleMock->WriteLineMock.CalledOnceWith("ZenUnit command line usage error: Too many arguments.\n"));
      METALMOCK(_consoleMock->WriteLineAndExitMock.CalledOnceWith(ExpectedCommandLineUsage, 1));
   }

   TEST1X1(Parse_InvalidArgument_PrintsErrorMessageAndCommandLineUsageAndExits1,
      const string& invalidArgument,
      "--abc",
      "--Always-exit-0",
      "--test-runs")
   {
      _consoleMock->WriteLineMock.Expect();
      _consoleMock->WriteLineAndExitMock.ThrowExceptionWhenCalled<WriteLineAndExitException>();
      const vector<string> stringArgs { _testProgramPath, invalidArgument };
      //
      THROWS_EXCEPTION(_argsParser.Parse(stringArgs), WriteLineAndExitException, "");
      //
      METALMOCK(_consoleMock->WriteLineMock.CalledOnceWith(
         "ZenUnit command line usage error: Invalid argument \"" + invalidArgument + "\"\n"));
      METALMOCK(_consoleMock->WriteLineAndExitMock.CalledOnceWith(ExpectedCommandLineUsage, 1));
   }

   TEST(Parse_DashDashHelp_PrintsCommandLineUsageAndExits0)
   {
      _consoleMock->WriteLineAndExitMock.ThrowExceptionWhenCalled<WriteLineAndExitException>();
      const vector<string> stringArgs { _testProgramPath, "--help" };
      //
      THROWS_EXCEPTION(_argsParser.Parse(stringArgs), WriteLineAndExitException, "");
      //
      METALMOCK(_consoleMock->WriteLineAndExitMock.CalledOnceWith(ExpectedCommandLineUsage, 0));
   }

   TEST(Parse_DashDashVersion_PrintsVersionAndExits0)
   {
      _consoleMock->WriteLineAndExitMock.ThrowExceptionWhenCalled<WriteLineAndExitException>();
      const vector<string> stringArgs { _testProgramPath, "--version" };
      //
      THROWS_EXCEPTION(_argsParser.Parse(stringArgs), WriteLineAndExitException, "");
      //
      METALMOCK(_consoleMock->WriteLineAndExitMock.CalledOnceWith("0.7.1", 0));
   }

   TEST(Parse_AllArgumentsSpecifiedExpectForTestNameFilter_ReturnsZenUnitArgsWithAllFieldsSet)
   {
      const int testruns = ToIntMock.ReturnRandom();
      const unsigned randomSeed = ToUnsignedMock.ReturnRandom();
      _caller_SetRandomSeedIfNotSetByUserMock->CallConstMemberFunctionMock.Expect();
      const string startDateTime = _watchMock->DateTimeNowMock.ReturnRandom();
      const vector<string> stringArgs
      {
         _testProgramPath,
         "--pause-before",
         "--pause-after",
         "--always-exit-0",
         "--fail-fast",
         "--exit-1-if-tests-skipped",
         "--random-test-ordering",
         "--test-runs=" + to_string(testruns),
         "--random-seed=" + to_string(randomSeed)
      };
      //
      const ZenUnitArgs zenUnitArgs = _argsParser.Parse(stringArgs);
      //
      METALMOCK(ToIntMock.CalledOnceWith(to_string(testruns)));
      METALMOCK(ToUnsignedMock.CalledOnceWith(to_string(randomSeed)));
      METALMOCK(_watchMock->DateTimeNowMock.CalledOnce());
      ZenUnitArgs expectedZenUnitArgs;
      expectedZenUnitArgs.commandLine = VectorUtils::JoinWithSeparator(stringArgs, ' ');
      expectedZenUnitArgs.pauseBefore = true;
      expectedZenUnitArgs.pauseAfter = true;
      expectedZenUnitArgs.alwaysExit0 = true;
      expectedZenUnitArgs.failFast = true;
      expectedZenUnitArgs.exit1IfTestsSkipped = true;
      expectedZenUnitArgs.randomTestOrdering = true;
      expectedZenUnitArgs.testRuns = testruns;
      expectedZenUnitArgs.randomSeed = randomSeed;
      expectedZenUnitArgs.randomSeedSetByUser = true;
      expectedZenUnitArgs.startDateTime = startDateTime;
      AssertCallToSetRandomSeedIfNotSetByUser(expectedZenUnitArgs);
      ARE_EQUAL(expectedZenUnitArgs, zenUnitArgs);
   }

   TEST(Parse_DashDashRun_ReturnsZenUnitArgsWithExpectedTestNameFilters)
   {
      _caller_SetRandomSeedIfNotSetByUserMock->CallConstMemberFunctionMock.Expect();

      const vector<TestNameFilter> testNameFilters = { Random<TestNameFilter>() };
      _testNameFilterStringParserMock->ParseTestNameFilterStringsMock.Return(testNameFilters);

      const string unusedDateTimeNow = _watchMock->DateTimeNowMock.ReturnRandom();

      const string runArgument = ZenUnit::Random<string>();
      const vector<string> stringArgs { ZenUnit::Random<string>(), "--run=" + runArgument };
      //
      const ZenUnitArgs zenUnitArgs = _argsParser.Parse(stringArgs);
      //
      const vector<string> splitRunArgument = String::Split(runArgument, ',');
      METALMOCK(_testNameFilterStringParserMock->ParseTestNameFilterStringsMock.CalledOnceWith(splitRunArgument));

      METALMOCK(_watchMock->DateTimeNowMock.CalledOnce());

      ZenUnitArgs expectedZenUnitArgs;
      expectedZenUnitArgs.commandLine = VectorUtils::JoinWithSeparator(stringArgs, ' ');
      expectedZenUnitArgs.testNameFilters = testNameFilters;
      AssertCallToSetRandomSeedIfNotSetByUser(expectedZenUnitArgs);
      ARE_EQUAL(expectedZenUnitArgs, zenUnitArgs);
   }

   TEST(Parse_ValidBoolArg_ReturnsExpectedZenUnitArgs)
   {
      AssertArgSetsBoolField("--pause-before", &ZenUnitArgs::pauseBefore);
      Startup();

      AssertArgSetsBoolField("--pause-after", &ZenUnitArgs::pauseAfter);
      Startup();

      AssertArgSetsBoolField("--always-exit-0", &ZenUnitArgs::alwaysExit0);
      Startup();

      AssertArgSetsBoolField("--fail-fast", &ZenUnitArgs::failFast);
      Startup();

      AssertArgSetsBoolField("--exit-1-if-tests-skipped", &ZenUnitArgs::exit1IfTestsSkipped);
      Startup();

      AssertArgSetsBoolField("--random-test-ordering", &ZenUnitArgs::randomTestOrdering);
      Startup();
   }
   void AssertArgSetsBoolField(const string& arg, bool ZenUnitArgs::* expectedFieldToBeSet)
   {
      const string dateTimeNow = _watchMock->DateTimeNowMock.ReturnRandom();
      ExpectCallToSetRandomSeedIfNotSetByUser();
      const vector<string> stringArgs { _testProgramPath, arg };
      //
      const ZenUnitArgs zenUnitArgs = _argsParser.Parse(stringArgs);
      //
      METALMOCK(_watchMock->DateTimeNowMock.CalledOnce());
      ZenUnitArgs expectedZenUnitArgs;
      expectedZenUnitArgs.commandLine = _testProgramPath + " " + arg;
      (expectedZenUnitArgs.*expectedFieldToBeSet) = true;
      AssertCallToSetRandomSeedIfNotSetByUser(expectedZenUnitArgs);
      ARE_EQUAL(expectedZenUnitArgs, zenUnitArgs);
   }

   TEST(Parse_ValidBoolArgSpecifiedTwice_ReturnsExpectedZenUnitArgs)
   {
      const string dateTimeNow = _watchMock->DateTimeNowMock.ReturnRandom();
      ExpectCallToSetRandomSeedIfNotSetByUser();
      const vector<string> stringArgs { _testProgramPath, "--always-exit-0", "--always-exit-0" };
      //
      const ZenUnitArgs zenUnitArgs = _argsParser.Parse(stringArgs);
      //
      METALMOCK(_watchMock->DateTimeNowMock.CalledOnce());
      ZenUnitArgs expectedZenUnitArgs;
      expectedZenUnitArgs.commandLine = VectorUtils::JoinWithSeparator(stringArgs, ' ');
      expectedZenUnitArgs.alwaysExit0 = true;
      AssertCallToSetRandomSeedIfNotSetByUser(expectedZenUnitArgs);
      ARE_EQUAL(expectedZenUnitArgs, zenUnitArgs);
   }

   TEST1X1(Parse_ArgContainsEqualsSign_ValueIsEmptyString_PrintsErrorMessageAndCommandLineUsageAndExits1,
      const string& arg,
      "--test-runs=",
      "--test-runs===",
      "--random-seed=",
      "--random-seed===")
   {
      _consoleMock->WriteLineMock.Expect();
      _consoleMock->WriteLineAndExitMock.ThrowExceptionWhenCalled<WriteLineAndExitException>();
      const vector<string> stringArgs { _testProgramPath, arg };
      //
      THROWS_EXCEPTION(_argsParser.Parse(stringArgs), WriteLineAndExitException, "");
      //
      const string expectedErrorMessage =
         "ZenUnit command line usage error: " + string("String::Split(arg, '=') unexpectedly returned not 2 for arg = \"" + arg + "\"") + "\n";
      METALMOCK(_consoleMock->WriteLineMock.CalledOnceWith(expectedErrorMessage));
      METALMOCK(_consoleMock->WriteLineAndExitMock.CalledOnceWith(ExpectedCommandLineUsage, 1));
   }

   TEST(Parse_TimesEqualsArg_StringToUnsignedThrowsInvalidArgumentWhenProcessingValue_PrintsErrorMessageAndCommandLineUsageAndExits1)
   {
      _consoleMock->WriteLineMock.Expect();
      _consoleMock->WriteLineAndExitMock.ThrowExceptionWhenCalled<WriteLineAndExitException>();
      ToIntMock.ThrowExceptionWhenCalled<invalid_argument>("");
      const string InvalidTimesArg = "--test-runs=-1_for_example";
      const vector<string> stringArgs { _testProgramPath, InvalidTimesArg };
      //
      THROWS_EXCEPTION(_argsParser.Parse(stringArgs), WriteLineAndExitException, "");
      //
      METALMOCK(ToIntMock.CalledOnceWith("-1_for_example"));
      METALMOCK(_consoleMock->WriteLineMock.CalledOnceWith(
         "ZenUnit command line usage error: Invalid --name=value argument value: " + InvalidTimesArg + "\n"));
      METALMOCK(_consoleMock->WriteLineAndExitMock.CalledOnceWith(ExpectedCommandLineUsage, 1));
   }

   TEST(Parse_TimesEqualsArg_ValidUnsignedValue_ReturnsExpectedZenUnitArgs)
   {
      const string dateTimeNow = _watchMock->DateTimeNowMock.ReturnRandom();

      ExpectCallToSetRandomSeedIfNotSetByUser();

      const int testRuns = ToIntMock.ReturnRandom();
      const vector<string> stringArgs{ _testProgramPath, "--test-runs=" + to_string(testRuns) };
      //
      const ZenUnitArgs zenUnitArgs = _argsParser.Parse(stringArgs);
      //
      METALMOCK(ToIntMock.CalledOnceWith(to_string(testRuns)));
      METALMOCK(_watchMock->DateTimeNowMock.CalledOnce());
      ZenUnitArgs expectedZenUnitArgs;
      expectedZenUnitArgs.commandLine = VectorUtils::JoinWithSeparator(stringArgs, ' ');
      expectedZenUnitArgs.testRuns = testRuns;
      AssertCallToSetRandomSeedIfNotSetByUser(expectedZenUnitArgs);
      ARE_EQUAL(expectedZenUnitArgs, zenUnitArgs);
   }

   TEST(Parse_RandomEqualsArg_ValidRandomUnsignedValue_ReturnsExpectedZenUnitArgs)
   {
      const string dateTimeNow = _watchMock->DateTimeNowMock.ReturnRandom();
      ExpectCallToSetRandomSeedIfNotSetByUser();
      const unsigned randomSeed = ToUnsignedMock.ReturnRandom();
      const vector<string> stringArgs{ _testProgramPath, "--random-seed=" + to_string(randomSeed) };
      //
      const ZenUnitArgs zenUnitArgs = _argsParser.Parse(stringArgs);
      //
      METALMOCK(ToUnsignedMock.CalledOnceWith(to_string(randomSeed)));
      METALMOCK(_watchMock->DateTimeNowMock.CalledOnce());
      ZenUnitArgs expectedZenUnitArgs;
      expectedZenUnitArgs.commandLine = VectorUtils::JoinWithSeparator(stringArgs, ' ');
      expectedZenUnitArgs.randomTestOrdering = false;
      expectedZenUnitArgs.randomSeed = randomSeed;
      expectedZenUnitArgs.randomSeedSetByUser = true;
      AssertCallToSetRandomSeedIfNotSetByUser(expectedZenUnitArgs);
      ARE_EQUAL(expectedZenUnitArgs, zenUnitArgs);
   }

   TEST(Parse_UnrecognizedEqualsSignArgName_PrintsCommandLineUsageAndExits1)
   {
      _consoleMock->WriteLineMock.Expect();
      _consoleMock->WriteLineAndExitMock.ThrowExceptionWhenCalled<WriteLineAndExitException>();
      const string unrecognizedNameArg = String::Concat('-', ZenUnit::Random<string>(), '=', ZenUnit::Random<string>());
      const vector<string> stringArgs{ _testProgramPath, unrecognizedNameArg };
      //
      THROWS_EXCEPTION(_argsParser.Parse(stringArgs), WriteLineAndExitException, "");
      //
      const string expectedErrorMessage = String::Concat(
         "ZenUnit command line usage error: Unrecognized --name=value argument: ", unrecognizedNameArg, '\n');
      METALMOCK(_consoleMock->WriteLineMock.CalledOnceWith(expectedErrorMessage));
      METALMOCK(_consoleMock->WriteLineAndExitMock.CalledOnceWith(ExpectedCommandLineUsage, 1));
   }

   TEST(SetRandomSeedIfNotSetByUser_RandomSeedSetByUser_DoesNothing)
   {
      ZenUnitArgs zenUnitArgs = ZenUnit::Random<ZenUnitArgs>();
      zenUnitArgs.randomSeedSetByUser = true;
      ZenUnitArgs expectedResultingZenUnitArgs = zenUnitArgs;
      //
      _argsParser.SetRandomSeedIfNotSetByUser(zenUnitArgs);
      //
      ARE_EQUAL(expectedResultingZenUnitArgs, zenUnitArgs);
   }

   TEST(SetRandomSeedIfNotSetByUser_RandomSeedNotSetByUser_SetsRandomSeedToSecondsSince1970)
   {
      ZenUnitArgs zenUnitArgs = ZenUnit::Random<ZenUnitArgs>();
      zenUnitArgs.randomSeedSetByUser = false;

      ZenUnitArgs expectedResultingZenUnitArgs = zenUnitArgs;
      const long long secondsSince1970 = _watchMock->SecondsSince1970Mock.ReturnRandom();
      //
      _argsParser.SetRandomSeedIfNotSetByUser(zenUnitArgs);
      //
      METALMOCK(_watchMock->SecondsSince1970Mock.CalledOnce());
      expectedResultingZenUnitArgs.randomSeed = static_cast<unsigned>(secondsSince1970);
      ARE_EQUAL(expectedResultingZenUnitArgs, zenUnitArgs);
   }

   RUN_TESTS(ArgsParserTests)
}
