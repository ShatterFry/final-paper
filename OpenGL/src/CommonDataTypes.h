#pragma once

#include "vector"
#include "string"

class UCallbackDataBase {};

template <typename T>
class UCallbackGetVectorData : public UCallbackDataBase
{
public:
	std::vector<std::string>* ColumnNames;
	std::vector<T>* VectorToFill;
private:

};
