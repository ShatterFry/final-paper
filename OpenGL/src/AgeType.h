#pragma once
#include <string>

enum class EAgeType { se, p, j, im, v, g1, g2, g3, ss, s, MAX };

std::string GetAgeTypeAbbrev(EAgeType inAgeType);