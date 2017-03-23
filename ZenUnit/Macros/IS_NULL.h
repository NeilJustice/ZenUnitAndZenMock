#pragma once
#include "ZenUnit/Utils/FileLine.h"
#include "ZenUnit/Utils/VRText.h"
#include "ZenUnit/Anomaly/Anomaly.h"
#include "ZenUnit/ToStringer/ToStringer.h"

#define IS_NULL(pointer, ...) \
   ZenUnit::IS_NULL_Defined(VRT(pointer), \
   FILELINE, VATEXT(__VA_ARGS__), ##__VA_ARGS__)

namespace ZenUnit
{
   template<typename PointerType, typename... MessageTypes>
   void IS_NULL_Defined(VRText<PointerType> pointerVRT,
      FileLine fileLine, const char* messagesText, const MessageTypes&... messages)
   {
      bool pointerIsNull = pointerVRT.value == nullptr;
      if (!pointerIsNull)
      {
         std::string toStringedActual = ToStringer::ToString(pointerVRT.value);
         Anomaly anomaly("IS_NULL", pointerVRT.text, "", "", messagesText,
            Anomaly::Default,
            "nullptr",
            toStringedActual,
            ExpectedActualFormat::Fields, fileLine, messages...);
         throw anomaly;
      }
   }
}
