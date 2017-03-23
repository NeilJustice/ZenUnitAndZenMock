#pragma once
#include "ZenUnit/ToStringer/ToStringer.h"
#include "ZenUnit/ZenMock/Exceptions/ZenMockException.h"

namespace ZenMock
{
   class UnexpectedCallException : public ZenMockException
   {
   private:
      std::string _what;
   public:
      template<typename... ArgTypes>
      UnexpectedCallException(
         const std::string& zenMockedFunctionSignature, const ArgTypes&... args)
         : _what(MakeWhat(zenMockedFunctionSignature, args...))
      {
      }

      template<typename... ArgTypes>
      static std::string MakeWhat(const std::string& zenMockedFunctionSignature, const ArgTypes&... args)
      {
         std::ostringstream whatBuilder;
         whatBuilder << "Unexpected call to \"" << zenMockedFunctionSignature << "\"";
         AppendToStringedArgs(whatBuilder, 0, args...);
         whatBuilder << R"(
Fix for this: Add before the unexpected call a call to
[ZenMockObjectName].[Expect or )";
         bool isVoid = zenMockedFunctionSignature.find("void ") != std::string::npos;
         if (isVoid)
         {
            whatBuilder << "ExpectAndThrow";
         }
         else
         {
            whatBuilder << "ExpectAndReturn or ExpectAndReturnValues or ExpectAndThrow";
         }
         whatBuilder << "]()";
         std::string what = whatBuilder.str();
         return what;
      }

      const char* what() const noexcept override
      {
         return _what.c_str();
      }
   private:
      template<typename ArgType, typename... SubsequentArgTypes>
      static void AppendToStringedArgs(
         std::ostringstream& outWhatBuilder,
         size_t argIndex,
         const ArgType& arg,
         const SubsequentArgTypes&... args)
      {
         std::string toStringedArg = ZenUnit::ToStringer::ToString(arg);
         outWhatBuilder << "\nArg" << ++argIndex << ": " << toStringedArg;
         AppendToStringedArgs(outWhatBuilder, argIndex, args...);
      }

      static void AppendToStringedArgs(std::ostringstream&, size_t)
      {
      }
   };
}
