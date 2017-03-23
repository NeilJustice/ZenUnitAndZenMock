#pragma once
#include <chrono>

namespace ZenUnit
{
   class Stopwatch
   {
      friend class StopwatchTests;
   private:
      std::function<std::chrono::time_point<
         std::chrono::high_resolution_clock>()> _highres_now_ZenMockable;
      std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
   public:
      Stopwatch();
      virtual ~Stopwatch() {}
      virtual void Start();
      virtual long long StopMilliseconds();
      virtual long long StopNanoseconds();
   private:
      long long Stop(bool nanosecondsFalseMilliseconds);
   };
}
