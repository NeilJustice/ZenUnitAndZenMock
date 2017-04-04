#pragma once
#include "ZenUnit/Macros/ARE_EQUAL.h"
#include "ZenUnit/ToStringer/ToStringer.h"

namespace ZenMock
{
   template<typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type, typename Arg5Type, typename Arg6Type, typename Arg7Type>
   struct SevenArgCall
   {
      typename std::decay<Arg1Type>::type arg1;
      typename std::decay<Arg2Type>::type arg2;
      typename std::decay<Arg3Type>::type arg3;
      typename std::decay<Arg4Type>::type arg4;
      typename std::decay<Arg5Type>::type arg5;
      typename std::decay<Arg6Type>::type arg6;
      typename std::decay<Arg6Type>::type arg7;

      SevenArgCall()
         : arg1()
         , arg2()
         , arg3()
         , arg4()
         , arg5()
         , arg6()
         , arg7()
      {
      }

      SevenArgCall(
         Arg1Type arg1,
         Arg2Type arg2,
         Arg3Type arg3,
         Arg4Type arg4,
         Arg5Type arg5,
         Arg6Type arg6,
         Arg7Type arg7)
         : arg1(std::move(arg1))
         , arg2(std::move(arg2))
         , arg3(std::move(arg3))
         , arg4(std::move(arg4))
         , arg5(std::move(arg5))
         , arg6(std::move(arg6))
         , arg7(std::move(arg7))
      {
      }
   };
}

template<typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type, typename Arg5Type, typename Arg6Type, typename Arg7Type>
struct ZenUnitEqualizer<ZenMock::SevenArgCall<Arg1Type, Arg2Type, Arg3Type, Arg4Type, Arg5Type, Arg6Type, Arg7Type>>
{
   static void AssertEqual(
      const ZenMock::SevenArgCall<Arg1Type, Arg2Type, Arg3Type, Arg4Type, Arg5Type, Arg6Type, Arg7Type>& expectedSevenArgCall,
      const ZenMock::SevenArgCall<Arg1Type, Arg2Type, Arg3Type, Arg4Type, Arg5Type, Arg6Type, Arg7Type>& actualSevenArgCall)
   {
      ARE_EQUAL(expectedSevenArgCall.arg1, actualSevenArgCall.arg1);
      ARE_EQUAL(expectedSevenArgCall.arg2, actualSevenArgCall.arg2);
      ARE_EQUAL(expectedSevenArgCall.arg3, actualSevenArgCall.arg3);
      ARE_EQUAL(expectedSevenArgCall.arg4, actualSevenArgCall.arg4);
      ARE_EQUAL(expectedSevenArgCall.arg5, actualSevenArgCall.arg5);
      ARE_EQUAL(expectedSevenArgCall.arg6, actualSevenArgCall.arg6);
      ARE_EQUAL(expectedSevenArgCall.arg7, actualSevenArgCall.arg7);
   }
};
