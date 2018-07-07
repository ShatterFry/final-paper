#pragma once
#include <string>
//#include <iostream>

enum class AgeTypeId { seId, pId, jId, imId, vId, g1Id, g2Id, g3Id, ssId, sId };

std::ostream& operator << (std::ostream& s, const AgeTypeId& atid)
{
	std::cout << (int)atid;
	return s;
}

struct AgeType {
private:
	AgeTypeId id;
	std::string name;
	int _minAge;
	int maxAge;
public:
	AgeTypeId GetId();
	void SetId(AgeTypeId atid);

	std::string GetName();
	void SetName(std::string name);

	int GetMinAge();
	void SetMinAge(int minAge);

	int GetMaxAge();
	void SetMaxAge(int maxAge);

	friend std::istream& operator >> (std::istream& s, AgeType& at);
	friend std::ostream& operator << (std::ostream& s, AgeType& at);
	friend std::ostream& operator << (std::ostream& s, const AgeType& at);
};

AgeTypeId AgeType::GetId()
{
	return this->id;
}

void AgeType::SetId(AgeTypeId atid)
{
	this->id = atid;
}

std::string AgeType::GetName()
{
	return this->name;
}

void AgeType::SetName(std::string name)
{
	this->name.erase(0, this->name.length());
	this->name.resize(name.length());
	for (int i = 0; i < name.length(); ++i)
	{
		this->name[i] = name[i];
	}
}

int AgeType::GetMinAge()
{
	return this->_minAge;
}

void AgeType::SetMinAge(int minAge)
{
	this->_minAge = minAge;
}

int AgeType::GetMaxAge()
{
	return this->maxAge;
}

void AgeType::SetMaxAge(int maxAge)
{
	this->maxAge = maxAge;
}

std::istream& operator >> (std::istream& s, AgeType& at)
{
	std::cin >> at.name;
	return s;
}

std::ostream& operator << (std::ostream& s, AgeType& at)
{
	std::cout << at.name.c_str();
	return s;
}

std::ostream& operator << (std::ostream& s, const AgeType& at)
{
	std::cout << at.name.c_str();
	return s;
}