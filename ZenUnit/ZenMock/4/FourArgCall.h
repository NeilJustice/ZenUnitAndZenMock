#pragma once
#include "ZenUnit/Macros/ARE_EQUAL.h"
#include "ZenUnit/ToStringer/ToStringer.h"

namespace ZenMock
{
   template<typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
   struct FourArgCall
   {
      typename std::decay<Arg1Type>::type arg1;
      typename std::decay<Arg2Type>::type arg2;
      typename std::decay<Arg3Type>::type arg3;
      typename std::decay<Arg4Type>::type arg4;

      FourArgCall()
         : arg1()
         , arg2()
         , arg3()
         , arg4()
      {
      }

      FourArgCall(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3, Arg4Type arg4)
         : arg1(std::move(arg1))
         , arg2(std::move(arg2))
         , arg3(std::move(arg3))
         , arg4(std::move(arg4))
      {
      }
   };
}

template<typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
struct ZenUnitEqualizer<ZenMock::FourArgCall<Arg1Type, Arg2Type, Arg3Type, Arg4Type>>
{
   static void AssertEqual(
      const ZenMock::FourArgCall<Arg1Type, Arg2Type, Arg3Type, Arg4Type>& expectedFourArgCall,
      const ZenMock::FourArgCall<Arg1Type, Arg2Type, Arg3Type, Arg4Type>& actualFourArgCall)
    {
       ARE_EQUAL(expectedFourArgCall.arg1, actualFourArgCall.arg1);
       ARE_EQUAL(expectedFourArgCall.arg2, actualFourArgCall.arg2);
       ARE_EQUAL(expectedFourArgCall.arg3, actualFourArgCall.arg3);
       ARE_EQUAL(expectedFourArgCall.arg4, actualFourArgCall.arg4);
    }
};
