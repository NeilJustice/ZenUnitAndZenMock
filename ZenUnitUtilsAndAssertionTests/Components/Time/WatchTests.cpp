#include "pch.h"

namespace ZenUnit
{
   template<>
   class Equalizer<tm>
   {
   public:
      static void AssertEqual(const tm& expectedTm, const tm& actualTm)
      {
         ARE_EQUAL(expectedTm.tm_sec, actualTm.tm_sec);
         ARE_EQUAL(expectedTm.tm_min, actualTm.tm_min);
         ARE_EQUAL(expectedTm.tm_hour, actualTm.tm_hour);
         ARE_EQUAL(expectedTm.tm_mday, actualTm.tm_mday);
         ARE_EQUAL(expectedTm.tm_mon, actualTm.tm_mon);
         ARE_EQUAL(expectedTm.tm_year, actualTm.tm_year);
         ARE_EQUAL(expectedTm.tm_wday, actualTm.tm_wday);
         ARE_EQUAL(expectedTm.tm_yday, actualTm.tm_yday);
         ARE_EQUAL(expectedTm.tm_isdst, actualTm.tm_isdst);
      }
   };

   TESTS(WatchTests)
   AFACT(Constructor_SetsStrftTimeFunctionPointer)
   FACTS(DateTimeNow_ReturnsLocalDateTimeNow)
   AFACT(GetTimeZone_ReturnTimeZoneOfExecutingProcess)
   FACTS(MicrosecondsToTwoDecimalPlaceMillisecondsString_ReturnsMicrosecondsAsMillisecondsRoundedToThreePlaces)
   EVIDENCE

   class WatchSelfMocked : public Zen::Mock<ZenUnit::Watch>
   {
   public:
      ZENMOCK_NONVOID0_CONST(tm, TMNow)
      ZENMOCK_NONVOID1_CONST(string, GetTimeZone, const tm&)
   };
   unique_ptr<WatchSelfMocked> _watchSelfMocked;
   ZENMOCK_NONVOID4_FREE(size_t, strftime, char*, size_t, char const*, const tm*)

   STARTUP
   {
      _watchSelfMocked = make_unique<WatchSelfMocked>();
      _watchSelfMocked->_call_strftime = BIND_4ARG_ZENMOCK_OBJECT(strftimeMock);
   }

   TEST(Constructor_SetsStrftTimeFunctionPointer)
   {
      const Watch watch;
      STD_FUNCTION_TARGETS(strftime, watch._call_strftime);
   }

   TEST2X2(DateTimeNow_ReturnsLocalDateTimeNow,
      const tm& tmNow, const string& expectedDateTimeNowPrefix,
      Tm(0, 1, 0, 0, 0, 0), "1900-01-01 12:00:00 AM ",
      Tm(1, 2, 1, 1, 1, 1), "1901-02-02 01:01:01 AM ",
      Tm(2, 3, 2, 11, 11, 11), "1902-03-03 11:11:11 AM ",
      Tm(11, 31, 99, 23, 59, 59), "1999-12-31 11:59:59 PM ",
      Tm(0, 1, 100, 0, 0, 0), "2000-01-01 12:00:00 AM ",
      Tm(1, 3, 101, 4, 5, 6), "2001-02-03 04:05:06 AM ")
   {
      _watchSelfMocked->TMNowMock.Return(tmNow);
      const string timeZone = _watchSelfMocked->GetTimeZoneMock.ReturnRandom();
      //
      const string dateTimeNow = _watchSelfMocked->DateTimeNow();
      //
      ZENMOCK(_watchSelfMocked->TMNowMock.CalledOnce());
      ZENMOCK(_watchSelfMocked->GetTimeZoneMock.CalledOnceWith(tmNow));
      const string expectedDateTimeNow = expectedDateTimeNowPrefix + timeZone;
      ARE_EQUAL(expectedDateTimeNow, dateTimeNow);
   }

   static constexpr tm Tm(int tmMonth, int tmMonthDay, int tmYear, int tmHour, int tmMin, int tmSec)
   {
      tm tmNow{};
      tmNow.tm_sec = tmSec;
      tmNow.tm_min = tmMin;
      tmNow.tm_hour = tmHour;
      tmNow.tm_mday = tmMonthDay;
      tmNow.tm_mon = tmMonth;
      tmNow.tm_year = tmYear;
      tmNow.tm_wday = 0;
      tmNow.tm_yday = 0;
      tmNow.tm_isdst = 0;
      return tmNow;
   }

   TEST(GetTimeZone_ReturnTimeZoneOfExecutingProcess)
   {
      Watch watch;
      const tm tmNow = watch.TMNow();
      //
      const string timeZone = watch.GetTimeZone(tmNow);
      //
      char expectedTimeZoneChars[64];
      strftime(expectedTimeZoneChars, sizeof(expectedTimeZoneChars), "%Z", &tmNow);
      const string expectedTimeZone(expectedTimeZoneChars);
      ARE_EQUAL(expectedTimeZone, timeZone);
   }

   TEST2X2(MicrosecondsToTwoDecimalPlaceMillisecondsString_ReturnsMicrosecondsAsMillisecondsRoundedToThreePlaces,
      unsigned microseconds, const char* expectedReturnValue,
      0, "[0.00 ms]",
      1, "[0.00 ms]",
      2, "[0.00 ms]",
      10, "[0.01 ms]",
      12, "[0.01 ms]",
      100, "[0.10 ms]",
      120, "[0.12 ms]",
      123, "[0.12 ms]",
      1000, "[1.00 ms]",
      1234, "[1.23 ms]",
      12345, "[12.35 ms]",
      123456, "[123.46 ms]",
      1234567, "[1234.57 ms]",
      12345678, "[12345.68 ms]",
      123456789, "[123456.79 ms]",
      1234567890, "[1234567.89 ms]")
   {
      const string twoDecimalPlaceMillisecondsString = Watch::MicrosecondsToTwoDecimalPlaceMillisecondsString(microseconds);
      ARE_EQUAL(expectedReturnValue, twoDecimalPlaceMillisecondsString);
   }

   RUN_TESTS(WatchTests)
}
