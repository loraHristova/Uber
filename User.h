#pragma once
#include "MyString.h"

class User
{
protected:
	MyString username = "";
	MyString password = "";
	MyString firstName = "";
	MyString lastName = "";
	double money = 0;

public:
	User() = default;
	User(const MyString& username, const MyString& password, const MyString& firstName, const MyString& lastName);

	const MyString& getUsername() const;
	const MyString& getFirstName() const;
	const MyString& getLastName() const;

	void setFirstName(const MyString& username);
	void setLastName(const MyString& username);

	bool comparePasswords(const MyString& pass);
	virtual void printInfo() const;

	virtual ~User() = default;
};
