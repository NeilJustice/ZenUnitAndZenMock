#pragma once
#include "ZenMock/ZenMockTester.h"

namespace ZenMock
{
   template<
      typename ZenMockObjectType,
      typename GlobalMockType,
      typename NamespaceMockType,
      typename StaticNameClashMockType,
      typename StaticMockType>
   struct ZenMock1Tester
   {
      ZenMockObjectType mock;
      const string virtualSignature;
      const string virtualConstSignature;
      const string nonVirtualSignature;
      const string nonVirtualConstSignature;

      GlobalMockType globalMock;
      const string globalSignature;

      NamespaceMockType namespaceMock;
      const string namespaceSignature;

      StaticNameClashMockType staticNameClashMock;
      const string staticNameClashSignature;

      StaticMockType staticMock;
      const string staticSignature;

      ZenMock1Tester(
         const ZenMockObjectType& mock,
         const string& virtualSignature,
         const string& virtualConstSignature,
         const string& nonVirtualSignature,
         const string& nonVirtualConstSignature,
         const GlobalMockType& globalMock,
         const string& globalSignature,
         const NamespaceMockType& namespaceMock,
         const string& namespaceSignature,
         const StaticNameClashMockType& staticNameClashMock,
         const string& staticNameClashSignature,
         const StaticMockType& staticMock,
         const string& staticSignature)
         : mock(mock)
         , virtualSignature(virtualSignature)
         , virtualConstSignature(virtualConstSignature)
         , nonVirtualSignature(nonVirtualSignature)
         , nonVirtualConstSignature(nonVirtualConstSignature)
         , globalMock(globalMock)
         , globalSignature(globalSignature)
         , namespaceMock(namespaceMock)
         , namespaceSignature(namespaceSignature)
         , staticNameClashMock(staticNameClashMock)
         , staticNameClashSignature(staticNameClashSignature)
         , staticMock(staticMock)
         , staticSignature(staticSignature)
      {
      }

      void Expect_CalledTwice_Throws()
      {
         const auto test = [](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject.Expect();
            THROWS(zenMockObject.Expect(), FunctionAlreadyExpectedException,
               FunctionAlreadyExpectedException::MakeWhat(expectedSignature));
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(globalMock, globalSignature);
         test(namespaceMock, namespaceSignature);
         test(staticNameClashMock, staticNameClashSignature);
         test(staticMock, staticSignature);
      }

      void Function_NotExpected_Throws()
      {
         const auto test = [](auto functionCallLambda, const string& expectedSignature)
         {
            THROWS(functionCallLambda(), UnexpectedCallException,
            UnexpectedCallException::MakeWhat(expectedSignature, 0));
         };
         test([&]{ mock.Virtual(0); }, virtualSignature);
         test([&]{ mock.VirtualConst(0); }, virtualConstSignature);
         test([&]{ mock.NonVirtual(0); }, nonVirtualSignature);
         test([&]{ mock.NonVirtualConst(0); }, nonVirtualConstSignature);
         std::function<void(int)> zenBoundGlobalMock = ZENBIND1(globalMock);
         test([&]{ zenBoundGlobalMock(0); }, globalSignature);
         std::function<void(int)> zenBoundNamespaceMock = ZENBIND1(namespaceMock);
         test([&]{ zenBoundNamespaceMock(0); }, namespaceSignature);
         std::function<void(int)> zenBoundStaticNameClashMock = ZENBIND1(staticNameClashMock);
         test([&]{ zenBoundStaticNameClashMock(0); }, staticNameClashSignature);
         std::function<void(int)> zenBoundStaticMock = ZENBIND1(staticMock);
         test([&]{ zenBoundStaticMock(0); }, staticSignature);
      }

      void Function_Expected_DoesNotThrow()
      {
         const auto test = [](auto& zenMockObject)
         {
            zenMockObject.Expect();
            zenMockObject.PrivateZenMock(0);
            zenMockObject.AssertCalledOnceWith(0);
         };
         test(mock.VirtualMock);
         test(mock.VirtualConstMock);
         test(mock.NonVirtualMock);
         test(mock.NonVirtualConstMock);

         test(globalMock);
         test(namespaceMock);
         test(staticNameClashMock);
         test(staticMock);
      }

      void ExpectAndThrow_CalledTwice_Throws()
      {
         const auto test = [](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject.template ExpectAndThrow<exception>();
            THROWS(zenMockObject.template ExpectAndThrow<exception>(), FunctionAlreadyExpectedException,
               FunctionAlreadyExpectedException::MakeWhat(expectedSignature));
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(globalMock, globalSignature);
         test(namespaceMock, namespaceSignature);
         test(staticNameClashMock, staticNameClashSignature);
         test(staticMock, staticSignature);
      }

      const string What = "what";
      void ExpectAndThrow_ThenFunction_ThrowsTheException()
      {
         const auto assertCalledOnce = [](auto& zenMockObject)
         {
            zenMockObject.AssertCalledOnceWith(0);
            zenMockObject.AssertCalledNTimesWith(1, 0);
         };

         mock.VirtualMock.template ExpectAndThrow<runtime_error>(What);
         THROWS(mock.Virtual(0), runtime_error, What);
         assertCalledOnce(mock.VirtualMock);

         mock.VirtualConstMock.template ExpectAndThrow<runtime_error>(What);
         THROWS(mock.VirtualConst(0), runtime_error, What);
         assertCalledOnce(mock.VirtualConstMock);

         mock.NonVirtualMock.template ExpectAndThrow<runtime_error>(What);
         THROWS(mock.NonVirtual(0), runtime_error, What);
         assertCalledOnce(mock.NonVirtualMock);

         mock.NonVirtualConstMock.template ExpectAndThrow<runtime_error>(What);
         THROWS(mock.NonVirtualConst(0), runtime_error, What);
         assertCalledOnce(mock.NonVirtualConstMock);

         std::function<void(int)> zenBoundGlobalVoid0 = ZENBIND1(globalMock);
         globalMock.template ExpectAndThrow<runtime_error>(What);
         THROWS(zenBoundGlobalVoid0(0), runtime_error, What);
         assertCalledOnce(globalMock);

         std::function<void(int)> zenBoundNamespaceVoid0 = ZENBIND1(namespaceMock);
         namespaceMock.template ExpectAndThrow<runtime_error>(What);
         THROWS(zenBoundNamespaceVoid0(0), runtime_error, What);
         assertCalledOnce(namespaceMock);

         std::function<void(int)> zenBoundStaticVoid0 = ZENBIND1(staticNameClashMock);
         staticNameClashMock.template ExpectAndThrow<runtime_error>(What);
         THROWS(zenBoundStaticVoid0(0), runtime_error, What);
         assertCalledOnce(staticNameClashMock);

         std::function<void(int)> zenBoundStatic = ZENBIND1(staticMock);
         staticMock.template ExpectAndThrow<runtime_error>(What);
         THROWS(zenBoundStatic(0), runtime_error, What);
         assertCalledOnce(staticMock);
      }

      void FunctionNotCalled_AssertCalledOnceWithThrows_AssertCalledNTimesWithThrows()
      {
         const auto test = [](auto& zenMockObject, const string& expectedSignature)
         {
            THROWS(zenMockObject.AssertCalledOnceWith(0), Anomaly, R"(
  Failed: ARE_EQUAL(expectedNumberOfCalls, numberOfCalls, this->ZenMockedFunctionSignature)
Expected: 1
  Actual: 0
 Message: ")" + expectedSignature + R"("
File.cpp(1))");

            THROWS(zenMockObject.AssertCalledNTimesWith(1, 0), Anomaly, R"(
  Failed: ARE_EQUAL(expectedNumberOfCalls, numberOfCalls, this->ZenMockedFunctionSignature)
Expected: 1
  Actual: 0
 Message: ")" + expectedSignature + R"("
File.cpp(1))");

            THROWS(zenMockObject.AssertCalledNTimesWith(2, 0), Anomaly, R"(
  Failed: ARE_EQUAL(expectedNumberOfCalls, numberOfCalls, this->ZenMockedFunctionSignature)
Expected: 2
  Actual: 0
 Message: ")" + expectedSignature + R"("
File.cpp(1))");
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(globalMock, globalSignature);
         test(namespaceMock, namespaceSignature);
         test(staticNameClashMock, staticNameClashSignature);
         test(staticMock, staticSignature);
      }

      void Function_ExpectedFunctionCalledOnceThenTwice_AssertCallsOnceWithDoesNotThrow_AssertCalledNTimesWithDoesNotThrow()
      {
         const auto assertAfterFirstCall = [](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject.AssertCalledOnceWith(0);
            zenMockObject.AssertCalledNTimesWith(1, 0);
            THROWS(zenMockObject.AssertCalledNTimesWith(2, 0), Anomaly,
               ZenMockTester::ExpectedCallCountMismatchWhat(expectedSignature, 2, 1));
         };
         const auto assertAfterSecondCall = [](auto& zenMockObject, const string& expectedSignature)
         {
            THROWS(zenMockObject.AssertCalledOnceWith(0), Anomaly,
               ZenMockTester::ExpectedCallCountMismatchWhat(expectedSignature, 1, 2));
            zenMockObject.AssertCalledNTimesWith(2, 0);
            THROWS(zenMockObject.AssertCalledNTimesWith(3, 0), Anomaly,
               ZenMockTester::ExpectedCallCountMismatchWhat(expectedSignature, 3, 2));
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

         const std::function<void(int)> zenBoundGlobalMock = ZENBIND1(globalMock);
         globalMock.Expect();
         zenBoundGlobalMock(0);
         assertAfterFirstCall(globalMock, globalSignature);
         zenBoundGlobalMock(0);
         assertAfterSecondCall(globalMock, globalSignature);

         const std::function<void(int)> zenBoundNamespaceMock = ZENBIND1(namespaceMock);
         namespaceMock.Expect();
         zenBoundNamespaceMock(0);
         assertAfterFirstCall(namespaceMock, namespaceSignature);
         zenBoundNamespaceMock(0);
         assertAfterSecondCall(namespaceMock, namespaceSignature);

         const std::function<void(int)> zenBoundStaticNameClashMock = ZENBIND1(staticNameClashMock);
         staticNameClashMock.Expect();
         zenBoundStaticNameClashMock(0);
         assertAfterFirstCall(staticNameClashMock, staticNameClashSignature);
         zenBoundStaticNameClashMock(0);
         assertAfterSecondCall(staticNameClashMock, staticNameClashSignature);

         const std::function<void(int)> zenBoundStaticMock = ZENBIND1(staticMock);
         staticMock.Expect();
         zenBoundStaticMock(0);
         assertAfterFirstCall(staticMock, staticSignature);
         zenBoundStaticMock(0);
         assertAfterSecondCall(staticMock, staticSignature);
      }

      // One Arg Tests

      void AssertCalledOnceWith_ExpectedFunctionCalled0Or2OrMoreTimes_Throws(size_t numberOfCalls)
      {
         const auto test = [&](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject.Expect();
            for (size_t i = 0; i < numberOfCalls; ++i)
            {
               zenMockObject.PrivateZenMock(0);
            }
           const string expectedWhat = String::Concat(R"(
  Failed: ARE_EQUAL(expectedNumberOfCalls, numberOfCalls, this->ZenMockedFunctionSignature)
Expected: 1
  Actual: )", numberOfCalls, R"(
 Message: ")", expectedSignature, R"("
File.cpp(1))");
            THROWS(zenMockObject.AssertCalledOnceWith(0), Anomaly, expectedWhat);
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(globalMock, globalSignature);
         test(namespaceMock, namespaceSignature);
         test(staticNameClashMock, staticNameClashSignature);
         test(staticMock, staticSignature);
      }

      void AssertCalledOnceWith_ExpectedFunctionCalledOnceWithMistmatchingArg_Throws()
      {
         const auto test = [](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject.Expect();
            //
            zenMockObject.PrivateZenMock(10);
            //
            const string expectedWhat = String::Concat(R"(
  Failed: ARE_EQUAL(expectedArg, oneArgCalls[0].arg, this->ZenMockedFunctionSignature)
Expected: 20
  Actual: 10
 Message: ")", expectedSignature, R"("
File.cpp(1))");
            THROWS(zenMockObject.AssertCalledOnceWith(20), Anomaly, expectedWhat);
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(globalMock, globalSignature);
         test(namespaceMock, namespaceSignature);
         test(staticNameClashMock, staticNameClashSignature);
         test(staticMock, staticSignature);
      }

      void AssertCalledOnceWith_ExpectedFunctionCalledOnceWithMatchingArg_DoesNotThrow()
      {
         const auto test = [](auto& zenMockObject)
         {
            zenMockObject.Expect();
            //
            zenMockObject.PrivateZenMock(10);
            //
            zenMockObject.AssertCalledOnceWith(10);
         };
         test(mock.VirtualMock);
         test(mock.VirtualConstMock);
         test(mock.NonVirtualMock);
         test(mock.NonVirtualConstMock);
         test(globalMock);
         test(namespaceMock);
         test(staticNameClashMock);
         test(staticMock);
      }

      void AssertCalledNTimesWith_N0_Throws()
      {
         const auto test = [](auto& zenMockObject, const string& expectedSignature)
         {
            THROWS(zenMockObject.AssertCalledNTimesWith(0, 0), UnsupportedAssertCalledZeroTimesException,
               UnsupportedAssertCalledZeroTimesException::MakeWhat(expectedSignature));
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(globalMock, globalSignature);
         test(namespaceMock, namespaceSignature);
         test(staticNameClashMock, staticNameClashSignature);
         test(staticMock, staticSignature);
      }

      void AssertCalledNTimesWith_N1OrGreater_FunctionCalledNotNTimes_Throws(size_t n, size_t numberOfCalls)
      {
         const auto test = [&](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject.Expect();
            for (size_t i = 0; i < numberOfCalls; ++i)
            {
               zenMockObject.PrivateZenMock(0);
            }
            string expectedWhat = String::Concat(R"(
  Failed: ARE_EQUAL(expectedNumberOfCalls, numberOfCalls, this->ZenMockedFunctionSignature)
Expected: )", n, R"(
  Actual: )", numberOfCalls, R"(
 Message: ")", expectedSignature, R"("
File.cpp(1))");
            THROWS(zenMockObject.AssertCalledNTimesWith(n, 123), Anomaly, expectedWhat);
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(globalMock, globalSignature);
         test(namespaceMock, namespaceSignature);
         test(staticNameClashMock, staticNameClashSignature);
         test(staticMock, staticSignature);
      }

      void AssertCalledNTimesWith_N1OrGreater_FunctionCalledNTimesWithOneOfTheCallsMismatching_Throws(
         size_t n, size_t mismatchingCallIndex)
      {
         const auto test = [&](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject.Expect();
            //
            for (size_t i = 0; i < n; ++i)
            {
               if (i == mismatchingCallIndex)
               {
                  zenMockObject.PrivateZenMock(20);
               }
               else
               {
                  zenMockObject.PrivateZenMock(10);
               }
            }
            //
            const string expectedWhat = String::Concat(R"(
  Failed: ARE_EQUAL(expectedArg, oneArgCalls[i].arg, zenMockedFunctionSignatureAndCallIndex)
Expected: 10
  Actual: 20
 Message: ")", expectedSignature, " at i=", mismatchingCallIndex, R"("
File.cpp(1))");
            THROWS(zenMockObject.AssertCalledNTimesWith(n, 10), Anomaly, expectedWhat);
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(globalMock, globalSignature);
         test(namespaceMock, namespaceSignature);
         test(staticNameClashMock, staticNameClashSignature);
         test(staticMock, staticSignature);
      }

      void AssertCalledNTimesWith_N1OrGreater_FunctionCalledNTimesWithMatchingArg_DoesNotThrow(size_t n)
      {
         const auto test = [&](auto& zenMockObject)
         {
            zenMockObject.Expect();
            //
            for (size_t i = 0; i < n; ++i)
            {
               zenMockObject.PrivateZenMock(10);
            }
            //
            zenMockObject.AssertCalledNTimesWith(n, 10);
         };
         test(mock.VirtualMock);
         test(mock.VirtualConstMock);
         test(mock.NonVirtualMock);
         test(mock.NonVirtualConstMock);
         test(globalMock);
         test(namespaceMock);
         test(staticNameClashMock);
         test(staticMock);
      }

      void AssertCalls_EmptyCalls_Throws()
      {
         const auto test = [](auto& zenMockObject, const string& expectedSignature)
         {
            THROWS(zenMockObject.AssertCalls({}), UnsupportedAssertCalledZeroTimesException,
               UnsupportedAssertCalledZeroTimesException::MakeWhat(expectedSignature));
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(globalMock, globalSignature);
         test(namespaceMock, namespaceSignature);
         test(staticNameClashMock, staticNameClashSignature);
         test(staticMock, staticSignature);
      }

      void AssertCalls_NonEmptyCalls_FunctionCalledNotCallsSizeTimes_Throws(
         size_t expectedCallsSize, size_t numberOfCalls)
      {
         const auto test = [&](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject.Expect();
            //
            ZenMockTester::call_n_times(numberOfCalls, [&]{ zenMockObject.PrivateZenMock(0); });
            //
            const string expectedWhat = String::Concat(R"(
  Failed: VECTORS_EQUAL(expectedOneArgCalls, actualOneArgCalls, this->ZenMockedFunctionSignature)
Expected: vector<T>
  Actual: vector<T>
 Because: ARE_EQUAL(expectedVector.size(), actualVector.size()) failed
Expected: )", expectedCallsSize, R"(
  Actual: )", numberOfCalls, R"(
 Message: ")", expectedSignature, R"("
File.cpp(1)
File.cpp(1))");
            int lvalue = 0;
            vector<OneArgCallRef<int>> expectedCalls;
            ZenMockTester::call_n_times(expectedCallsSize, [&]{ expectedCalls.emplace_back(lvalue); });
            THROWS(zenMockObject.AssertCalls(expectedCalls), Anomaly, expectedWhat);
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(globalMock, globalSignature);
         test(namespaceMock, namespaceSignature);
         test(staticNameClashMock, staticNameClashSignature);
         test(staticMock, staticSignature);
      }

      void AssertCalls_NonEmptyCalls_FunctionCalledCallsSizeTimesWithOneOfTheCallsMismatching_Throws(
         size_t expectedCallsSize, size_t mismatchingCallIndex)
      {
         const auto test = [&](auto& zenMockObject, const string& expectedSignature)
         {
            zenMockObject.Expect();
            //
            for (size_t i = 0; i < expectedCallsSize; ++i)
            {
               if (i == mismatchingCallIndex)
               {
                  zenMockObject.PrivateZenMock(20);
               }
               else
               {
                  zenMockObject.PrivateZenMock(10);
               }
            }
            //
            const string expectedWhat = String::Concat(R"(
  Failed: VECTORS_EQUAL(expectedOneArgCalls, actualOneArgCalls, this->ZenMockedFunctionSignature)
Expected: vector<T>
  Actual: vector<T>
 Because: ARE_EQUAL(ithExpectedElement, ithActualElement, iEqualsIndexMessage) failed
Expected: ZenMock::OneArgCall:
Arg: 10
  Actual: ZenMock::OneArgCall:
Arg: 20
 Message: "i=)", mismatchingCallIndex, R"("
 Message: ")", expectedSignature, R"("
File.cpp(1)
File.cpp(1))");
            const int expectedArg = 10;
            vector<OneArgCallRef<int>> expectedCalls;
            ZenMockTester::call_n_times(expectedCallsSize, [&]{ expectedCalls.emplace_back(expectedArg); });
            THROWS(zenMockObject.AssertCalls(expectedCalls), Anomaly, expectedWhat);
         };
         test(mock.VirtualMock, virtualSignature);
         test(mock.VirtualConstMock, virtualConstSignature);
         test(mock.NonVirtualMock, nonVirtualSignature);
         test(mock.NonVirtualConstMock, nonVirtualConstSignature);
         test(globalMock, globalSignature);
         test(namespaceMock, namespaceSignature);
         test(staticNameClashMock, staticNameClashSignature);
         test(staticMock, staticSignature);
      }

      void AssertCalls_NonEmptyCalls_FunctionCalledCallsSizeTimesMatchingArgs_DoesNotThrow(
         size_t expectedCallsSize)
      {
         const auto test = [&](auto& zenMockObject)
         {
            zenMockObject.Expect();
            //
            ZenMockTester::call_n_times(expectedCallsSize, [&]{ zenMockObject.PrivateZenMock(10); });
            //
            const int expectedArg = 10;
            vector<OneArgCallRef<int>> expectedCalls;
            ZenMockTester::call_n_times(expectedCallsSize, [&]{ expectedCalls.emplace_back(expectedArg); });
            zenMockObject.AssertCalls(expectedCalls);
         };
         test(mock.VirtualMock);
         test(mock.VirtualConstMock);
         test(mock.NonVirtualMock);
         test(mock.NonVirtualConstMock);
         test(globalMock);
         test(namespaceMock);
         test(staticNameClashMock);
         test(staticMock);
      }
   };
}
