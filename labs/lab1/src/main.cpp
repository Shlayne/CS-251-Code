/*
 * Name: Bruce Cosgrove
 * Class: CS 251
 * Date: 18 January 2022
 */

#include <gmpxx.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <string>
#include <vector>

// User Input

/*
 * Description:
 *	Reads the max cardinality of the input sets from the user.
 * Returns:
 *	An integer in the range [2, 1024).
 */
int getInputSetMaxCardinality()
{
	auto retry = []()
	{
		std::cout << "Invalid cardinality. ";
		return true;
	};
	
	int n = 0;
	std::string line;
	
	do
	{
		std::cout << "Please enter the max cardinality of the input sets, within the range [2, 1024): ";
		std::getline(std::cin, line);
		
		try
		{
			n = std::stoi(line);
			if (n < 2 || n >= (1 << 10))
				n = 0;
		}
		catch (...) {}
	}
	while (n == 0 && retry());
	
	return n;
}

/*
 * Description:
 *	Reads a set of space-separated strings from the user.
 * Inputs:
 *	n: The input set cardinality.
 *	name: The name of the input set.
 * Returns:
 *	The input set.
 */
std::set<std::string> getInputSet(int n, char name)
{
	std::set<std::string> set;
	
	std::cout << "Please enter the space-separated roster of set " << name << ": ";
	std::string line;
	std::getline(std::cin, line);
	
	std::istringstream stream(line);
	for (std::string element; n > 0 && stream >> element && !element.empty();)
		if (set.insert(std::move(element)).second)
			n--;
	
	return set;
}

// Set Operations

/*
 * Description:
 *	Calculates the cross product of A and B.
 * Inputs:
 *	A: A set.
 *	B: Another set.
 * Returns:
 *	The cross product of A and B.
 */
template<typename E>
std::set<std::pair<E, E>> crossProduct(const std::set<E>& A, const std::set<E>& B)
{
	std::set<std::pair<E, E>> set;
	for (const auto& elementA : A)
		for (const auto& elementB : B)
			set.emplace(elementA, elementB);
	return set;
}

// Output

/*
 * Description:
 *	Prints the pair to ostr.
 * Inputs:
 *	ostr: The stream to output to.
 *	pair: The Pair whose elements will be outputted to ostr.
 * Returns:
 *	ostr to chain this operator.
 */
template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& ostr, const std::pair<T1, T2>& pair)
{
	return ostr << '(' << pair.first << ',' << pair.second << ')';
}

/*
 * Description:
 *	Prints the set to ostr.
 * Inputs:
 *	ostr: The stream to output to.
 *	set: The set whose elements will be outputted to ostr.
 * Returns:
 *	ostr to chain this operator.
 */
template<typename E>
std::ostream& operator<<(std::ostream& ostr, const std::set<E>& set)
{
	ostr << '{';
	if (!set.empty())
	{
		auto it = set.begin();
		ostr << *it;
		while (++it != set.end())
			ostr << ',' << *it;
	}
	return ostr << '}';
}

// Entry Point

int main()
{
	std::cout << '\n';
	std::cout << "******************************\n";
	std::cout << "* Program By: Bruce Cosgrove *\n";
	std::cout << "******************************\n";
	std::cout << '\n';
	
	int n = getInputSetMaxCardinality();
	std::set<std::string> A = getInputSet(n, 'A');
	std::set<std::string> B = getInputSet(n, 'B');
	
	std::cout << '\n';
	std::cout << "Given A=" << A << '\n';
	std::cout << "Given B=" << B << '\n';
	std::cout << '\n';
	
	{
		std::set<std::string> set;
#define PRINT(a, op, b) \
		std::set_##op(a.begin(), a.end(), b.begin(), b.end(), std::inserter(set, set.begin())); \
		std::cout << "The " #op " of " #a " and " #b ": cardinality=" << set.size() << ", " << set << '\n'; \
		set.clear()
		
		PRINT(A, intersection, B);
		PRINT(A, union, B);
		PRINT(A, difference, B);
		PRINT(B, difference, A);
		PRINT(A, symmetric_difference, B);
#undef PRINT
	}
	
	auto AcrossProductB = crossProduct(A, B);
	std::cout << "Cross product of A and B: cardinality=" << AcrossProductB.size() << ", " << AcrossProductB << '\n';
	
	// 2^(1023^2) is way too large to fit in 64 bits, so use gmp's big int.
	mpz_class crossProductPowerSetCardinality = 1;
	crossProductPowerSetCardinality <<= AcrossProductB.size();
	std::cout << "Cardinality of the power set of the cross product of A and B: " << crossProductPowerSetCardinality << '\n';
	
	return 0;
}
