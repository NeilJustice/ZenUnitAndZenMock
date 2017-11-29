#pragma once
#include "ZenUnit/Macros/VECTORS_EQUAL.h"
#include "ZenUnit/ZenMock/7/SevenArgumentCall.h"
#include "ZenUnit/ZenMock/7/SevenArgumentCallRef.h"
#include "ZenUnit/ZenMock/Exceptions/UnexpectedCallException.h"
#include "ZenUnit/ZenMock/ZenMocker.h"

#define ZENMOCK_BIND7(FunctionName_ZenMock) \
   std::bind(&decltype(FunctionName_ZenMock)::ZenMockItFunctionPointer, &(FunctionName_ZenMock), \
   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, \
   std::placeholders::_5, std::placeholders::_6, std::placeholders::_7)

namespace ZenMock
{
   template<
      typename Arg1Type,
      typename Arg2Type,
      typename Arg3Type,
      typename Arg4Type,
      typename Arg5Type,
      typename Arg6Type,
      typename Arg7Type,
      typename MockableExceptionThrowerType = ExceptionThrower>
   class SevenArgumentMocker : public ZenMocker<MockableExceptionThrowerType>
   {
      friend class SevenArgumentMockerTests;
   private:
      std::vector<SevenArgumentCall<Arg1Type, Arg2Type, Arg3Type, Arg4Type, Arg5Type, Arg6Type, Arg7Type>> callHistory;
   public:
      explicit SevenArgumentMocker(const std::string& zenMockedFunctionSignature)
         : ZenMocker<MockableExceptionThrowerType>(zenMockedFunctionSignature)
      {
      }

      void ZenMockIt(
         const Arg1Type& firstArgument,
         const Arg2Type& secondArgument,
         const Arg3Type& thirdArgument,
         const Arg4Type& fourthArgument,
         const Arg5Type& fifthArgument,
         const Arg6Type& sixthArgument,
         const Arg7Type& seventhArgument)
      {
         this->ZenMockThrowIfNotExpected(firstArgument, secondArgument, thirdArgument, fourthArgument, fifthArgument, sixthArgument, seventhArgument);
         callHistory.emplace_back(firstArgument, secondArgument, thirdArgument, fourthArgument, fifthArgument, sixthArgument, seventhArgument);
         this->ZenMockThrowIfExceptionSet();
      }

      ZENAssertionToken AssertCalledOnceWith(
         const Arg1Type& expectedFirstArgument,
         const Arg2Type& expectedSecondArgument,
         const Arg3Type& expectedThirdArgument,
         const Arg4Type& expectedFourthArgument,
         const Arg5Type& expectedFifthArgument,
         const Arg6Type& expectedSixthArgument,
         const Arg7Type& expectedSeventhArgument)
      {
         this->ZenMockSetAsserted();
         const size_t expectedNumberOfCalls = 1;
         ARE_EQUAL(expectedNumberOfCalls, callHistory.size(), this->ZenMockedFunctionSignature);
         ARE_EQUAL(expectedFirstArgument, callHistory[0].firstArgument, this->ZenMockedFunctionSignature);
         ARE_EQUAL(expectedSecondArgument, callHistory[0].secondArgument, this->ZenMockedFunctionSignature);
         ARE_EQUAL(expectedThirdArgument, callHistory[0].thirdArgument, this->ZenMockedFunctionSignature);
         ARE_EQUAL(expectedFourthArgument, callHistory[0].fourthArgument, this->ZenMockedFunctionSignature);
         ARE_EQUAL(expectedFifthArgument, callHistory[0].fifthArgument, this->ZenMockedFunctionSignature);
         ARE_EQUAL(expectedSixthArgument, callHistory[0].sixthArgument, this->ZenMockedFunctionSignature);
         ARE_EQUAL(expectedSeventhArgument, callHistory[0].seventhArgument, this->ZenMockedFunctionSignature);
         return ZENAssertionToken::NoDiscard();
      }

      ZENAssertionToken AssertCalledNTimesWith(
         size_t expectedNumberOfCalls,
         const Arg1Type& expectedFirstArgument,
         const Arg2Type& expectedSecondArgument,
         const Arg3Type& expectedThirdArgument,
         const Arg4Type& expectedFourthArgument,
         const Arg5Type& expectedFifthArgument,
         const Arg6Type& expectedSixthArgument,
         const Arg7Type& expectedSeventhArgument)
      {
         this->ZenMockThrowIfExpectedNumberOfCalls0(expectedNumberOfCalls);
         this->ZenMockSetAsserted();
         ARE_EQUAL(expectedNumberOfCalls, callHistory.size(), this->ZenMockedFunctionSignature);
         for (size_t i = 0; i < expectedNumberOfCalls; ++i)
         {
            const std::string zenMockedFunctionSignatureAndCallIndex
               = ZenUnit::String::Concat(this->ZenMockedFunctionSignature, " at i=", i);
            ARE_EQUAL(expectedFirstArgument, callHistory[i].firstArgument, zenMockedFunctionSignatureAndCallIndex);
            ARE_EQUAL(expectedSecondArgument, callHistory[i].secondArgument, zenMockedFunctionSignatureAndCallIndex);
            ARE_EQUAL(expectedThirdArgument, callHistory[i].thirdArgument, zenMockedFunctionSignatureAndCallIndex);
            ARE_EQUAL(expectedFourthArgument, callHistory[i].fourthArgument, zenMockedFunctionSignatureAndCallIndex);
            ARE_EQUAL(expectedFifthArgument, callHistory[i].fifthArgument, zenMockedFunctionSignatureAndCallIndex);
            ARE_EQUAL(expectedSixthArgument, callHistory[i].sixthArgument, zenMockedFunctionSignatureAndCallIndex);
            ARE_EQUAL(expectedSeventhArgument, callHistory[i].seventhArgument, zenMockedFunctionSignatureAndCallIndex);
         }
         return ZENAssertionToken::NoDiscard();
      }

      ZENAssertionToken AssertCalls(const std::vector<SevenArgumentCallRef<
         Arg1Type, Arg2Type, Arg3Type, Arg4Type, Arg5Type, Arg6Type, Arg7Type>>& expectedSevenArgumentCalls)
      {
         this->ZenMockThrowIfExpectedCallsSizeIsZero(expectedSevenArgumentCalls.size());
         this->ZenMockSetAsserted();
         const std::vector<SevenArgumentCallRef<Arg1Type, Arg2Type, Arg3Type, Arg4Type, Arg5Type, Arg6Type, Arg7Type>>
            actualSevenArgumentCalls = PrivateCallsToCallRefs(callHistory);
         VECTORS_EQUAL(expectedSevenArgumentCalls, actualSevenArgumentCalls, this->ZenMockedFunctionSignature);
         return ZENAssertionToken::NoDiscard();
      }

   private:
      static std::vector<SevenArgumentCallRef<Arg1Type, Arg2Type, Arg3Type, Arg4Type, Arg5Type, Arg6Type, Arg7Type>>
         PrivateCallsToCallRefs(const std::vector<SevenArgumentCall<Arg1Type, Arg2Type, Arg3Type, Arg4Type, Arg5Type, Arg6Type, Arg7Type>>& callHistory)
      {
         std::vector<SevenArgumentCallRef<Arg1Type, Arg2Type, Arg3Type, Arg4Type, Arg5Type, Arg6Type, Arg7Type>> sevenArgumentCallRefs;
         sevenArgumentCallRefs.reserve(callHistory.size());
         std::for_each(callHistory.cbegin(), callHistory.cend(),
            [&](const SevenArgumentCall<Arg1Type, Arg2Type, Arg3Type, Arg4Type, Arg5Type, Arg6Type, Arg7Type>& sevenArgumentCall)
            {
               sevenArgumentCallRefs.emplace_back(sevenArgumentCall);
            });
         return sevenArgumentCallRefs;
      }
   };
}
