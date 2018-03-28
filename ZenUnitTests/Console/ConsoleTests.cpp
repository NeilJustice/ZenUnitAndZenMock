#include "pch.h"
#include "ZenUnitTests/Console/Mock/ConsoleColorerMock.h"

#if defined __linux__
#include <unistd.h>
#elif _WIN32
#include <io.h>
// fileno() and isatty() are Linux and Windows wheras _fileno() and _isatty() are Windows only
#pragma warning(disable: 4996) // 'fileno': The POSIX name for this item is deprecated
#endif

namespace ZenUnit
{
   TESTS(ConsoleTests)
   AFACT(Constructor_NewsConsoleColorer_SetsFunctionPointers)
   AFACT(Write_CallsWriteColorWithWhite)
   FACTS(WriteColor_WritesMessageInSpecifiedColor)
   AFACT(WriteLine_CallsWriteLineWithWhite)
   FACTS(WriteLineColor_WritesMessageInSpecifiedColorThenNewLine)
   FACTS(WriteLineAndExit_CallsWriteLineAndExit)
   AFACT(WriteStringsCommaSeparated_CallsDoWriteStringsCommaSeparated)
   FACTS(DoWriteStringsCommaSeparated_PrintsCommaSeparatedLengthNumberOfVectorValuesAtSpecifiedOffset)
   FACTS(WaitForAnyKeyIfDebuggerPresentOrValueTrue_WritesPressAnyKeyAndGetsCharIfDebuggerPresentOrValueTrue)
#if defined _WIN32
   FACTS(DebuggerIsPresent_ReturnsTrueIfIsDebuggerPresentFunctionReturns1)
   AFACT(WaitForAnyKey_CallsGetCh)
#endif
   EVIDENCE

   Console _console;
   ConsoleColorerMock* _consoleColorerMock = nullptr;
   const string Message = Random<string>();
#if _WIN32
   ZENMOCK_NONVOID0_FREE(int, _getch);
#endif

   struct ConsoleSelfMocked : public Zen::Mock<Console>
   {
      ZENMOCK_VOID2_CONST(WriteColor, const string&, Color)
      ZENMOCK_VOID2_CONST(WriteLineColor, const string&, Color)
      ZENMOCK_NONVOID0_CONST(bool, DebuggerIsPresent)
      ZENMOCK_VOID0_CONST(WaitForAnyKey)
      ZENMOCK_VOID0_CONST(WriteNewLine)
   } _consoleSelfMocked;

   STARTUP
   {
      _console._consoleColorer.reset(_consoleColorerMock = new ConsoleColorerMock);
#if defined _WIN32
      _console.call_getch = ZENMOCK_BIND0(_getch_ZenMock);
#endif
   }

   TEST(Constructor_NewsConsoleColorer_SetsFunctionPointers)
   {
      Console console;
      POINTER_WAS_NEWED(console._consoleColorer);
      STD_FUNCTION_TARGETS(::exit, console.call_exit);
#if defined _WIN32
      STD_FUNCTION_TARGETS(::IsDebuggerPresent, console.call_IsDebuggerPresent);
      STD_FUNCTION_TARGETS(_getch, console.call_getch);
#endif
   }

   TEST(Write_CallsWriteColorWithWhite)
   {
      _consoleSelfMocked.WriteColorMock.Expect();
      //
      _consoleSelfMocked.Write(Message);
      //
      ZEN(_consoleSelfMocked.WriteColorMock.CalledOnceWith(Message, Color::White));
   }

   TEST2X2(WriteColor_WritesMessageInSpecifiedColor,
      Color color, bool setColorReturnValue,
      Color::Green, false,
      Color::Green, true,
      Color::Red, false,
      Color::Red, true,
      Color::Teal, false,
      Color::Teal, true)
   {
      _consoleColorerMock->SetColorMock.Return(setColorReturnValue);
      _consoleColorerMock->UnsetColorMock.Expect();
      //
      _console.WriteColor(Message, color);
      //
      ZEN(_consoleColorerMock->SetColorMock.CalledOnceWith(color));
      ZEN(_consoleColorerMock->UnsetColorMock.CalledOnceWith(setColorReturnValue));
   }

   TEST(WriteLine_CallsWriteLineWithWhite)
   {
      _consoleSelfMocked.WriteLineColorMock.Expect();
      //
      _consoleSelfMocked.WriteLine(Message);
      //
      ZEN(_consoleSelfMocked.WriteLineColorMock.CalledOnceWith(Message, Color::White));
   }

   TEST2X2(WriteLineColor_WritesMessageInSpecifiedColorThenNewLine,
      Color color, bool setColorReturnValue,
      Color::Green, false,
      Color::Green, true,
      Color::Red, false,
      Color::Red, true,
      Color::Teal, false,
      Color::Teal, true)
   {
      _consoleColorerMock->SetColorMock.Return(setColorReturnValue);
      _consoleColorerMock->UnsetColorMock.Expect();
      //
      _console.WriteLineColor(Message, color);
      //
      ZEN(_consoleColorerMock->SetColorMock.CalledOnceWith(color));
      ZEN(_consoleColorerMock->UnsetColorMock.CalledOnceWith(setColorReturnValue));
   }

   TEST1X1(WriteLineAndExit_CallsWriteLineAndExit,
      int exitCode,
      0,
      1)
   {
      ZENMOCK_VOID1_FREE(exit, int);
      exit_ZenMock.Expect();
      _console.call_exit = ZENMOCK_BIND1(exit_ZenMock);
      //
      _console.WriteLineAndExit(Message, exitCode);
      //
      ZEN(exit_ZenMock.CalledOnceWith(exitCode));
   }

   TEST(WriteStringsCommaSeparated_CallsDoWriteStringsCommaSeparated)
   {
      struct ConsoleSelfMocked : public Zen::Mock<Console>
      {
         ZENMOCK_VOID3_CONST(DoWriteStringsCommaSeparated, const vector<string>&, size_t, size_t)
      } consoleSelfMocked;
      vector<string> strings = { Random<string>() };
      const size_t startIndex = Random<size_t>();
      const size_t numberOfElements = Random<size_t>();
      consoleSelfMocked.DoWriteStringsCommaSeparatedMock.Expect();
      //
      consoleSelfMocked.WriteStringsCommaSeparated(strings, startIndex, numberOfElements);
      //
      consoleSelfMocked.DoWriteStringsCommaSeparatedMock.CalledOnceWith(strings, startIndex, numberOfElements);
   }

   TEST4X4(DoWriteStringsCommaSeparated_PrintsCommaSeparatedLengthNumberOfVectorValuesAtSpecifiedOffset,
      const vector<string>& strings, size_t startIndex, size_t numberOfElements, const vector<string>& expectedConsoleWrites,
      vector<string>{ "Argument1" }, size_t(0), size_t(1), vector<string>{ "Argument1" },
      vector<string>{ "Argument1", "Argument2" }, size_t(0), size_t(2), vector<string>{ "Argument1", ", ", "Argument2" },
      vector<string>{ "Argument1", "Argument2" }, size_t(1), size_t(1), vector<string>{ "Argument2" },
      vector<string>{ "Argument1", "Argument2", "Argument3", "Argument4" }, size_t(2), size_t(2), vector<string>{ "Argument3", ", ", "Argument4" })
   {
      struct ConsoleSelfMocked : public Zen::Mock<Console>
      {
         ZENMOCK_VOID1_CONST(Write, const string&)
      } consoleSelfMocked;
      consoleSelfMocked.WriteMock.Expect();
      //
      consoleSelfMocked.DoWriteStringsCommaSeparated(strings, startIndex, numberOfElements);
      //
      vector<ZenMock::OneArgumentCallRef<const string&>> expectedConsoleWriteCalls;
      expectedConsoleWriteCalls.reserve(expectedConsoleWrites.size());
      for (const string& expectedConsoleWrite : expectedConsoleWrites)
      {
         expectedConsoleWriteCalls.emplace_back(expectedConsoleWrite);
      }
      ZEN(consoleSelfMocked.WriteMock.CalledAsFollows(expectedConsoleWriteCalls));
   }

   TEST3X3(WaitForAnyKeyIfDebuggerPresentOrValueTrue_WritesPressAnyKeyAndGetsCharIfDebuggerPresentOrValueTrue,
      bool doWait, bool debuggerIsPresent, bool expectPressAnyKeyAndGetChar,
      false, false, false,
      false, true, true,
      true, false, true,
      true, true, true)
   {
      if (!doWait)
      {
         _consoleSelfMocked.DebuggerIsPresentMock.Return(debuggerIsPresent);
      }
      if (expectPressAnyKeyAndGetChar)
      {
         _consoleSelfMocked.WriteLineColorMock.Expect();
         _consoleSelfMocked.WaitForAnyKeyMock.Expect();
      }
      //
      _consoleSelfMocked.WaitForAnyKeyIfDebuggerPresentOrValueTrue(doWait);
      //
      if (!doWait)
      {
         ZEN(_consoleSelfMocked.DebuggerIsPresentMock.CalledOnce());
      }
      if (expectPressAnyKeyAndGetChar)
      {
         ZEN(_consoleSelfMocked.WriteLineColorMock.CalledOnceWith("Press any key to continue . . .", Color::White));
         ZEN(_consoleSelfMocked.WaitForAnyKeyMock.CalledOnce());
      }
   }

#if defined _WIN32
   TEST2X2(DebuggerIsPresent_ReturnsTrueIfIsDebuggerPresentFunctionReturns1,
      int isDebuggerPresentReturnValue, bool expectedReturnValue,
      0, false,
      1, true,
      2, false)
   {
      ZENMOCK_NONVOID0_FREE(int, IsDebuggerPresent);
      IsDebuggerPresent_ZenMock.Return(isDebuggerPresentReturnValue);
      _console.call_IsDebuggerPresent = ZENMOCK_BIND0(IsDebuggerPresent_ZenMock);
      //
      const bool debuggerIsPresent = _console.DebuggerIsPresent();
      //
      ZEN(IsDebuggerPresent_ZenMock.CalledOnce());
      ARE_EQUAL(expectedReturnValue, debuggerIsPresent);
   }

   TEST(WaitForAnyKey_CallsGetCh)
   {
      _getch_ZenMock.Return(0);
      //
      _console.WaitForAnyKey();
      //
      ZEN(_getch_ZenMock.CalledOnce());
   }
#endif

   RUN_TESTS(ConsoleTests)


   TESTS(ConsoleColorerTests)
   AFACT(Constructor_SetsFunctionPointers_SetsSupportsColorAndSupportsColorSetToFalse)
   FACTS(SetSupportsColorIfUnset_SetsSupportsColorIfUnset)
   FACTS(SetColor_CallsSupportsColorAndSetTextColorIfColorNotWhite)
   FACTS(UnsetColor_CallsSetTextColorWhiteIfDidSetTextColorTrue)
   FACTS(SupportsColor_ReturnsTrueIfStdoutIsATTY)
   #if defined _WIN32
   FACTS(SetTextColor_WIN32_CallsWin32ApiToSetColorTextColor)
   #endif
   EVIDENCE

   struct ConsoleColorer_SupportsColorMocked : public Zen::Mock<ConsoleColorer>
   {
      ZENMOCK_NONVOID0_CONST(bool, SupportsColor)
   };

   struct ConsoleColorer_SetCallsMocked : public Zen::Mock<ConsoleColorer>
   {
      ZENMOCK_VOID0(SetSupportsColorIfUnset)
      ZENMOCK_VOID1_CONST(SetTextColor, Color)
   };

   ConsoleColorer _consoleColorer;
   ConsoleColorer_SupportsColorMocked _consoleColorer_SupportsColorMocked;
   ConsoleColorer_SetCallsMocked _consoleColorer_SetCallsMocked;

   TEST(Constructor_SetsFunctionPointers_SetsSupportsColorAndSupportsColorSetToFalse)
   {
      const ConsoleColorer consoleColorer;
      STD_FUNCTION_TARGETS(::fileno, consoleColorer.call_fileno);
      STD_FUNCTION_TARGETS(::isatty, consoleColorer.call_isatty);
   #if defined _WIN32
      STD_FUNCTION_TARGETS(::GetStdHandle, consoleColorer.call_GetStdHandle);
      STD_FUNCTION_TARGETS(::SetConsoleTextAttribute, consoleColorer.call_SetConsoleTextAttribute);
   #endif
      IS_FALSE(consoleColorer._supportsColor);
      IS_FALSE(consoleColorer._supportsColorSet);
   }

   TEST2X2(SetSupportsColorIfUnset_SetsSupportsColorIfUnset,
      bool supportsColorSet, bool expectSupportsColorCall,
      false, true,
      true, false)
   {
      _consoleColorer_SupportsColorMocked._supportsColorSet = supportsColorSet;
      const bool supportsColorReturnValue = ZenUnit::Random<bool>();
      if (expectSupportsColorCall)
      {
         _consoleColorer_SupportsColorMocked.SupportsColorMock.Return(supportsColorReturnValue);
      }
      //
      _consoleColorer_SupportsColorMocked.SetSupportsColorIfUnset();
      //
      if (expectSupportsColorCall)
      {
         ZEN(_consoleColorer_SupportsColorMocked.SupportsColorMock.CalledOnce());
         ARE_EQUAL(_consoleColorer_SupportsColorMocked._supportsColor, supportsColorReturnValue);
      }
      IS_TRUE(_consoleColorer_SupportsColorMocked._supportsColorSet);
   }

   TEST3X3(SetColor_CallsSupportsColorAndSetTextColorIfColorNotWhite,
      Color color,
      bool supportsColor,
      bool expectSetTextColorCallAndExpectedReturnValue,
      Color::White, false, false,
      Color::White, true, false,
      Color::Green, false, false,
      Color::Green, true, true,
      Color::Red, false, false,
      Color::Red, true, true,
      Color::Red, false, false,
      Color::Teal, true, true)
   {
      _consoleColorer_SetCallsMocked._supportsColor = supportsColor;
      _consoleColorer_SetCallsMocked.SetSupportsColorIfUnsetMock.Expect();
      if (expectSetTextColorCallAndExpectedReturnValue)
      {
         _consoleColorer_SetCallsMocked.SetTextColorMock.Expect();
      }
      //
      const bool didSetColor = _consoleColorer_SetCallsMocked.SetColor(color);
      //
      ZEN(_consoleColorer_SetCallsMocked.SetSupportsColorIfUnsetMock.CalledOnce());
      if (expectSetTextColorCallAndExpectedReturnValue)
      {
         ZEN(_consoleColorer_SetCallsMocked.SetTextColorMock.CalledOnceWith(color));
      }
      ARE_EQUAL(expectSetTextColorCallAndExpectedReturnValue, didSetColor);
   }

   TEST2X2(UnsetColor_CallsSetTextColorWhiteIfDidSetTextColorTrue,
      bool didSetTextColor, bool expectSetTextColorWhite,
      false, false,
      true, true)
   {
      if (expectSetTextColorWhite)
      {
         _consoleColorer_SetCallsMocked.SetTextColorMock.Expect();
      }
      //
      _consoleColorer_SetCallsMocked.UnsetColor(didSetTextColor);
      //
      if (expectSetTextColorWhite)
      {
         _consoleColorer_SetCallsMocked.SetTextColorMock.CalledOnceWith(Color::White);
      }
   }

   TEST2X2(SupportsColor_ReturnsTrueIfStdoutIsATTY,
      int isattyReturnValue, bool expectedReturnValue,
      -1, true,
      0, false,
      1, true)
   {
      ZENMOCK_NONVOID1_FREE(int, fileno, FILE*);
      ZENMOCK_NONVOID1_FREE(int, isatty, int);
      const int StdoutFileHandle = 1;
      fileno_ZenMock.Return(StdoutFileHandle);
      isatty_ZenMock.Return(isattyReturnValue);
      _consoleColorer.call_fileno = ZENMOCK_BIND1(fileno_ZenMock);
      _consoleColorer.call_isatty = ZENMOCK_BIND1(isatty_ZenMock);
      //
      const bool consoleSupportsColor = _consoleColorer.SupportsColor();
      //
      ZEN(fileno_ZenMock.CalledOnceWith(stdout));
      ZEN(isatty_ZenMock.CalledOnceWith(StdoutFileHandle));
      ARE_EQUAL(expectedReturnValue, consoleSupportsColor);
   }

   #if defined _WIN32
   TEST2X2(SetTextColor_WIN32_CallsWin32ApiToSetColorTextColor,
      Color color, WindowsColor expectedWindowsColor,
      Color::White, WindowsColor::White,
      Color::Green, WindowsColor::Green,
      Color::Red, WindowsColor::Red,
      Color::Teal, WindowsColor::Teal)
   {
      ZENMOCK_NONVOID1_FREE(HANDLE, GetStdHandle, DWORD);
      ZENMOCK_NONVOID2_FREE(BOOL, SetConsoleTextAttribute, HANDLE, WORD);
      _consoleColorer.call_GetStdHandle = ZENMOCK_BIND1(GetStdHandle_ZenMock);
      _consoleColorer.call_SetConsoleTextAttribute = ZENMOCK_BIND2(SetConsoleTextAttribute_ZenMock);

      const HANDLE GetStdHandleReturnValue = reinterpret_cast<HANDLE>(1);
      GetStdHandle_ZenMock.Return(GetStdHandleReturnValue);

      SetConsoleTextAttribute_ZenMock.Return(TRUE);
      //
      _consoleColorer.SetTextColor(color);
      //
      GetStdHandle_ZenMock.CalledOnceWith(STD_OUTPUT_HANDLE);
      SetConsoleTextAttribute_ZenMock.CalledOnceWith(
         GetStdHandleReturnValue, static_cast<WORD>(expectedWindowsColor));
   }
   #endif

   RUN_TESTS(ConsoleColorerTests)


   TESTS(LinuxColorTests)
   FACTS(ColorToLinuxColor_ReturnsExpected)
   EVIDENCE

   TEST2X2(ColorToLinuxColor_ReturnsExpected,
      Color color, const char* expectedLinuxColor,
      Color::Red, "\033[31m",
      Color::White, "\033[0m",
      Color::Teal, "\033[34m",
      Color::Green, "\033[32m",
      Color::Yellow, "\033[33m",
      Color::Unset, "\033[0m",
      static_cast<Color>(-1), "\033[0m")
   {
      ARE_EQUAL(expectedLinuxColor, ColorToLinuxColor(color));
   }

   RUN_TESTS(LinuxColorTests)


   TESTS(WindowsColorTests)
   FACTS(ColorToWindowsColor_ReturnsExpectedWindowsColor)
   EVIDENCE

   TEST2X2(ColorToWindowsColor_ReturnsExpectedWindowsColor,
      Color color, WindowsColor expectedWindowsColor,
      Color::Red, WindowsColor::Red,
      Color::White, WindowsColor::White,
      Color::Teal, WindowsColor::Teal,
      Color::Green, WindowsColor::Green,
      Color::Yellow, WindowsColor::Yellow,
      Color::Unset, WindowsColor::White,
      static_cast<Color>(-1), WindowsColor::White)
   {
      const WindowsColor windowsColor = ZenUnit::ColorToWindowsColor(color);
      ARE_EQUAL(expectedWindowsColor, windowsColor);
   }

   RUN_TESTS(WindowsColorTests)
}
