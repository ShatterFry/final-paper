#pragma once

#include <vector>
#include <map>
#include <functional>

#include "AgeType.h"

using UIntVector = std::vector<unsigned int>;
using FloatVector = std::vector<float>;
using AgeRadiusVector = std::vector<float>;
using AgeRadiusMap = std::map<EAgeType, float>;

using FFunctionCallback = std::function<void()>;
