#pragma once
#include "ZenUnit/Macros/ARE_EQUAL.h"
#include "ZenUnit/Anomaly/Anomaly.h"
#include "ZenUnit/Utils/Map.h"
#include "ZenUnit/Utils/assert_true.h"

#define MAPS_EQUAL(expectedMap, actualMap, ...) \
   ZenUnit::MAPS_EQUAL_Defined(VRT(expectedMap), VRT(actualMap), \
   FILELINE, VATEXT(__VA_ARGS__), ##__VA_ARGS__)

namespace ZenUnit
{
   template<typename... MessageTypes>
   void MAPS_EQUAL_ThrowAnomaly(
      const char* failedPrefixSpaces,
      const char* expectedMapText,
      const char* actualMapText,
      const std::string& whyBody,
      FileLine fileLine,
      const char* messagePrefixSpaces,
      const char* messagesText,
      const MessageTypes&... messages)
   {
      std::string failedLinePrefix = String::Concat(
         failedPrefixSpaces, "Failed: MAPS_EQUAL(", expectedMapText, ", ", actualMapText);
      Anomaly anomaly(failedLinePrefix, whyBody, fileLine, messagePrefixSpaces, messagesText, messages...);
      throw anomaly;
   }

   template<typename MapType, typename... MessageTypes>
   void MAPS_EQUAL_Defined(
      VRText<MapType> expectedMapVRT,
      VRText<MapType> actualMapVRT,
      FileLine fileLine, const char* messagesText, const MessageTypes&... messages)
   {
      const char* expectedMapText = expectedMapVRT.text;
      const char* actualMapText = actualMapVRT.text;
      const MapType& expectedMap = expectedMapVRT.value;
      const MapType& actualMap = actualMapVRT.value;
      try
      {
         ARE_EQUAL(expectedMap.size(), actualMap.size());
      }
      catch (const Anomaly&)
      {
         std::string whyBody = String::Concat(
             " Because: ARE_EQUAL(expectedMap.size(), actualMap.size()) failed\n",
             "Expected: ", expectedMap.size(), '\n',
             "  Actual: ", actualMap.size());
         MAPS_EQUAL_ThrowAnomaly("  ", expectedMapText, actualMapText,
            whyBody, fileLine, " ", messagesText, messages...);
      }
      for (const auto& expectedKeyValuePair : expectedMapVRT.value)
      {
         const auto& expectedKey = expectedKeyValuePair.first;
         const auto& expectedValue = expectedKeyValuePair.second;
         std::pair<bool, bool> containsKeyValue =
            Map::ContainsKeyWithValue(actualMap, expectedKey, expectedValue);
         bool mapContainsKey = containsKeyValue.first;
         using KeyType = typename std::decay<decltype(expectedKey)>::type;
         if (!mapContainsKey)
         {
            std::string toStringedExpectedKey = ToStringer::ToString<KeyType>(expectedKey);
            std::string whyBody = String::Concat(
               "Because: Actual map does not contain expected key\n",
               "Expected key: ", toStringedExpectedKey);
            MAPS_EQUAL_ThrowAnomaly(" ", expectedMapText, actualMapText,
               whyBody, fileLine, "", messagesText, messages...);
         }
         bool mapContainsValue = containsKeyValue.second;
         if (!mapContainsValue)
         {
            assert_true(containsKeyValue.first);
            std::string toStringedExpectedKey = ToStringer::ToString<KeyType>(expectedKey);
            const std::string& toStringedActualKey = toStringedExpectedKey;
            std::string toStringedExpectedValue =
               ToStringer::ToString<typename std::decay<decltype(expectedValue)>::type>(expectedValue);
            const auto& actualValue = Map::At(actualMap, expectedKey);
            std::string toStringedActualValue =
               ToStringer::ToString<typename std::decay<decltype(actualValue)>::type>(actualValue);
            std::string whyBody = String::Concat(
                "Because: Actual map contains expected key but with an unexpected value\n",
                "  Expected key: ", toStringedExpectedKey, '\n',
                "    Actual key: ", toStringedActualKey, '\n',
                "Expected value: ", toStringedExpectedValue, '\n',
                "  Actual value: ", toStringedActualValue);
            MAPS_EQUAL_ThrowAnomaly(" ", expectedMapText, actualMapText,
               whyBody, fileLine, "", messagesText, messages...);
         }
      }
   }
}
