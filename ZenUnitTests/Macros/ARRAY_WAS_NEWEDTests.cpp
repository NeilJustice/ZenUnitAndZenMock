#include "pch.h"
#include "ZenUnit/Macros/ARRAY_WAS_NEWED.h"

TESTS(ARRAY_WAS_NEWEDTests)
FACT(NullRawPointer_Throws)
FACT(NullRawPointer_Throws_MessagesTestCase)
FACT(EmptyUniqueArrayPtr_Throws)
FACT(NonNullRawPointer_DestructsEachElement_NotCallableTwiceWithoutUndefinedBehavior)
FACT(NonNullUniqueArrayPointer_DestructsEachElement_ThrowsWhenCalledTwice)
EVIDENCE

struct Deletable
{
   static unsigned s_destructorCallCount;
   ~Deletable() { ++s_destructorCallCount; }
};

CLEANUP
{
   Deletable::s_destructorCallCount = 0;
}

TEST(NullRawPointer_Throws)
{
   const char* nullRawPointer = nullptr;
   THROWS(ARRAY_WAS_NEWED(nullRawPointer), Anomaly, R"(
  Failed: ARRAY_WAS_NEWED(nullRawPointer)
Expected: not a nullptr
  Actual: nullptr
File.cpp(1))");
}

TEST(NullRawPointer_Throws_MessagesTestCase)
{
   const char* nullRawPointer = nullptr;
   const string MessageA = "A", MessageB = "B";
   THROWS(ARRAY_WAS_NEWED(nullRawPointer, MessageA, MessageB), Anomaly, R"(
  Failed: ARRAY_WAS_NEWED(nullRawPointer, MessageA, MessageB)
Expected: not a nullptr
  Actual: nullptr
 Message: "A", "B"
File.cpp(1))");
}

TEST(EmptyUniqueArrayPtr_Throws)
{
   unique_ptr<const int[]> emptyUniqueArrayPtr;
   THROWS(ARRAY_WAS_NEWED(emptyUniqueArrayPtr), Anomaly, R"(
  Failed: ARRAY_WAS_NEWED(emptyUniqueArrayPtr)
Expected: not a nullptr
  Actual: nullptr
File.cpp(1))");
}

TEST(NonNullRawPointer_DestructsEachElement_NotCallableTwiceWithoutUndefinedBehavior)
{
   Deletable* deletables = new Deletable[3];
   ARE_EQUAL(0, Deletable::s_destructorCallCount);
   //
   ARRAY_WAS_NEWED(deletables);
   //
   ARE_EQUAL(3, Deletable::s_destructorCallCount);

   int* ints = new int[1];
   ARRAY_WAS_NEWED(ints);
}

TEST(NonNullUniqueArrayPointer_DestructsEachElement_ThrowsWhenCalledTwice)
{
   unique_ptr<const Deletable[]> nonNullUniqueArrayPtr(new Deletable[5]);
   ARE_EQUAL(0, Deletable::s_destructorCallCount);
   //
   ARRAY_WAS_NEWED(nonNullUniqueArrayPtr);
   //
   ARE_EQUAL(5, Deletable::s_destructorCallCount);

   THROWS(ARRAY_WAS_NEWED(nonNullUniqueArrayPtr), Anomaly, R"(
  Failed: ARRAY_WAS_NEWED(nonNullUniqueArrayPtr)
Expected: not a nullptr
  Actual: nullptr
File.cpp(1))");

   unique_ptr<const int[]> ints(new int[1]);
   ARRAY_WAS_NEWED(ints);
   THROWS(ARRAY_WAS_NEWED(ints), Anomaly, R"(
  Failed: ARRAY_WAS_NEWED(ints)
Expected: not a nullptr
  Actual: nullptr
File.cpp(1))");
}

}; RUNTESTS(ARRAY_WAS_NEWEDTests)

unsigned ARRAY_WAS_NEWEDTests::Deletable::s_destructorCallCount = 0;
