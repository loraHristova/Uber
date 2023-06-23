#pragma once
#include "Address.h"
class Order;
#include "Driver.h"
#include <fstream>
#include <iostream>
#include "HelperFunctions.h"
//class Driver;//forward declaration for the pointer


class Order
{
	MyString firstNameOfTheOrderer = "";
	MyString lastNameOfTheOrderer = "";
	Address currentAddress;
	Address destination;
	unsigned countOfPassengers = 0;
	int idOfOrder = -1;

	MyVector<bool> seenByDrivers;
	bool isOrderAccepted = false;
	Driver* driverForTheOrder = nullptr;
	unsigned howManyMinutesUntilDriverComes = 1000;
	double rateForTheDriver = -1;
	bool isOrderFinished = false;

	double paidMoneyForTheOrder = 0;
	bool isPaymentAcceptedByTheDriver = false;
	bool isCanceled = false;

public:
	Order() = default;
	Order(const MyString& firstName, const MyString& lastName, const MyString& currentAddress, int coordinateX, int coordinateY, const MyString& additionalInfo, const MyString& nameOfDestination, int coordinateXDes, int coordinateYDes, const MyString& additionalInfoDes, unsigned passengerNumber, size_t sizeForSeenArr);
	Order(const MyString& firstNameOfTheOrder, const MyString& lastNameOfTheOrder, const Address& currentAdress, const Address& destination, unsigned countOfPassengers, size_t sizeOfSeenArr);

	int getIdOfOrder() const;
	bool isAccepted() const;
	unsigned getMinutesUntilDriverComes() const;
	bool isFinished() const;
	const Driver* getDriver() const;
	const MyString& getFirstNameOfTheOrder() const;
	const MyString& getLastNameOfTheOrder() const;
	const Address& getCurrentAddress() const;
	const Address& getDestination() const;
	double getPaidMoneyForTheOrder() const;
	bool isMoneyAccepted() const;
	double getDriversRatingForTheOrder() const;
	bool getIsCanceled() const;
	const MyVector<bool>& getSeen() const;

	void rateDriver(unsigned rate);
	void payDriver(double amount);

	void setAcceptedToTrue();
	void setFinishedToTrue();
	void setMinutesUntilDriverComes(unsigned minutes);
	void setPaidMoney(double moneyPaid);
	void setIsMoneyAcceptedToTrue();
	void setDriver(Driver& driver);
	void setIsCanceledToTrue();
	void setId(unsigned id);
	void setSeenByDriver(unsigned index); 
	void setFirstName(const MyString& firstName);
	void setLastName(const MyString& lastName);

	void deleteOrder();
	void printOrder() const;

	friend void writeOrder(std::ofstream& file, const Order& order);
};
