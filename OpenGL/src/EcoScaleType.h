#pragma once

enum class EEcoScaleType
{
	OM,
	HD,
	TR,
	RC,
	LC,
	MAX
};

std::string GetEcoScaleAbbreviation(EEcoScaleType inType);