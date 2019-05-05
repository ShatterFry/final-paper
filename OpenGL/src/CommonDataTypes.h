#pragma once

#include "vector"
#include "string"

class UCallbackDataBase {};

template <typename T>
class UCallbackGetVectorData : public UCallbackDataBase
{
public:
	std::string ColumnName;
	std::vector<T>* VectorToFill;
private:

};