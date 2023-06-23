#pragma once
#include "MyString.h"
#include "User.h"
#include "Address.h"
#include "Vector.hpp"
class Driver;
#include "Order.h"
#include <exception>
#include <fstream>
#include "HelperFunctions.h"

class Driver : public User
{
	Address currentAdress;
	unsigned rating = 0;
	unsigned carNumber = 0;
	MyString phoneNumber;
	unsigned id;

	Order** messages = nullptr;
	unsigned countOfMessages = 0;
	unsigned capacity = 4;

	void free();
	void copyFrom(const Driver& other);
	void moveFrom(Driver&& other);

public:
	Driver() = default;
	Driver(const MyString& username, const MyString& password, const MyString& firstName, const MyString& lastName, unsigned carNumber, const char* phoneNumber);
	Driver(const Driver& other);
	Driver& operator=(const Driver& other);
	Driver(Driver&& other);
	Driver& operator=(Driver&& other);

	void resize();

	void setId(unsigned id);
	void setCurrentAddress(MyString name, unsigned x, unsigned y);
	void checkMessages() const;
	void acceptOrder(int id, unsigned minutes);
	void declineOrder(int id);
	void finishOrder(int id);
	void acceptPayment(int id);

	void updateRating();
	void addNewMessage(Order& newMessage);

	const MyString& getFirstName() const;
	const MyString& getLastName() const;
	unsigned getCarNumber() const;
	const MyString& getPhoneNumber() const;
	const Address& getCurrentAddress() const;
	const Order* getMessages() const;
	unsigned getId() const;

	bool checkValidityOfId(unsigned id) const;
	const Order* findMessage(unsigned id) const;

	void printInfo() const override;
	void deleteMessage(unsigned index);

	friend void writeDriverToFile(std::ofstream& file, const Driver& driver);

	~Driver();
};


