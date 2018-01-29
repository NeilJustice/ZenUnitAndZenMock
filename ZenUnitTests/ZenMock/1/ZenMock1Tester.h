#pragma once
#include "ZenUnitTests/ZenMock/ZenMockTestUtil.h"

namespace ZenMock
{
   template<
      typename ZenMockObjectType,
      typename FreeMockType,
      typename NamespaceMockType,
      typename StaticMockType,
      typename StaticNameClashMockType>
   class ZenMock1Tester
   {
   private:
      ZenMockObjectType mock;

      const string virtualSignature;
      const string virtualConstSignature;
      const string nonVirtualSignature;
      const string nonVirtualConstSignature;

      FreeMockType freeMock;
      const string freeSignature;

      NamespaceMockType namespaceMock;
      const string namespaceSignature;

      StaticMockType staticMock;
      const string staticSignature;

      StaticNameClashMockType staticNameClashMock;
      const string staticNameClashSignature;
   public:
      ZenMock1Tester(
         ZenMockObjectType mock,
         string virtualSignature,
         string virtualConstSignature,
         string nonVirtualSignature,
         string nonVirtualConstSignature,
         FreeMockType freeMock,
         string freeSignature,
         NamespaceMockType namespaceMock,
         string namespaceSignature,
         StaticMockType staticMock,
         string staticSignature,
         StaticNameClashMockType staticNameClashMock,
         string staticNameClashSignature)
         : mock(move(mock))
         , virtualSignature(move(virtualSignature))
         , virtualConstSignature(move(virtualConstSignature))
         , nonVirtualSignature(move(nonVirtualSignature))
         , nonVirtualConstSignature(move(nonVirtualConstSignature))
         , freeMock(move(freeMock))
         , freeSignature(move(freeSignature))
         , namespaceMock(move(namespaceMock))
         , namespaceSignature(move(namespaceSignature))
         , staticMock(move(staticMock))
         , staticSignature(move(staticSignature))
         , staticNameClashMock(move(staticNameClashMock))
         , staticNameClashSignature(move(staticNameClashSignature))
      {
      }

      void Throw_CalledTwice_Throws()
      {
         const auto test = [](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject.template Throw<exception>();
            THROWS(zenMockObject.template Throw<exception>(), FunctionAlreadyExpectedException,
               FunctionAlreadyExpectedException::MakeWhat(expectedSignature));
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(freeMock, freeSignature);
         test(namespaceMock, namespaceSignature);
         test(staticMock, staticSignature);
         test(staticNameClashMock, staticNameClashSignature);
      }

      void MockedFunction_NotExpected_Throws()
      {
         const auto test = [](auto functionCallLambda, const string& expectedSignature)
         {
            THROWS(functionCallLambda(), UnexpectedCallException,
               UnexpectedCallException::MakeWhat(expectedSignature, 0));
         };
         test([&] { mock.Virtual(0); }, virtualSignature);
         test([&] { mock.VirtualConst(0); }, virtualConstSignature);
         test([&] { mock.NonVirtual(0); }, nonVirtualSignature);
         test([&] { mock.NonVirtualConst(0); }, nonVirtualConstSignature);
         function<void(int)> zenBoundFreeMock = ZENMOCK_BIND1(freeMock);
         test([&] { zenBoundFreeMock(0); }, freeSignature);
         function<void(int)> zenBoundNamespaceMock = ZENMOCK_BIND1(namespaceMock);
         test([&] { zenBoundNamespaceMock(0); }, namespaceSignature);
         function<void(int)> zenBoundStaticMock = ZENMOCK_BIND1(staticMock);
         test([&] { zenBoundStaticMock(0); }, staticSignature);
         function<void(int)> zenBoundStaticNameClashMock = ZENMOCK_BIND1(staticNameClashMock);
         test([&] { zenBoundStaticNameClashMock(0); }, staticNameClashSignature);
      }

      void MockedFunction_Expected_DoesNotThrow()
      {
         const auto test = [](auto& zenMockObject)
         {
            zenMockObject._expected = true;
            zenMockObject.ZenMockIt(0);
            zenMockObject.CalledOnceWith(0);
         };
         test(mock.VirtualMock);
         test(mock.VirtualConstMock);
         test(mock.NonVirtualMock);
         test(mock.NonVirtualConstMock);
         test(freeMock);
         test(namespaceMock);
         test(staticMock);
         test(staticNameClashMock);
      }

      const string What = "what";
      void Throw_ThenFunction_ThrowsTheException()
      {
         const auto assertCalledOnce = [](auto& zenMockObject)
         {
            zenMockObject.CalledOnceWith(0);
            zenMockObject.CalledNTimesWith(1, 0);
         };

         mock.VirtualMock.template Throw<runtime_error>(What);
         THROWS(mock.Virtual(0), runtime_error, What);
         assertCalledOnce(mock.VirtualMock);

         mock.VirtualConstMock.template Throw<runtime_error>(What);
         THROWS(mock.VirtualConst(0), runtime_error, What);
         assertCalledOnce(mock.VirtualConstMock);

         mock.NonVirtualMock.template Throw<runtime_error>(What);
         THROWS(mock.NonVirtual(0), runtime_error, What);
         assertCalledOnce(mock.NonVirtualMock);

         mock.NonVirtualConstMock.template Throw<runtime_error>(What);
         THROWS(mock.NonVirtualConst(0), runtime_error, What);
         assertCalledOnce(mock.NonVirtualConstMock);

         function<void(int)> zenBoundFreeVoid0 = ZENMOCK_BIND1(freeMock);
         freeMock.template Throw<runtime_error>(What);
         THROWS(zenBoundFreeVoid0(0), runtime_error, What);
         assertCalledOnce(freeMock);

         function<void(int)> zenBoundNamespaceVoid0 = ZENMOCK_BIND1(namespaceMock);
         namespaceMock.template Throw<runtime_error>(What);
         THROWS(zenBoundNamespaceVoid0(0), runtime_error, What);
         assertCalledOnce(namespaceMock);

         function<void(int)> zenBoundStatic = ZENMOCK_BIND1(staticMock);
         staticMock.template Throw<runtime_error>(What);
         THROWS(zenBoundStatic(0), runtime_error, What);
         assertCalledOnce(staticMock);

         function<void(int)> zenBoundStaticVoid0 = ZENMOCK_BIND1(staticNameClashMock);
         staticNameClashMock.template Throw<runtime_error>(What);
         THROWS(zenBoundStaticVoid0(0), runtime_error, What);
         assertCalledOnce(staticNameClashMock);
      }

      void FunctionNotCalled_CalledOnceWithThrows_CalledAsFollowsWithThrows()
      {
         const auto test = [](auto& zenMockObject, const string& expectedSignature)
         {
            THROWS(zenMockObject.CalledOnceWith(0), Anomaly, R"(
  Failed: ARE_EQUAL(expectedNumberOfCalls, actualNumberOfCalls, this->ZenMockedFunctionSignature)
Expected: 1
  Actual: 0
 Message: ")" + expectedSignature + R"("
File.cpp(1))");

            THROWS(zenMockObject.CalledNTimesWith(1, 0), Anomaly, R"(
  Failed: ARE_EQUAL(expectedNumberOfCalls, actualNumberOfCalls, this->ZenMockedFunctionSignature)
Expected: 1
  Actual: 0
 Message: ")" + expectedSignature + R"("
File.cpp(1))");

            THROWS(zenMockObject.CalledNTimesWith(2, 0), Anomaly, R"(
  Failed: ARE_EQUAL(expectedNumberOfCalls, actualNumberOfCalls, this->ZenMockedFunctionSignature)
Expected: 2
  Actual: 0
 Message: ")" + expectedSignature + R"("
File.cpp(1))");
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(freeMock, freeSignature);
         test(namespaceMock, namespaceSignature);
         test(staticMock, staticSignature);
         test(staticNameClashMock, staticNameClashSignature);
      }

      void Function_ExpectedFunctionCalledOnceThenTwice_CalledAsFollowsOnceWithDoesNotThrow_CalledAsFollowsWithDoesNotThrow()
      {
         const auto assertAfterFirstCall = [](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject.CalledOnceWith(0);
            zenMockObject.CalledNTimesWith(1, 0);
            THROWS(zenMockObject.CalledNTimesWith(2, 0), Anomaly,
               ZenMockTestUtil::ExpectedCallCountMismatchWhat(expectedSignature, 2, 1));
         };
         const auto assertAfterSecondCall = [](auto& zenMockObject, const string& expectedSignature)
         {
            THROWS(zenMockObject.CalledOnceWith(0), Anomaly,
               ZenMockTestUtil::ExpectedCallCountMismatchWhat(expectedSignature, 1, 2));
            zenMockObject.CalledNTimesWith(2, 0);
            THROWS(zenMockObject.CalledNTimesWith(3, 0), Anomaly,
               ZenMockTestUtil::ExpectedCallCountMismatchWhat(expectedSignature, 3, 2));
         };

         mock.VirtualMock.Expect();
         mock.Virtual(0);
         assertAfterFirstCall(mock.VirtualMock, virtualSignature);
         mock.Virtual(0);
         assertAfterSecondCall(mock.VirtualMock, virtualSignature);

         mock.VirtualConstMock.Expect();
         mock.VirtualConst(0);
         assertAfterFirstCall(mock.VirtualConstMock, virtualConstSignature);
         mock.VirtualConst(0);
         assertAfterSecondCall(mock.VirtualConstMock, virtualConstSignature);

         mock.NonVirtualMock.Expect();
         mock.NonVirtual(0);
         assertAfterFirstCall(mock.NonVirtualMock, nonVirtualSignature);
         mock.NonVirtual(0);
         assertAfterSecondCall(mock.NonVirtualMock, nonVirtualSignature);

         mock.NonVirtualConstMock.Expect();
         mock.NonVirtualConst(0);
         assertAfterFirstCall(mock.NonVirtualConstMock, nonVirtualConstSignature);
         mock.NonVirtualConst(0);
         assertAfterSecondCall(mock.NonVirtualConstMock, nonVirtualConstSignature);

         const function<void(int)> zenBoundFreeMock = ZENMOCK_BIND1(freeMock);
         freeMock.Expect();
         zenBoundFreeMock(0);
         assertAfterFirstCall(freeMock, freeSignature);
         zenBoundFreeMock(0);
         assertAfterSecondCall(freeMock, freeSignature);

         const function<void(int)> zenBoundNamespaceMock = ZENMOCK_BIND1(namespaceMock);
         namespaceMock.Expect();
         zenBoundNamespaceMock(0);
         assertAfterFirstCall(namespaceMock, namespaceSignature);
         zenBoundNamespaceMock(0);
         assertAfterSecondCall(namespaceMock, namespaceSignature);

         const function<void(int)> zenBoundStaticMock = ZENMOCK_BIND1(staticMock);
         staticMock.Expect();
         zenBoundStaticMock(0);
         assertAfterFirstCall(staticMock, staticSignature);
         zenBoundStaticMock(0);
         assertAfterSecondCall(staticMock, staticSignature);

         const function<void(int)> zenBoundStaticNameClashMock = ZENMOCK_BIND1(staticNameClashMock);
         staticNameClashMock.Expect();
         zenBoundStaticNameClashMock(0);
         assertAfterFirstCall(staticNameClashMock, staticNameClashSignature);
         zenBoundStaticNameClashMock(0);
         assertAfterSecondCall(staticNameClashMock, staticNameClashSignature);
      }

      // One Arg Tests

      void CalledOnceWith_ExpectedFunctionCalled0Or2OrMoreTimes_Throws(size_t numberOfCalls)
      {
         const auto test = [&](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject._expected = true;
            for (size_t i = 0; i < numberOfCalls; ++i)
            {
               zenMockObject.ZenMockIt(0);
            }
            const string expectedWhat = String::Concat(R"(
  Failed: ARE_EQUAL(expectedNumberOfCalls, callHistory.size(), this->ZenMockedFunctionSignature)
Expected: 1
  Actual: )", numberOfCalls, R"(
 Message: ")", expectedSignature, R"("
File.cpp(1))");
            THROWS(zenMockObject.CalledOnceWith(0), Anomaly, expectedWhat);
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(freeMock, freeSignature);
         test(namespaceMock, namespaceSignature);
         test(staticMock, staticSignature);
         test(staticNameClashMock, staticNameClashSignature);
      }

      void CalledOnceWith_ExpectedFunctionCalledOnceWithMistmatchingArg_Throws()
      {
         const auto test = [](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject._expected = true;
            //
            zenMockObject.ZenMockIt(10);
            //
            const string expectedWhat = String::Concat(R"(
  Failed: ARE_EQUAL(expectedArgument, callHistory[0].argument, this->ZenMockedFunctionSignature)
Expected: 20
  Actual: 10
 Message: ")", expectedSignature, R"("
File.cpp(1))");
            THROWS(zenMockObject.CalledOnceWith(20), Anomaly, expectedWhat);
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(freeMock, freeSignature);
         test(namespaceMock, namespaceSignature);
         test(staticMock, staticSignature);
         test(staticNameClashMock, staticNameClashSignature);
      }

      void CalledOnceWith_ExpectedFunctionCalledOnceWithMatchingArg_DoesNotThrow()
      {
         const auto test = [](auto& zenMockObject)
         {
            zenMockObject._expected = true;
            //
            zenMockObject.ZenMockIt(10);
            //
            zenMockObject.CalledOnceWith(10);
         };
         test(mock.VirtualMock);
         test(mock.VirtualConstMock);
         test(mock.NonVirtualMock);
         test(mock.NonVirtualConstMock);
         test(freeMock);
         test(namespaceMock);
         test(staticMock);
         test(staticNameClashMock);
      }

      void CalledAsFollowsWith_N0_Throws()
      {
         const auto test = [](auto& zenMockObject, const string& expectedSignature)
         {
            THROWS(zenMockObject.CalledNTimesWith(0, 0), UnsupportedCalledZeroTimesException,
               UnsupportedCalledZeroTimesException::MakeWhat(expectedSignature));
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(freeMock, freeSignature);
         test(namespaceMock, namespaceSignature);
         test(staticMock, staticSignature);
         test(staticNameClashMock, staticNameClashSignature);
      }

      void CalledAsFollowsWith_N1OrGreater_FunctionCalledNotNTimes_Throws(size_t n, size_t numberOfCalls)
      {
         const auto test = [&](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject._expected = true;
            for (size_t i = 0; i < numberOfCalls; ++i)
            {
               zenMockObject.ZenMockIt(0);
            }
            const string expectedWhat = String::Concat(R"(
  Failed: ARE_EQUAL(expectedNumberOfCalls, callHistory.size(), this->ZenMockedFunctionSignature)
Expected: )", n, R"(
  Actual: )", numberOfCalls, R"(
 Message: ")", expectedSignature, R"("
File.cpp(1))");
            THROWS(zenMockObject.CalledNTimesWith(n, 123), Anomaly, expectedWhat);
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(freeMock, freeSignature);
         test(namespaceMock, namespaceSignature);
         test(staticMock, staticSignature);
         test(staticNameClashMock, staticNameClashSignature);
      }

      void CalledAsFollowsWith_N1OrGreater_FunctionCalledAsFollowsWithOneOfTheCallsMismatching_Throws(
         size_t n, size_t mismatchingCallIndex)
      {
         const auto test = [&](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject._expected = true;
            //
            for (size_t i = 0; i < n; ++i)
            {
               if (i == mismatchingCallIndex)
               {
                  zenMockObject.ZenMockIt(20);
               }
               else
               {
                  zenMockObject.ZenMockIt(10);
               }
            }
            //
            const string expectedWhat = String::Concat(R"(
  Failed: ARE_EQUAL(expectedArgument, callHistory[i].argument, zenMockedFunctionSignatureAndCallIndex)
Expected: 10
  Actual: 20
 Message: ")", expectedSignature, " at i=", mismatchingCallIndex, R"("
File.cpp(1))");
            THROWS(zenMockObject.CalledNTimesWith(n, 10), Anomaly, expectedWhat);
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(freeMock, freeSignature);
         test(namespaceMock, namespaceSignature);
         test(staticMock, staticSignature);
         test(staticNameClashMock, staticNameClashSignature);
      }

      void CalledAsFollowsWith_N1OrGreater_FunctionCalledAsFollowsWithMatchingArg_DoesNotThrow(size_t n)
      {
         const auto test = [&](auto& zenMockObject)
         {
            zenMockObject._expected = true;
            //
            for (size_t i = 0; i < n; ++i)
            {
               zenMockObject.ZenMockIt(10);
            }
            //
            zenMockObject.CalledNTimesWith(n, 10);
         };
         test(mock.VirtualMock);
         test(mock.VirtualConstMock);
         test(mock.NonVirtualMock);
         test(mock.NonVirtualConstMock);
         test(freeMock);
         test(namespaceMock);
         test(staticMock);
         test(staticNameClashMock);
      }

      void CalledAsFollows_EmptyCalls_Throws()
      {
         const auto test = [](auto& zenMockObject, const string& expectedSignature)
         {
            THROWS(zenMockObject.CalledAsFollows({}), UnsupportedCalledZeroTimesException,
               UnsupportedCalledZeroTimesException::MakeWhat(expectedSignature));
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(freeMock, freeSignature);
         test(namespaceMock, namespaceSignature);
         test(staticMock, staticSignature);
         test(staticNameClashMock, staticNameClashSignature);
      }

      void CalledAsFollows_NonEmptyCalls_FunctionCalledNotCallsSizeTimes_Throws(
         size_t expectedCallsSize, size_t numberOfCalls, const string& expectedExceptionWhatPrefix)
      {
         const auto test = [&](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject._expected = true;
            //
            ZenMockTestUtil::CallNTimes(numberOfCalls, [&] { zenMockObject.ZenMockIt(0); });
            //
            const string expectedExceptionWhat = String::Concat(
               expectedExceptionWhatPrefix, R"(
 Message: ")", expectedSignature, R"("
File.cpp(1)
File.cpp(1))");
            const int lvalue = 0;
            vector<OneArgumentCallRef<int>> expectedCalls;
            ZenMockTestUtil::CallNTimes(expectedCallsSize, [&] { expectedCalls.emplace_back(lvalue); });
            THROWS(zenMockObject.CalledAsFollows(expectedCalls), Anomaly, expectedExceptionWhat);
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(freeMock, freeSignature);
         test(namespaceMock, namespaceSignature);
         test(staticMock, staticSignature);
         test(staticNameClashMock, staticNameClashSignature);
      }

      void CalledAsFollows_NonEmptyCalls_FunctionCalledCallsSizeTimesWithOneOfTheCallsMismatching_Throws(
         size_t expectedCallsSize, size_t mismatchingCallIndex, const string& expectedExceptionWhatPrefix)
      {
         const auto test = [&](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject._expected = true;
            //
            for (size_t i = 0; i < expectedCallsSize; ++i)
            {
               if (i == mismatchingCallIndex)
               {
                  zenMockObject.ZenMockIt(20);
               }
               else
               {
                  zenMockObject.ZenMockIt(10);
               }
            }
            //
            const int expectedArgument = 10;
            vector<OneArgumentCallRef<int>> expectedCalls;
            ZenMockTestUtil::CallNTimes(expectedCallsSize, [&] { expectedCalls.emplace_back(expectedArgument); });
            const string expectedExceptionWhat = R"(
  Failed: VECTORS_EQUAL(expectedOneArgumentCalls, actualOneArgumentCalls, this->ZenMockedFunctionSignature))"
               + expectedExceptionWhatPrefix + R"(
 Message: ")" + expectedSignature + R"("
File.cpp(1)
File.cpp(1))";
            THROWS(zenMockObject.CalledAsFollows(expectedCalls), Anomaly, expectedExceptionWhat);
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(freeMock, freeSignature);
         test(namespaceMock, namespaceSignature);
         test(staticMock, staticSignature);
         test(staticNameClashMock, staticNameClashSignature);
      }

      void CalledAsFollows_NonEmptyCalls_FunctionCalledCallsSizeTimesMatchingArgs_DoesNotThrow(
         size_t expectedCallsSize)
      {
         const auto test = [&](auto& zenMockObject)
         {
            zenMockObject._expected = true;
            const int argument = ZenUnit::Random<int>();
            //
            ZenMockTestUtil::CallNTimes(expectedCallsSize, [&] { zenMockObject.ZenMockIt(argument); });
            //
            vector<OneArgumentCallRef<int>> expectedCalls;
            ZenMockTestUtil::CallNTimes(expectedCallsSize, [&] { expectedCalls.emplace_back(argument); });
            zenMockObject.CalledAsFollows(expectedCalls);
         };
         test(mock.VirtualMock);
         test(mock.VirtualConstMock);
         test(mock.NonVirtualMock);
         test(mock.NonVirtualConstMock);
         test(freeMock);
         test(namespaceMock);
         test(staticMock);
         test(staticNameClashMock);
      }
   };
}
