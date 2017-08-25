#include "pch.h"
#include "ZenUnit/ZenMock/ValueReturner.h"

struct NonDefaultConstructible
{
   int x;
   NonDefaultConstructible(int x)
      : x(x) {}
};

template<>
struct ZenUnitEqualizer<NonDefaultConstructible>
{
   static void AssertEqual(
      const NonDefaultConstructible& expected,
      const NonDefaultConstructible& actual)
   {
      ARE_EQUAL(expected.x, actual.x);
   }
};

namespace ZenMock
{
   TESTS(ValueReturnerTests)
   FACT(DefaultConstructor_SetsZenMockedFunctionSignature_SetsReturnValueIndexTo0)
   FACT(ZenMockNextReturnValue_DefConReturnType_NoReturnValuesSpecified_ReturnsDefaultValue)
   FACT(ZenMockNextReturnValue_DefConReturnType_ReturnValuesSpecified_ReturnsValuesThenLastValueThereafter)
   FACT(ZenMockNextReturnValue_NonDefConReturnType_NoReturnValuesSpecified_Throws)
   FACT(ZenMockNextReturnValue_NonDefConReturnType_ReturnValuesPreviouslySpecified_ReturnsValuesThenLastValueTherafter)
   FACT(ZenMockAddContainerReturnValues_ThrowsIfReturnValuesArgumentEmpty)
   EVIDENCE

   const string ZenMockedFunctionSignature = "ZenMockedFunctionSignature";

   TEST(DefaultConstructor_SetsZenMockedFunctionSignature_SetsReturnValueIndexTo0)
   {
      ValueReturner<int> valueReturner(ZenMockedFunctionSignature);
      //
      ARE_EQUAL(ZenMockedFunctionSignature, valueReturner.ZenMockedFunctionSignature);
      ARE_EQUAL(0, valueReturner._returnValueIndex);
      IS_EMPTY(valueReturner._returnValues);
   }

   TEST(ZenMockNextReturnValue_DefConReturnType_NoReturnValuesSpecified_ReturnsDefaultValue)
   {
      ValueReturner<int> valueReturnerInt(ZenMockedFunctionSignature);
      ARE_EQUAL(0, valueReturnerInt.ZenMockNextReturnValue());
      ARE_EQUAL(0, valueReturnerInt.ZenMockNextReturnValue());
      ARE_EQUAL(0, valueReturnerInt.ZenMockNextReturnValue());

      ValueReturner<vector<int>> valueReturnerVectorOfInt(ZenMockedFunctionSignature);
      VECTORS_EQUAL(vector<int>(), valueReturnerVectorOfInt.ZenMockNextReturnValue());
      VECTORS_EQUAL(vector<int>(), valueReturnerVectorOfInt.ZenMockNextReturnValue());
      VECTORS_EQUAL(vector<int>(), valueReturnerVectorOfInt.ZenMockNextReturnValue());
   }

   TEST(ZenMockNextReturnValue_DefConReturnType_ReturnValuesSpecified_ReturnsValuesThenLastValueThereafter)
   {
      ValueReturner<int> valueReturner(ZenMockedFunctionSignature);
      valueReturner.ZenMockAddReturnValue(1);
      valueReturner.ZenMockAddReturnValues(2, 3);
      valueReturner.ZenMockAddContainerReturnValues(vector<int> { 4, 5 });
      valueReturner.ZenMockAddContainerReturnValues(std::array<int, 2>{{ 6, 7 }});
      vector<int> lvalueVector{8};
      valueReturner.ZenMockAddContainerReturnValues(lvalueVector);
      std::array<int, 1> lvalueArray{{9}};
      valueReturner.ZenMockAddContainerReturnValues(lvalueArray);
      ARE_EQUAL(1, valueReturner.ZenMockNextReturnValue());
      ARE_EQUAL(2, valueReturner.ZenMockNextReturnValue());
      ARE_EQUAL(3, valueReturner.ZenMockNextReturnValue());
      ARE_EQUAL(4, valueReturner.ZenMockNextReturnValue());
      ARE_EQUAL(5, valueReturner.ZenMockNextReturnValue());
      ARE_EQUAL(6, valueReturner.ZenMockNextReturnValue());
      ARE_EQUAL(7, valueReturner.ZenMockNextReturnValue());
      ARE_EQUAL(8, valueReturner.ZenMockNextReturnValue());
      ARE_EQUAL(9, valueReturner.ZenMockNextReturnValue());
      ARE_EQUAL(9, valueReturner.ZenMockNextReturnValue());
   }

   TEST(ZenMockNextReturnValue_NonDefConReturnType_NoReturnValuesSpecified_Throws)
   {
      ValueReturner<NonDefaultConstructible> valueReturner(ZenMockedFunctionSignature);
      THROWS(valueReturner.ZenMockNextReturnValue(), ReturnValueMustBeSpecifiedException,
         ReturnValueMustBeSpecifiedException::MakeWhat(ZenMockedFunctionSignature));
   }

   TEST(ZenMockNextReturnValue_NonDefConReturnType_ReturnValuesPreviouslySpecified_ReturnsValuesThenLastValueTherafter)
   {
      ValueReturner<NonDefaultConstructible> valueReturner(ZenMockedFunctionSignature);
      valueReturner.ZenMockAddReturnValue(NonDefaultConstructible(1));
      valueReturner.ZenMockAddReturnValues(NonDefaultConstructible(2), NonDefaultConstructible(3));
      valueReturner.ZenMockAddContainerReturnValues(vector<NonDefaultConstructible> { NonDefaultConstructible(4), NonDefaultConstructible(5) });
      //
      ARE_EQUAL(NonDefaultConstructible(1), valueReturner.ZenMockNextReturnValue());
      ARE_EQUAL(NonDefaultConstructible(2), valueReturner.ZenMockNextReturnValue());
      ARE_EQUAL(NonDefaultConstructible(3), valueReturner.ZenMockNextReturnValue());
      ARE_EQUAL(NonDefaultConstructible(4), valueReturner.ZenMockNextReturnValue());
      ARE_EQUAL(NonDefaultConstructible(5), valueReturner.ZenMockNextReturnValue());
      ARE_EQUAL(NonDefaultConstructible(5), valueReturner.ZenMockNextReturnValue());
   }

   TEST(ZenMockAddContainerReturnValues_ThrowsIfReturnValuesArgumentEmpty)
   {
      ValueReturner<int> valueReturner(ZenMockedFunctionSignature);
      const char* const ExpectedWhat = "ZenMock::ValueReturner::ZenMockAddContainerReturnValues(): Return values vector cannot be empty";
      THROWS(valueReturner.ZenMockAddContainerReturnValues(vector<int>{}), invalid_argument, ExpectedWhat);
   }

   }; RUNTESTS(ValueReturnerTests)
}
