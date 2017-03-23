#pragma once
#include "ZenUnit/Macros/MAPS_EQUAL.h"

template<
   typename KeyType, 
   typename ValueType, 
   typename PredicateType, 
   typename AllocatorType>
struct ZenUnitEqualizer<std::map<
   KeyType, ValueType, PredicateType, AllocatorType>>
{
   static void AssertEqual(
      const std::map<KeyType, ValueType, PredicateType, AllocatorType>& expectedStdMap,
      const std::map<KeyType, ValueType, PredicateType, AllocatorType>& actualStdMap)
   {
      MAPS_EQUAL(expectedStdMap, actualStdMap);
   }
};

template<
   typename KeyType, 
   typename ValueType, 
   typename HasherType, 
   typename KeyEqualityComparator, 
   typename AllocatorType>
struct ZenUnitEqualizer<std::unordered_map<
   KeyType, ValueType, HasherType, KeyEqualityComparator, AllocatorType>>
{
   static void AssertEqual(
      const std::unordered_map<
         KeyType, ValueType, HasherType, KeyEqualityComparator, AllocatorType>& expectedStdUnorderedMap,
      const std::unordered_map<
         KeyType, ValueType, HasherType, KeyEqualityComparator, AllocatorType>& actualStdUnorderedMap)
   {
      MAPS_EQUAL(expectedStdUnorderedMap, actualStdUnorderedMap);
   }
};
