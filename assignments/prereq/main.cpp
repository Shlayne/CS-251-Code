/*
 * Name: Bruce Cosgrove
 * Class: CS 251
 * Date: 11 January 2022
 */

#include <iostream>
#include <string>
#include <stdexcept>

static constexpr int minExclusive = 1;
static constexpr int maxExclusive = 1 << 10;

/*
 * Inputs:
 *	string: A preexisting string to read into.
 *	number: If successful, the inputted number, otherwise unchanged.
 * Returns:
 *	If the user input an integer within the specified bounds.
 */
bool tryConvert(std::string& string, int& number)
{
	try
	{
		std::getline(std::cin, string);
		number = std::stoi(string);
		if (number <= minExclusive || number >= maxExclusive)
			throw std::out_of_range("");
		return true;
	}
	catch (std::invalid_argument&)
	{
		std::cout << "What you entered was not a number";
	}
	catch (std::out_of_range&)
	{
		std::cout << "The number you entered was out of range";
	}
	
	std::cout << ". Please e";
	return false;
}

/*
 * Returns:
 *	The number inputted by the user.
 */
int getNumber()
{
	int number;
	std::string line;
	std::cout << 'E';
	do
		std::cout << "nter a number from " << minExclusive << ", exclusive, to " << maxExclusive << ", exclusive: ";
	while (!tryConvert(line, number));
	return number;
}

int main()
{
	std::cout << '\n';
	std::cout << "******************************\n";
	std::cout << "* Program By: Bruce Cosgrove *\n";
	std::cout << "******************************\n";
	std::cout << '\n';
	
	int number = getNumber();
	int sum = number * (number + 1) / 2;
	std::cout << "The sum from 1 to " << number << " is " << sum << ".\n";
	return 0;
}
