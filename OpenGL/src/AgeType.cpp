#include "AgeType.h"

std::string GetAgeTypeAbbrev(EAgeType inAgeType)
{
	switch (inAgeType)
	{
	case EAgeType::se:
		return std::string("se");
	case EAgeType::p:
		return std::string("p");
	case EAgeType::j:
		return std::string("j");
	case EAgeType::im:
		return std::string("im");
	case EAgeType::v:
		return std::string("v");
	case EAgeType::g1:
		return std::string("g1");
	case EAgeType::g2:
		return std::string("g2");
	case EAgeType::g3:
		return std::string("g3");
	case EAgeType::ss:
		return std::string("ss");
	case EAgeType::s:
		return std::string("s");
	default:
		break;
	}
}