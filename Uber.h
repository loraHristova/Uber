#pragma once
#include "Client.h"
class Order;
#include "Driver.h"
#include "MyString.h"
class Driver;
#include "Order.h"
#include "Vector.hpp"
#include <fstream>

class Uber
{
	User* loggedUser;
	MyVector<Client> clients;
	unsigned countOfClients = 0;
	MyVector<Driver> drivers;
	unsigned countOfDrivers = 0;
	MyVector<Order> allMadeOrders;
	unsigned countOfOrders = 0;

public:
	void regClient(const MyString& username, const MyString& password, const MyString& firstName, const MyString& lastName);
	void regDriver(const MyString& username, const MyString& password, const MyString& firstName, const MyString& lastName, unsigned carNumber, char* phoneNumber);

	void loginClient(const MyString& username, const MyString& password);
	void loginDriver(const MyString& username, const MyString& password);
	void logout();

	void usersInitReg();
	void usersInitLoginAndOptions();
	void usersCoresp();

	double calculateDistance(int x1, int y1, int x2, int y2) const;
	void sendMessageToTheClosestDriver(Order& currentOrder);

	void clientCoresp(MyString& username, MyString& pass);
	void driverCoresp(MyString& username, MyString& pass);

	void writeAllMadeOrders(std::ofstream& file);
	void writeDrivers(std::ofstream& file);
	void writeClients(std::ofstream& file);

	void writeSystemToFile(std::ofstream& file);
};

