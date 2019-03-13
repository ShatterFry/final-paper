#pragma once
#include <EcoScaleType.h>

using EcoScaleData = std::vector<struct EcoScaleDataEntry>;

struct EcoScaleDataEntry
{
	EEcoScaleType mType;
	int mMin;
	int mMax;
};