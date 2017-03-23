#pragma once
#include <regex>
#include <unordered_map>
#ifdef __linux__
#include <cxxabi.h>
#include "ZenUnit/Utils/assert_true.h"
#endif

namespace ZenUnit
{
   class Type
   {
      friend class TypeTests;
   private:
      // Demangling is expensive so this demangled type name cache exists
      static std::unordered_map<const char*, std::string> s_mangledToDemangledTypeName;
   public:
      template<typename T>
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4100) // 'variable' unreferenced formal parameter (MSVC does not count typeid(variable) as referencing variable
#endif
      static const std::string* GetName(const T& variable)
#ifdef _WIN32
#pragma warning(pop)
#endif
      {
         return TypeInfoToTypeName(typeid(variable));
      }

      template<typename T>
      static const std::string* GetName()
      {
         return TypeInfoToTypeName(typeid(T));
      }
   private:
      static const std::string* TypeInfoToTypeName(const std::type_info& typeInfo)
      {
         const char* mangledTypeName = typeInfo.name();
         std::unordered_map<const char*, std::string>::const_iterator findIter
            = s_mangledToDemangledTypeName.find(mangledTypeName);
         if (findIter == s_mangledToDemangledTypeName.end())
         {
            std::string demangledTypeName = Demangle(mangledTypeName);
            std::pair<std::unordered_map<const char*, std::string>::const_iterator, bool>
               emplaceResult = s_mangledToDemangledTypeName.emplace(mangledTypeName, demangledTypeName);
            const std::string* cachedDemangledTypeName = &emplaceResult.first->second;
            return cachedDemangledTypeName;
         }
         const std::string* cachedDemangledTypeName = &findIter->second;
         return cachedDemangledTypeName;
      }

#ifdef __linux__
      static std::string Demangle(const char* mangledTypeName)
      {
         int demangleStatus = -1;
         std::unique_ptr<char, void(*)(void*)> demangledTypeNamePointer(
            abi::__cxa_demangle(mangledTypeName, 0, 0, &demangleStatus),
            std::free);
         assert_true(demangleStatus == 0);
         std::string demangledTypeName(demangledTypeNamePointer.get());
         return demangledTypeName;
      }
#elif _WIN32
      static std::string Demangle(const char* mangledTypeName)
      {
         static const std::regex classStructPattern("(class |struct )");
         std::string typeNameMinusClassAndStruct = std::regex_replace(mangledTypeName, classStructPattern, "");
         return typeNameMinusClassAndStruct;
      }
#endif
   };
}
