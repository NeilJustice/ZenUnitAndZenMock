#include "pch.h"
#include "Anomaly.h"
#include "Macros/ARE_EQUAL.h"

namespace ZenUnit
{
   Anomaly::Anomaly()
   {
   }

   std::string Anomaly::MakeAssertExpression(
      const char* assertionName,
      const char* arg1Text,
      const char* arg2Text,
      const char* arg3Text,
      const char* messagesText)
   {
      std::ostringstream assertExpressionBuilder;
      assertExpressionBuilder << assertionName << "(" << arg1Text;
      if (strlen(arg2Text) > 0)
      {
         assertExpressionBuilder << ", " << arg2Text;
      }
      if (strlen(arg3Text) > 0)
      {
         assertExpressionBuilder << ", " << arg3Text;
      }
      if (strlen(messagesText) > 0)
      {
         assertExpressionBuilder << ", " << messagesText;
      }
      assertExpressionBuilder << ")";
      const std::string assertExpression = assertExpressionBuilder.str();
      return assertExpression;
   }

   const char* Anomaly::what() const noexcept
   {
      return why.c_str();
   }

   Anomaly Anomaly::ZENWrapped(
      const std::string& zenMockAssertExpression,
      const Anomaly& zenWrappedAnomaly,
      FileLine fileLine)
   {
      Anomaly anomaly;
      anomaly.assertExpression = zenMockAssertExpression;
      anomaly.expected = zenWrappedAnomaly.expected;
      anomaly.actual = zenWrappedAnomaly.actual;
      anomaly.message = zenWrappedAnomaly.message;
      anomaly.fileLine = fileLine;
      ostringstream whyBuilder;
      whyBuilder << "\n"
         "  Failed: " << zenMockAssertExpression << '\n';
      whyBuilder <<
         " Because: " << zenWrappedAnomaly.assertExpression << " failed\n"
         "Expected: " << zenWrappedAnomaly.expected << "\n"
         "  Actual: " << zenWrappedAnomaly.actual << "\n"
         " Message: " << zenWrappedAnomaly.message << '\n';
      whyBuilder << fileLine;
      anomaly.why = whyBuilder.str();
      return anomaly;
   }

   const Anomaly Anomaly::Default;
}

void ZenUnitEqualizer<ZenUnit::Anomaly>::
AssertEqual(const ZenUnit::Anomaly& expectedAnomaly, const ZenUnit::Anomaly& actualAnomaly)
{
   ARE_EQUAL(expectedAnomaly.assertExpression, actualAnomaly.assertExpression);
   ARE_EQUAL(expectedAnomaly.expected, actualAnomaly.expected);
   ARE_EQUAL(expectedAnomaly.actual, actualAnomaly.actual);
   ARE_EQUAL(expectedAnomaly.message, actualAnomaly.message);
   ARE_EQUAL(expectedAnomaly.why, actualAnomaly.why);
   ARE_EQUAL(expectedAnomaly.fileLine, actualAnomaly.fileLine);
}
