#pragma once
#include "MyString.h"

class Address
{
	struct Point {
		int x;
		int y;

		Point(int x, int y) : x(x), y(y) {}
	};

	MyString name = "";
	Point coordinates{ 0, 0 };
	MyString additionalInfo = "";
public:
	Address() = default;
	Address(const MyString& name, unsigned x, unsigned y, const MyString& additionalInfo);

	void setName(const MyString& newName);
	void setCoordinates(int x, int y);
	void setAddInfo(const MyString& addInfo);

	const MyString& getNameOfAddress() const;
	int getXCoordinate() const;
	int getYCoordinate() const;
	const MyString& getAdditionalInfo() const;

	void printAddress() const;
};

