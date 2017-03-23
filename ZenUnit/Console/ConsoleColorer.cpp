#include "pch.h"
#include "ConsoleColorer.h"
#include "Utils/assert_true.h"
#ifdef __linux__
#include <unistd.h>
#include "LinuxColor.h"
#elif _WIN32
#include <io.h>
#include "WindowsColor.h"
#endif

namespace ZenUnit
{
   ConsoleColorer::ConsoleColorer()
      : fileno_ZenMockable(::fileno)
      , isatty_ZenMockable(::isatty)
#ifdef _WIN32
      , GetStdHandle_ZenMockable(::GetStdHandle)
      , SetConsoleTextAttribute_ZenMockable(::SetConsoleTextAttribute)
#endif
   {
   }

   bool ConsoleColorer::SetColor(Color color) const
   {
      bool doSetTextColor = color != Color::White && SupportsColor();
      if (doSetTextColor)
      {
         SetTextColor(color);
         return true;
      }
      return false;
   }

   void ConsoleColorer::UnsetColor(bool didSetTextColor) const
   {
      if (didSetTextColor)
      {
         SetTextColor(Color::White);
      }
   }

   bool ConsoleColorer::SupportsColor() const
   {
      int stdoutFileHandle = fileno_ZenMockable(stdout);
      int isAtty = isatty_ZenMockable(stdoutFileHandle);
      bool supportsColor = isAtty != 0;
      return supportsColor;
   }

   void ConsoleColorer::SetTextColor(Color color) const
   {
#ifdef __linux__
      const char* linuxColor = ColorToLinuxColor(color);
      cout << linuxColor;
#elif _WIN32
      HANDLE stdOutHandle = GetStdHandle_ZenMockable(STD_OUTPUT_HANDLE);
      WindowsColor windowsColor = ColorToWindowsColor(color);
      BOOL didSetConsoleTextAttr = SetConsoleTextAttribute_ZenMockable(
         stdOutHandle, static_cast<WORD>(windowsColor));
      assert_true(didSetConsoleTextAttr == TRUE);
#endif
   }
}
