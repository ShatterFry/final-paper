#include <string>
#include "EcoScaleType.h"

std::string GetEcoScaleAbbreviation(EEcoScaleType inType)
{
	switch (inType)
	{
	case EEcoScaleType::OM:
		return std::string("om");
	case EEcoScaleType::HD:
		return std::string("hd");
	case EEcoScaleType::TR:
		return std::string("tr");
	case EEcoScaleType::RC:
		return std::string("rc");
	case EEcoScaleType::LC:
		return std::string("lc");
	case EEcoScaleType::MAX:
	default:
		return std::string("");
	}
}