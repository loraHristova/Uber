#include "User.h"


User::User(const MyString& username, const MyString& password, const MyString& firstName, const MyString& lastName) : username(username), password(password), firstName(firstName), lastName(lastName)
{}

const MyString& User::getUsername() const
{
	return username;
}

const MyString& User::getFirstName() const
{
	return firstName;
}

const MyString& User::getLastName() const
{
	return lastName;
}

void User::setFirstName(const MyString& firstName)
{
	this->firstName = firstName;
}

void User::setLastName(const MyString& lastName)
{
	this->lastName = lastName;
}

bool User::comparePasswords(const MyString& pass)
{
	return (strcmp(password.c_str(), pass.c_str()) == 0);
}

void User::printInfo() const
{
	std::cout << "Username: " << username << std::endl <<
		"Full name: " << firstName << " " << lastName << std::endl <<
		"Amount: " << money << std::endl;
}