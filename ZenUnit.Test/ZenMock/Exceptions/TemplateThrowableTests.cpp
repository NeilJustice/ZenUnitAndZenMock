#include "pch.h"
#include "ZenUnit/ZenMock/Exceptions/TemplateThrowable.h"

namespace ZenMock
{
   template<typename ExceptionType>
   TEMPLATETESTS(TemplateThrowableTests, ExceptionType)
   SPEC(New_ReturnsNewInstanceOfSelfAsThrowablePointerWithExceptionCreatedFromExceptionArgs)
   SPEC(Throw_ExceptionIsNullptr_DoesNothing)
   SPEC(Throw_ExceptionIsNotNullptr_ThrowsTheException)
   SPECEND

   TemplateThrowable<ExceptionType> _templateThrowable;
   const string ExceptionWhat = "ExceptionWhat";

   TEST(New_ReturnsNewInstanceOfSelfAsThrowablePointerWithExceptionCreatedFromExceptionArgs)
   {
      const Throwable* throwable = TemplateThrowable<ExceptionType>::New(ExceptionWhat);
      THROWS(throwable->Throw(), ExceptionType, ExceptionWhat);
      delete throwable;
   }

   TEST(Throw_ExceptionIsNullptr_DoesNothing)
   {
      _templateThrowable.Throw();
   }

   TEST(Throw_ExceptionIsNotNullptr_ThrowsTheException)
   {
      _templateThrowable._exception.reset(new ExceptionType(ExceptionWhat));
      THROWS(_templateThrowable.Throw(), ExceptionType, ExceptionWhat);
   }

   };
   TEMPLATERUN(TemplateThrowableTests, runtime_error)
   TEMPLATERUN(TemplateThrowableTests, logic_error)
}
