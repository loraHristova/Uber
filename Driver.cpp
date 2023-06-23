#include "Driver.h"

Driver::Driver(const MyString& username, const MyString& password, const MyString& firstName, const MyString& lastName, unsigned carNumber,const char* phoneNumber) : User(username, password, firstName, lastName), carNumber(carNumber)
{
	messages = new Order * [capacity];

	this->phoneNumber = phoneNumber;
}

void Driver::setCurrentAddress(MyString name, unsigned x, unsigned y)
{
	currentAdress.setName(name);
	currentAdress.setCoordinates(x, y);
}

void Driver::checkMessages() const
{
	bool isThereANewMessage = false;

	for (unsigned i = 0; i < countOfMessages; i++) {
		if (!messages[i]->isFinished() && !messages[i]->getIsCanceled()) {
			std::cout << messages[i]->getFirstNameOfTheOrder() << " " <<
				messages[i]->getLastNameOfTheOrder() << "has made an order from: " << std::endl;
			messages[i]->getCurrentAddress().printAddress();
			std::cout << "To: " << std::endl;
			messages[i]->getDestination().printAddress();
			std::cout << "ID: " << messages[i]->getIdOfOrder() << std::endl;
			isThereANewMessage = true;
		}

		if (messages[i]->getIsCanceled()) {
			std::cout << "ORDER ";
			messages[i]->getCurrentAddress().printAddress();
			std::cout << " - ";
			messages[i]->getDestination().printAddress();
			std::cout << " HAS BEEN CANCELLED!" << std::endl;
			messages[i] = nullptr;
		}
	}

	if (!isThereANewMessage)
		std::cout << "No new messages!" << std::endl;
}

const Order* Driver::findMessage(unsigned id) const
{
	for (unsigned i = 0; i < countOfMessages; i++)
	{
		if (messages[i]->getIdOfOrder() == id)
			return messages[i];
	}

	return nullptr;
}

void Driver::acceptOrder(int id, unsigned minutes)
{
	checkValidityOfId(id);

	for (unsigned i = 0; i < countOfMessages; i++)
	{
		if (messages[i]->getIdOfOrder() == id) {
			messages[i]->setAcceptedToTrue();
			messages[i]->setMinutesUntilDriverComes(minutes);
			messages[i]->setDriver(*this);
			messages[i]->setSeenByDriver(this->id);
		}
	}
}

void Driver::declineOrder(int id)
{
	checkValidityOfId(id);

	for (unsigned i = 0; i < countOfMessages; i++)
	{
		if (messages[i]->getIdOfOrder() == id) {
			messages[i]->setSeenByDriver(this->id);
			deleteMessage(i);
		}
	}
}

void Driver::deleteMessage(unsigned index)
{
	if (index == countOfMessages - 1) 
		messages[index] = nullptr;

	else {
		while (index < countOfMessages - 1) {
			messages[index] = messages[index + 1];
			index++;
		}

		messages[index] = nullptr;
	}

	countOfMessages--;

}

void Driver::finishOrder(int id)
{
	checkValidityOfId(id);

	for (unsigned i = 0; i < countOfMessages; i++)
	{
		if (messages[i]->getIdOfOrder() == id) {
			if (messages[i]->isFinished())
				throw std::invalid_argument("This order was already marked as finished!");
			messages[i]->setFinishedToTrue();
			currentAdress = messages[i]->getDestination();
		}
	}
}

void Driver::acceptPayment(int id)
{
	checkValidityOfId(id);

	for (unsigned i = 0; i < countOfMessages; i++)
	{
		if (messages[i]->getIdOfOrder() == id) {
			if (messages[i]->getPaidMoneyForTheOrder() == 0) {
				std::cout << "The client hasn't paid yet!" << std::endl;
				return;
			}

			if (!messages[i]->isMoneyAccepted()) {
				money = messages[i]->getPaidMoneyForTheOrder();
				std::cout << "Balance: " << this->money << std::endl;
				messages[i]->setIsMoneyAcceptedToTrue();
				std::cout << "Payment has been accepted!" << std::endl;
				return;
			}

		}
	}

	throw std::invalid_argument("The money have already been accepted!");
}

void Driver::updateRating()
{
	unsigned averageRating = 0;

	for (unsigned i = 0; i < countOfMessages; i++)
		averageRating += messages[i]->getDriversRatingForTheOrder();

	this->rating = averageRating / countOfMessages;
}

const MyString& Driver::getFirstName() const
{
	return this->firstName;
}

const MyString& Driver::getLastName() const
{
	return this->lastName;
}

unsigned Driver::getCarNumber() const
{
	return this->carNumber;
}

const MyString& Driver::getPhoneNumber() const
{
	return phoneNumber;
}

bool Driver::checkValidityOfId(unsigned id) const
{
	for (unsigned i = 0; i < countOfMessages; i++) {
		if (messages[i]->getIdOfOrder() == id)
			return true;
	}

	throw std::invalid_argument("Wrong order ID!");
}

void Driver::printInfo() const
{
	std::cout << "Driver" << std::endl << "Username: " << username << std::endl <<
		"Full name: " << firstName << " " << lastName << std::endl <<
		"Amount: " << money << std::endl <<
		"Car number: " << carNumber << std::endl <<
		"Phone number: " << phoneNumber << std::endl <<
		"Rating: " << rating << std::endl <<
		"Current address: ";
	currentAdress.printAddress();
	std::cout << std::endl;
}

const Address& Driver::getCurrentAddress() const
{
	return currentAdress;
}

void Driver::addNewMessage(Order& newMessage)
{
	if (countOfMessages == capacity)
		resize();

	messages[countOfMessages++] = &newMessage;
}

void Driver::setId(unsigned id) {
	this->id = id;
}

const Order* Driver::getMessages() const
{
	return *messages;
}

unsigned Driver::getId() const
{
	return id;
}

void Driver::free()
{
	delete[] messages;
}

void Driver::resize()
{
	capacity *= 2;

	Order** newMessages = new Order * [capacity];

	for (unsigned i = 0; i < countOfMessages; i++)
		newMessages[i] = messages[i];

	delete[] messages;

	messages = newMessages;
}

Driver::~Driver()
{
	free();
}

void Driver::copyFrom(const Driver& other)
{
	currentAdress = other.currentAdress;
	rating = other.rating;
	carNumber = other.carNumber;
	phoneNumber = other.phoneNumber;
	id = other.id;

	countOfMessages = other.countOfMessages;
	capacity = other.capacity;
	messages = new Order * [capacity];

	for (unsigned i = 0; i < countOfMessages; i++)
		messages[i] = other.messages[i];
}

void Driver::moveFrom(Driver&& other)
{
	currentAdress = other.currentAdress;
	rating = other.rating;
	carNumber = other.carNumber;
	phoneNumber = other.phoneNumber;
	id = other.id;

	countOfMessages = other.countOfMessages;
	capacity = other.capacity;
	messages = new Order * [capacity];

	for (unsigned i = 0; i < countOfMessages; i++) {
		messages[i] = other.messages[i];
		other.messages[i] = nullptr;
	}
}

Driver::Driver(const Driver& other) : User(other)
{
	copyFrom(other);
}

Driver& Driver::operator=(const Driver& other)
{
	if (this != &other)
	{
		User::operator=(other);
		free();
		copyFrom(other);
	}

	return *this;
}


Driver& Driver::operator=(Driver&& other)
{
	if (this != &other)
	{
		User::operator=(std::move(other));
		free();
		moveFrom(std::move(other));
	}

	return *this;
}


Driver::Driver(Driver&& other) : User(std::move(other))
{
	moveFrom(std::move(other));
}


void writeDriverToFile(std::ofstream& file, const Driver& driver)
{
	file << "Name of the driver: ";
	file << driver.firstName << ' ' << driver.lastName << std::endl;

	file << "Balance: " << driver.money << std::endl;

	file << "Current location of the driver: ";
	file << driver.currentAdress.getNameOfAddress() << std::endl;
	file << "Coordinates: ";
	file << driver.currentAdress.getXCoordinate() << ' ' << driver.currentAdress.getYCoordinate() << std::endl;

	const int buffer = 20;

	char numberToChar[buffer];

	file << "Driver's rating: ";
	turnNumbersToCharArr(driver.rating, numberToChar);
	file << numberToChar << std::endl;

	file << "Number of the driver's car: ";
	turnNumbersToCharArr(driver.carNumber, numberToChar);
	file << numberToChar << std::endl;
	file << driver.phoneNumber << std::endl;

	file << "Driver's ID: ";
	turnNumbersToCharArr(driver.id, numberToChar);
	file << numberToChar << std::endl;

	file << "Count of driver's messages: ";
	turnNumbersToCharArr(driver.countOfMessages, numberToChar);
	file << numberToChar << std::endl;

	file << "IDs of the driver's orders: ";
	for (unsigned i = 0; i < driver.countOfMessages; i++) {
		Order* order = driver.messages[i];
		turnNumbersToCharArr(order->getIdOfOrder(), numberToChar);
		file << numberToChar << ' ';
	}
	file << std::endl;
}