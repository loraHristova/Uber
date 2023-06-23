#include "Client.h"

Client::Client(const MyString& username, const MyString& password, const MyString& firstName, const MyString& lastName) : User(username, password, firstName, lastName) {}

void Client::setActiveOrder(Order* order)
{
	if (activeOrder)
		addOrderToHistory(*activeOrder);
	activeOrder = order;
}

void Client::checkOrder(unsigned id) const
{
	if (!checkValidityOfId(id))
		return;

	if (!activeOrder->isAccepted()) {
		std::cout << "Your order is waiting for an available car! Please check again in a few minutes!" << std::endl;
		return;
	}

	std::cout << "Your order has been accepted!" << std::endl
		<< "Driver: " << activeOrder->getDriver()->getFirstName() << " " <<
		activeOrder->getDriver()->getLastName() << std::endl <<
		"Car number: " << activeOrder->getDriver()->getCarNumber() <<
		std::endl << "Phone number: " << activeOrder->getDriver()->getPhoneNumber() << std::endl;
}

void Client::cancelOrder(unsigned id)
{
	if (!checkValidityOfId(id))
		return;

	if (!activeOrder->isFinished())
		activeOrder->deleteOrder();

	else
		std::cout << "Cannot cancel a finished order!" << std::endl;
}

void Client::pay(unsigned id, double amount)
{
	if (!checkValidityOfId(id))
		return;

	if (!activeOrder->isFinished())
		throw std::invalid_argument("The order hasn't been finished yet, you cannot pay now!");

	if (amount > money)
		throw std::invalid_argument("Please add money to your account, you do not have enough to pay this ride!");

	if(amount < 0)
		throw std::invalid_argument("You cannot enter a negative digit!");

	if(activeOrder->getPaidMoneyForTheOrder() != 0)
		throw std::invalid_argument("You have already paid!");

	activeOrder->setPaidMoney(amount);
	money -= amount;
	std::cout << "Balance left: " << this->money << std::endl;
}

void Client::rate(const MyString& firstNameOfDriver, const MyString& lasNameOfDriver, unsigned rating)
{
	if (rating > 5 || rating < 1) 
		throw std::invalid_argument("The rating can be between 1 and 5!");

	for (unsigned i = 0; i < historyOfFinishedOrders.getSize(); i++) {
		if (historyOfFinishedOrders[i].getDriver()->getFirstName() == firstNameOfDriver && historyOfFinishedOrders[i].getDriver()->getLastName() == lasNameOfDriver)
			historyOfFinishedOrders[i].rateDriver(rating);
	}
}

void Client::addMoney(double amount)
{	
	if (amount < 0)
		throw std::invalid_argument("Cannot add a negative number to your account!");
	this->money = amount;
	std::cout << amount << " have been added to your account!" << std::endl;
}

bool Client::checkValidityOfId(unsigned id) const
{
	if (activeOrder->getIdOfOrder() != id) {
		std::cout << "Wrong ID!" << std::endl;
		return false;
	}

	return true;
}

void Client::printInfo() const
{

	std::cout << "Username: " << username << std::endl <<
		"Full name: " << firstName << " " << lastName << std::endl <<
		"Amount: " << money << std::endl <<
		"Active order: ";
	activeOrder->printOrder();
	std::cout << std::endl;
}

const Order* Client::getActiveOrder() const
{
	return activeOrder;
}

void Client::addOrderToHistory(const Order& order)
{
	if (order.isFinished())
		historyOfFinishedOrders.pushBack(order);

	else
		throw std::invalid_argument("The order need to be finished to add to the history of orders!");
}


void writeClientToFile(std::ofstream& file, const Client& client)
{
	const int bufferSize = 20;

	char numberToChar[bufferSize];

	file << "Name of Client: ";
	file << client.firstName <<  ' '  << client.lastName << std::endl;

	file << "ID of active order: ";
	turnNumbersToCharArr(client.activeOrder->getIdOfOrder(), numberToChar);
	file << numberToChar << '\n';

	file << "Count of finished orders: ";
	turnNumbersToCharArr(client.countOfFinishedOrders, numberToChar);
	file << numberToChar << '\n';

	file << "IDs of finished orders: ";
	for (unsigned i = 0; i < client.countOfFinishedOrders; i++) {
		turnNumbersToCharArr(client.historyOfFinishedOrders[i].getIdOfOrder(), numberToChar);
		file << numberToChar << ' ';
	}
}