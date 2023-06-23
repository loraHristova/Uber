#include "Order.h"
#include <fstream>

Order::Order(const MyString& firstName, const MyString& lastName, const MyString& currentAddress, int coordinateX, int coordinateY, const MyString& additionalInfo, const MyString& nameOfDestination, int coordinateXDes, int coordinateYDes, const MyString& additionalInfoDes, unsigned passengerNumber, size_t sizeForSeenArr) : firstNameOfTheOrderer(firstName),
lastNameOfTheOrderer(lastName),
countOfPassengers(passengerNumber),
seenByDrivers(sizeForSeenArr, 10, 0),
currentAddress(currentAddress, coordinateX, coordinateY, additionalInfo),
destination(nameOfDestination, coordinateXDes, coordinateYDes, additionalInfoDes)
{}

Order::Order(const MyString& firstNameOfTheOrder, const MyString& lastNameOfTheOrder, const Address& currentAdress, const Address& destination, unsigned countOfPassengers, size_t sizeOfSeenArr) : firstNameOfTheOrderer(firstNameOfTheOrder), lastNameOfTheOrderer(lastNameOfTheOrder), countOfPassengers(countOfPassengers), seenByDrivers(sizeOfSeenArr, 10, 0)
{
	this->currentAddress = currentAdress;
	this->destination = destination;
}

int Order::getIdOfOrder() const
{
	return idOfOrder;
}

bool Order::isAccepted() const
{
	return this->isOrderAccepted;
}

unsigned Order::getMinutesUntilDriverComes() const
{
	return howManyMinutesUntilDriverComes;
}

bool Order::isFinished() const
{
	return isOrderFinished;
}

const Driver* Order::getDriver() const
{
	return driverForTheOrder;
}

const MyString& Order::getFirstNameOfTheOrder() const
{
	return firstNameOfTheOrderer;
}

const MyString& Order::getLastNameOfTheOrder() const
{
	return lastNameOfTheOrderer;
}

const Address& Order::getCurrentAddress() const
{
	return currentAddress;
}

const Address& Order::getDestination() const
{
	return destination;
}

double Order::getPaidMoneyForTheOrder() const
{
	return paidMoneyForTheOrder;
}

bool Order::isMoneyAccepted() const
{
	return isPaymentAcceptedByTheDriver;
}

double Order::getDriversRatingForTheOrder() const
{
	return rateForTheDriver;
}

void Order::rateDriver(unsigned rate)
{
	rateForTheDriver = rate;
	std::cout << "Driver has been rated!" << std::endl;
}

void Order::payDriver(double amount)
{
	if (paidMoneyForTheOrder != 0)
		throw std::invalid_argument("You have already paid for the ride!");
	if (amount < 0)
		throw std::invalid_argument("Cannot pay with negative digits!");

	paidMoneyForTheOrder = amount;
}

void Order::setAcceptedToTrue()
{
	isOrderAccepted = true;
}

void Order::setFinishedToTrue()
{
	isOrderFinished = true;
}

void Order::setMinutesUntilDriverComes(unsigned minutes)
{
	howManyMinutesUntilDriverComes = minutes;
}

void Order::setPaidMoney(double moneyPaid)
{
	paidMoneyForTheOrder = moneyPaid;
	std::cout << "Payment successdul!" << std::endl;
}

void Order::setIsMoneyAcceptedToTrue()
{
	isPaymentAcceptedByTheDriver = true;
}

void Order::deleteOrder()
{
	setIsCanceledToTrue();
}

void Order::setIsCanceledToTrue()
{
	isCanceled = true;
	std::cout << "Order cancelled!" << std::endl;
}

void Order::setDriver(Driver& driver)
{
	driverForTheOrder = &driver;
}

void Order::printOrder() const
{
	if (this->idOfOrder != -1) {
		std::cout << "Full name of the orderer: " << firstNameOfTheOrderer << " " << lastNameOfTheOrderer << std::endl <<
			"Pick-up address: ";
		currentAddress.printAddress();
		std::cout << std::endl << "Destination: ";
		destination.printAddress();
		std::cout << std::endl << "Count of passengers: " << countOfPassengers << std::endl <<
			"ID of order: " << idOfOrder << std::endl;
	}

	else
		std::cout << "No active order to show!" << std::endl;
}

void Order::setId(unsigned id)
{
	idOfOrder = id;
}

void Order::setSeenByDriver(unsigned index)
{
	seenByDrivers[index] = true;
}

const MyVector<bool>& Order::getSeen() const
{
	return seenByDrivers;
}

bool Order::getIsCanceled() const
{
	return isCanceled;
}

void Order::setFirstName(const MyString& firstName)
{
	firstNameOfTheOrderer = firstName;
}

void Order::setLastName(const MyString& lastName)
{
	lastNameOfTheOrderer = lastName;
}

void writeOrder(std::ofstream& file, const Order& order)
{
	file << "First name of the orderer: ";
	file << order.firstNameOfTheOrderer << std::endl;

	file << "Last name of the ordere: ";
	file << order.lastNameOfTheOrderer << std::endl;

	file << "Info about the pick-up address: " << std::endl;
	file << "Name: ";
	file << order.currentAddress.getNameOfAddress() << std::endl;
	file << "Coordinates: ";
	file << order.currentAddress.getXCoordinate() << ' ' << order.currentAddress.getYCoordinate() << std::endl;
	file << "Additional info:";
	file << order.currentAddress.getAdditionalInfo() << std::endl;

	file << "Info about the destination: " << std::endl;
	file << "Name: ";
	file << order.destination.getNameOfAddress() << std::endl;
	file << "Coordinates: ";
	file << order.destination.getXCoordinate() << ' ' << order.destination.getYCoordinate() << std::endl;
	file << "Additional info: ";
	file << order.destination.getAdditionalInfo() << std::endl;

	const int bufferSize = 20;
	char numberToChar[bufferSize];

	file << "Count of the passengers: ";
	turnNumbersToCharArr(order.countOfPassengers, numberToChar);
	file << numberToChar << std::endl;

	file << "ID of the order: ";
	turnNumbersToCharArr(order.idOfOrder, numberToChar);
	file << numberToChar << std::endl;

	file << "Seen by drivers array: ";
	for (unsigned i = 0; i < order.seenByDrivers.getSize(); i++) {
		file << order.seenByDrivers[i];
	}

	file << std::endl;

	file << "Is accepted: ";
	file << order.isOrderAccepted << std::endl;

	file << "ID of the driver: ";
	file << order.driverForTheOrder->getId() << std::endl;

	file << "Minutes Until driver comes: ";
	file << order.howManyMinutesUntilDriverComes << std::endl;

	file << "Rate of the driver: ";
	file << order.rateForTheDriver << std::endl;

	file << "Is order finished: ";
	file << order.isOrderFinished << std::endl;

	file << "Paid money for the order: ";
	file << order.paidMoneyForTheOrder << std::endl;

	file << "Is payment accepted by driver: ";
	file << order.isPaymentAcceptedByTheDriver << std::endl;

	file << "Is order cancelled: ";
	file << order.isCanceled << std::endl;
}