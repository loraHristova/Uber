#include "Address.h"

Address::Address(const MyString& name, unsigned x, unsigned y, const MyString& additionalInfo) : name(name), coordinates(x, y), additionalInfo(additionalInfo)
{}

void Address::setName(const MyString& newName) 
{
	name = newName;
}

void Address::setCoordinates(int x, int y)
{
	coordinates.x = x;
	coordinates.y = y;
}

const MyString& Address::getNameOfAddress() const
{
	return name;
}

int Address::getXCoordinate() const
{
	return coordinates.x;
}

int Address::getYCoordinate() const
{
	return coordinates.y;
}

const MyString& Address::getAdditionalInfo() const
{
	return additionalInfo;
}

void Address::printAddress() const
{
	std::cout << "Name of the address: " << name << std::endl <<
		"Coordinates: " << coordinates.x << " " << coordinates.y << std::endl <<
		"additional info: " << additionalInfo << std::endl;
}

void Address::setAddInfo(const MyString& addInfo)
{
	additionalInfo = addInfo;
}