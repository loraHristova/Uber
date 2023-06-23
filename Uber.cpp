#include "Uber.h"
#include <cstring>
#include <cmath>
#include <fstream>

void Uber::regClient(const MyString& username, const MyString& password, const MyString& firstName, const MyString& lastName)
{
	for (unsigned i = 0; i < countOfClients; i++)
	{
		if (strcmp(username.c_str(), clients[i].getUsername().c_str()) == 0)
			throw std::invalid_argument("This username is already taken! Choose another one!");
	}

	clients.pushBack(Client(username, password, firstName, lastName));
	countOfClients++;
}

void Uber::regDriver(const MyString& username, const MyString& password, const MyString& firstName, const MyString& lastName, unsigned carNumber, char* phoneNumber)
{

	for (unsigned i = 0; i < countOfDrivers; i++)
	{
		if (strcmp(username.c_str(), drivers[i].getUsername().c_str()) == 0)
			throw std::invalid_argument("This username is already taken! Choose another one!");
	}


	drivers.pushBack(Driver(username, password, firstName, lastName, carNumber, phoneNumber));
	drivers[drivers.getSize() - 1].setId(countOfDrivers++);
}

void Uber::loginClient(const MyString& username, const MyString& password)
{
	for (unsigned i = 0; i < countOfClients; i++) {
		if (strcmp(username.c_str(), clients[i].getUsername().c_str()) == 0 && clients[i].comparePasswords(password)) {
			loggedUser = &clients[i];
			return;
		}
	}

	throw std::invalid_argument("Wrong username or password!");
}

void Uber::loginDriver(const MyString& username, const MyString& password)
{
	for (unsigned i = 0; i < countOfDrivers; i++) {
		if (strcmp(username.c_str(), drivers[i].getUsername().c_str()) == 0 && drivers[i].comparePasswords(password)) {
			loggedUser = &drivers[i];
			return;
		}
	}

	throw std::invalid_argument("Wrong username or password!");
}

void Uber::logout()
{
	loggedUser = nullptr;
}

double Uber::calculateDistance(int x1, int y1, int x2, int y2) const
{
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

void Uber::sendMessageToTheClosestDriver(Order& currentOrder)
{
	unsigned idOfTheClosestDriver = 0;
	double distanceToDriver = 1000;

	if (countOfDrivers == 0) {
		currentOrder.setId(-1);
		throw std::invalid_argument("There are no drivers available! Please try later!");
	}

	for (unsigned i = 0; i < countOfDrivers; i++) {
		unsigned xCoordinateOfDriver = drivers[i].getCurrentAddress().getXCoordinate();
		unsigned yCoordinateOfDriver = drivers[i].getCurrentAddress().getYCoordinate();
		double currentDistance = calculateDistance(xCoordinateOfDriver, yCoordinateOfDriver, currentOrder.getCurrentAddress().getXCoordinate(), currentOrder.getCurrentAddress().getYCoordinate());
		
		bool a = !(currentOrder.getSeen()[i]);
		if (a && (currentDistance - distanceToDriver < 0.0000000001)) {
			distanceToDriver = currentDistance;
			idOfTheClosestDriver = i;
		}
	}

	drivers[idOfTheClosestDriver].addNewMessage(currentOrder);
	std::cout << "Order sent to driver!" << std::endl;
}

void Uber::clientCoresp(MyString& username, MyString& password)
{
	std::cout << "Username: ";
	std::cin >> username;
	std::cout << std::endl;

	std::cout << "Password: ";
	std::cin >> password;
	std::cout << std::endl;

	loginClient(username, password);
	std::cout << "Welcome, " << username << std::endl;

	Client* loggedClient = static_cast<Client*>(loggedUser);

	while (true) {
		int answer = -1;

		std::cout << "Please select:" << std::endl <<
			"1 for 'order'" << std::endl <<
			"2 for 'order check'" << std::endl <<
			"3 for 'cancel order'" << std::endl <<
			"4 for 'pay for order'" << std::endl <<
			"5 for 'rate the driver'" << std::endl <<
			"6 for 'add money to your account'" << std::endl <<
			"0 for logout" << std::endl;

		std::cout << "Answer: ";
		std::cin >> answer;

		if (answer == 1) {
			MyString namePickUp = "";
			MyString nameDes = "";
			int x1, y1, x2, y2, numberOfPassengers;
			MyString addInfoPickUp = "";
			MyString addInfoDes = "";
			char buffer[1024];
			std::cout << "Please enter name of the pick-up address: ";
			std::cin >> namePickUp;
			std::cout << std::endl;

			std::cout << "Please enter coordinates of the address: ";
			std::cin >> x1 >> y1;
			std::cout << std::endl;

			std::cout << "Please enter additional Info (if there is no additional info, enter '-'): ";
			std::cin >> addInfoPickUp;
			std::cout << std::endl;

			std::cout << "Please enter name of the destination: ";
			std::cin >> nameDes;
			std::cout << std::endl;

			std::cout << "Please enter coordinates of the destination: ";
			std::cin >> x2 >> y2;
			std::cout << std::endl;

			std::cout << "Please enter additional Info for the destination(if there is no additional info, enter '-'): ";
			std::cin >> addInfoDes;
			std::cout << std::endl;

			std::cout << "Please enter number of passengers: ";
			std::cin >> numberOfPassengers;
			std::cout << std::endl;

			Order currentOrder(loggedClient->getFirstName(), loggedClient->getLastName(), namePickUp, x1, y1, addInfoPickUp, nameDes, x2, y2, addInfoDes, numberOfPassengers, countOfDrivers);
			currentOrder.setId(countOfOrders);
			allMadeOrders.pushBack(currentOrder);

			loggedClient->setActiveOrder(&allMadeOrders[countOfOrders]);

			sendMessageToTheClosestDriver(allMadeOrders[countOfOrders++]);
		}

		else if (answer == 2) {
			loggedClient->checkOrder(loggedClient->getActiveOrder()->getIdOfOrder());
		}

		else if (answer == 3)
			loggedClient->cancelOrder(loggedClient->getActiveOrder()->getIdOfOrder());

		else if (answer == 4)
		{
			double amount = -1;
			std::cout << "Please enter the amount you wish to pay: ";
			std::cin >> amount;
			std::cout << std::endl;
			loggedClient->pay(loggedClient->getActiveOrder()->getIdOfOrder(), amount);
		}

		else if (answer == 5)
		{
			MyString firstName = "";
			MyString lastName = "";
			unsigned rate;
			std::cout << "Please enter the first name of the driver you wish to rate: ";
			std::cin >> firstName;
			std::cout << std::endl << "Please enter the last name of the driver you wish to rate:";
			std::cin >> lastName;
			std::cout << std::endl << "Please enter the rating you wish to leave (between 1 and 5): ";
			std::cin >> rate;
			std::cout << std::endl;
			loggedClient->rate(firstName, lastName, rate);
		}

		else if (answer == 6)
		{
			double amount;
			std::cout << "Please enter the amount you wish to enter in the system: ";
			std::cin >> amount;
			std::cout << std::endl;
			loggedClient->addMoney(amount);
		}

		else if (answer == 0)
		{
			unsigned answer;

			if (loggedClient->getActiveOrder()->isFinished() && loggedClient->getActiveOrder()->getPaidMoneyForTheOrder() == 0) {
				std::cout << "You wish to logout but your order has been finished but has not been paid! Please enter the amount you wish to pay: ";
				std::cin >> answer;

				while (answer <= 0) {
					std::cout << "You must pay more than 0!";
					std::cin >> answer;
				}

				loggedClient->pay(loggedClient->getActiveOrder()->getIdOfOrder(), answer);
			}
			
			logout();
			break;
		}
		else
			continue;
	}
}


void Uber::driverCoresp(MyString& username, MyString& password)
{
	std::cout << "Username: ";
	std::cin >> username;
	std::cout << std::endl;

	std::cout << "Password: ";
	std::cin >> password;
	std::cout << std::endl;

	loginDriver(username, password);
	std::cout << "Welcome, " << username << std::endl;

	Driver* loggedDriver = static_cast<Driver*>(loggedUser);

	while (true) {
		int answer = -1;

		std::cout << "Please select:" << std::endl <<
			"1 for 'change address'" << std::endl <<
			"2 for 'check messages'" << std::endl <<
			"3 for 'accept order'" << std::endl <<
			"4 for 'decline order'" << std::endl <<
			"5 for 'finish order'" << std::endl <<
			"6 for 'accept payment'" << std::endl <<
			"0 for logout" << std::endl;

		std::cout << "Answer: ";
		std::cin >> answer;
			if (answer == 1)
			{
				MyString name = "";
				char buffer[1024];
				int x, y;
				std::cout << "Please enter the name of the address you are on: ";
				std::cin >> std::ws;
				std::cin.getline(buffer, 1024);
				name = buffer;
				std::cout << std::endl << "Please enter the coordinates (x, y): ";
				std::cin >> x >> y;
				std::cout << std::endl;
				loggedDriver->setCurrentAddress(name, x, y);

				std::cout << "Successfully set address to: ";
				loggedDriver->getCurrentAddress().printAddress();
				std::cout << std::endl;
			}

			else if (answer == 2) {
				loggedDriver->checkMessages();

				std::cout << "Messeges checked!" << std::endl;
			}

			else if (answer == 3)
			{
				unsigned id, minutes;
				std::cout << "Please enter the ID of the order you want to accept: ";
				std::cin >> id;
				std::cout << std::endl << "Please enter how many minutes until you are on the address: ";
				std::cin >> minutes;
				std::cout << std::endl;
				loggedDriver->acceptOrder(id, minutes);

				if(loggedDriver->findMessage(id)->isAccepted())
					std::cout << "Order accepted!" << std::endl;

				else
					std::cout << "Error! Order is not accepted" << std::endl;
			}

			else if (answer == 4)
			{
				unsigned id;
				std::cout << "Please enter the ID of the order you want to decline: ";
				std::cin >> id;
				std::cout << std::endl;
				loggedDriver->declineOrder(id);

				for (unsigned i = 0; i < countOfOrders; i++)
				{
					if (allMadeOrders[i].getIdOfOrder() == id)
						sendMessageToTheClosestDriver(allMadeOrders[i]);
				}

				std::cout << "Order declined!" << std::endl;

			}

			else if (answer == 5)
			{
				unsigned id;
				std::cout << "Please enter the ID of the order you want to finish: ";
				std::cin >> id;
				std::cout << std::endl;
				loggedDriver->finishOrder(id);


				if (loggedDriver->findMessage(id)->isFinished())
					std::cout << "Order finished!" << std::endl;

				else
					std::cout << "Error! Order is not finished!" << std::endl;
			}

			else if (answer == 6)
			{
				unsigned id;
				std::cout << "Please enter the ID of the order you want to accept payment for: ";
				std::cin >> id;
				std::cout << std::endl;
				loggedDriver->acceptPayment(id);


				if (loggedDriver->findMessage(id)->isMoneyAccepted())
					std::cout << "Payment accepted" << std::endl;

				else
					std::cout << "Error! Payment has not been accepted!" << std::endl;
			}

			else if (answer == 0) {
				logout();

				if (!loggedUser)
					std::cout << "Successfully logged out" << std::endl;

				break;
			}

			else {
				std::cout << "Not a valid option!" << std::endl;
				continue;
			}
		
	}
}

	void Uber::usersInitLoginAndOptions()
	{
		try {

			std::cout << "Please enter the type of account you are going to be logging in (driver/client): " << std::endl;
			MyString type;
			std::cin >> type;

			MyString username;
			MyString password;

			if (type == "client") {
				clientCoresp(username, password);
			}

			else if (type == "driver") {
				driverCoresp(username, password);
			}
		}
		catch (const std::invalid_argument& err)
		{
			std::cout << "An error has occurred: " << err.what() << std::endl <<
			"Due to the error you have been logged out, please log in again!" << std::endl;
			logout();
			usersInitLoginAndOptions();
		}
		catch (...)
		{
			std::cout << "An unknown error has occured!" <<
			"Due to the error you have been logged out, please log in again!" << std::endl;
			logout();
			usersInitLoginAndOptions();
		}
	}

	void Uber::usersInitReg()
	{
		char type[7];

		std::cout << "Please enter the type of account you are going to be registering as (driver/client): " << std::endl;

		std::cin >> type;

		if (!strcmp(type, "client")) {
			try {
				MyString username;
				std::cout << "Please enter username: ";
				std::cin >> username;
				std::cout << std::endl << "Please enter password: ";
				MyString password;
				std::cin >> password;
				std::cout << std::endl << "Please enter your first name: ";
				MyString firstName;
				std::cin >> firstName;
				std::cout << std::endl << "Please enter your last name: ";
				MyString lastName;
				std::cin >> lastName;
				std::cout << std::endl;

				regClient(username, password, firstName, lastName);
				std::cout << "Registration complete!" << std::endl;
			}
			catch (const std::invalid_argument& err)
			{
				std::cout << "An error has occurred: " << err.what() << std::endl;
				usersInitReg();
			}
			catch (...)
			{
				std::cout << "An unknown error has occured!" << std::endl;
			}
		}

		else if (!strcmp(type, "driver")) {
			try {
				MyString username;
				std::cout << "Please enter username: ";
				std::cin >> username;
				std::cout << std::endl << "Please enter password: ";
				MyString password;
				std::cin >> password;
				std::cout << std::endl << "Please enter your first name: ";
				MyString firstName;
				std::cin >> firstName;
				std::cout << std::endl << "Please enter your last name: ";
				MyString lastName;
				std::cin >> lastName;
				std::cout << std::endl << "Please enter your car number: ";
				unsigned carNumber;
				std::cin >> carNumber;
				std::cout << std::endl << "Please enter your phone number: ";
				char phoneNumber[1024];
				std::cin >> phoneNumber;

				if (strlen(phoneNumber) != 10)
					throw std::invalid_argument("The phone number must be exactly 10 digits!");


				regDriver(username, password, firstName, lastName, carNumber, phoneNumber);
				std::cout << "Registration complete!" << std::endl;
			}
			catch (const std::invalid_argument& err)
			{
				std::cout << "An error has occurred: " << err.what() << std::endl;
				usersInitReg();
			}
			catch (...)
			{
				std::cout << "An unknown error has occured!" << std::endl;
			}
		}
	}

	void Uber::usersCoresp()
	{
		MyString choice = "-";

		while (choice != "0") {
			std::cout << "Please select one of the following options: " << std::endl <<
				"1 for 'registration'" << std::endl << "2 for 'login'" << std::endl << "0 for terminating the program"
				<< std::endl << "3 for calculating all the profit" << std::endl << "Your answer: ";
			std::cin >> choice;

			if (choice == "1")
				usersInitReg();

			else if (choice == "2")
				usersInitLoginAndOptions();

			else if (choice == "3") {
				unsigned moneySum = 0;
				for (unsigned i = 0; i < countOfOrders; i++) {
					moneySum += allMadeOrders[i].getPaidMoneyForTheOrder();
				}

				std::cout << "Profit until now: " << moneySum << std::endl;
			}

			else if (choice == "0") {
				std::ofstream fileToWriteInOrders("Orders.txt", std::ios::trunc);
				std::ofstream fileToWriteInDrivers("Drivers.txt", std::ios::trunc);
				std::ofstream fileToWriteInClients("Clients.txt", std::ios::trunc);

				if (!fileToWriteInOrders.is_open()) {
					try {
						throw std::invalid_argument("Couldn't open file for Orders!");
					}
					catch (const std::invalid_argument& err)
					{
						std::cout << "An error occured: " << err.what() << std::endl;
					}
				}

				if (!fileToWriteInDrivers.is_open()) {
					try {
						throw std::invalid_argument("Couldn't open file for Drivers!");
					}
					catch (const std::invalid_argument& err)
					{
						std::cout << "An error occured: " << err.what() << std::endl;
					}
				}

				if (!fileToWriteInClients.is_open()) {
					try {
						throw std::invalid_argument("Couldn't open file for Clients!");
					}
					catch (const std::invalid_argument& err)
					{
						std::cout << "An error occured: " << err.what() << std::endl;
					}
				}

				writeClients(fileToWriteInClients);
				writeDrivers(fileToWriteInDrivers);
				writeAllMadeOrders(fileToWriteInOrders);

				fileToWriteInOrders.close();
				fileToWriteInDrivers.close();
				fileToWriteInClients.close();
				break;
			}

			else {
				std::cout << "No such option! Try again!" << std::endl;
				usersCoresp();
			}
		}
		
	}

	void Uber::writeAllMadeOrders(std::ofstream& file)
	{
		file << countOfOrders << std::endl;

		for (unsigned i = 0; i < countOfOrders; i++)
			writeOrder(file, allMadeOrders[i]);
	}

	void Uber::writeDrivers(std::ofstream& file)
	{
		file << countOfDrivers << std::endl;

		for (unsigned i = 0; i < countOfDrivers; i++)
			writeDriverToFile(file, drivers[i]);
	}

	void Uber::writeClients(std::ofstream& file)
	{
		file << countOfClients << std::endl;

		for (unsigned i = 0; i < countOfClients; i++)
			writeClientToFile(file, clients[i]);
	}
