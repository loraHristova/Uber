#pragma once
#include "User.h"
#include "Address.h"
#include "Vector.hpp"
#include "Order.h"
#include "HelperFunctions.h"
#include <fstream>

class Client : public User
{
	Order* activeOrder = nullptr;
	MyVector<Order> historyOfFinishedOrders;
	unsigned countOfFinishedOrders = 0;

public:
	Client() = default;
	Client(const MyString& username, const MyString& password, const MyString& firstName, const MyString& lastName);

	void setActiveOrder(Order* order);
	void checkOrder(unsigned id) const;
	void cancelOrder(unsigned id);
	void pay(unsigned id, double amount);
	void rate(const MyString& firstNameOfDriver, const MyString& lastNameOfDriver, unsigned rating);
	void addMoney(double amount);
	void addOrderToHistory(const Order& order);

	bool checkValidityOfId(unsigned id) const;
	void printInfo() const override;
	const Order* getActiveOrder() const;

	friend void writeClientToFile(std::ofstream& file, const Client& client);
};


