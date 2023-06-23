#include "HelperFunctions.h"

unsigned numbersLength(int number)
{
	unsigned counter = 0;

	if (number == 0)
		return counter;

	while (number != 0) {
		number /= 10;
		counter++;
	}

	return counter;
}
 
char convertDigitToChar(unsigned digit) {
	if (digit > 9 || digit < 0)
		return '0';
	return digit + '0';
}


void turnNumbersToCharArr(int number, char* numberToChar)
{
	unsigned lenOfNum = numbersLength(number);

	for (int i = lenOfNum - 1; i >= 0; i--) {
		unsigned currDigit = number % 10;
		numberToChar[i] = convertDigitToChar(currDigit);
		number /= 10;
	}

	numberToChar[lenOfNum] = '\0';
}

