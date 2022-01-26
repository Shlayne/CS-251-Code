/*
 * Name: Bruce Cosgrove
 * Class: CS 251
 * Date: 23 January 2022
 */

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
 *	Reads a set of space-separated strings from the user.
 * Inputs:
 *	name: The name of the input set.
 * Returns:
 *	The input set.
 */
std::set<std::string> getInputSet(char name)
{
	std::set<std::string> set;
	
	std::cout << "Please enter the space-separated roster of set " << name << ": ";
	std::string line;
	std::getline(std::cin, line);
	
	std::istringstream stream(line);
	for (std::string element; stream >> element && !element.empty(); set.insert(std::move(element)).second);
	return set;
}

// Set Operations

/*
 * Description:
 *	Finds all possible functions from set a to set b.
 * Inputs:
 *	a: The input set.
 *	b: The output set.
 * Returns:
 *	A vector with all possible functions, in set notation.
 */
template<typename T1, typename T2>
std::vector<std::set<std::pair<T1, T2>>> getAllFunctions(const std::set<T1>& a, const std::set<T2>& b)
{
	std::vector<std::set<std::pair<T1, T2>>> functions;
	if (!(a.empty() || b.empty()))
	{
		std::vector<typename std::set<T2>::const_iterator> iterators(a.size(), b.begin());
		bool finished = false;
		do
		{
			auto& function = functions.emplace_back();
			
			auto itIt = iterators.begin();
			for (const auto& elementA : a)
			{
				function.emplace(elementA, **itIt);
				++itIt;
			}
			
			finished = true;
			for (itIt = iterators.begin(); finished && itIt != iterators.end(); ++itIt)
			{
				auto& it = *itIt;
				if (++it != b.end())
					finished = false;
				else
					it = b.begin();
			}
		}
		while (!finished);
	}
	return functions;
}

/*
 * Description:
 *	Finds all injective functions from one set to another.
 * Inputs:
 *	allFunctions: The vector of all possible functions from one set to another.
 * Returns:
 *	A vector with all injective functions, in set notation.
 */
template<typename T1, typename T2>
std::vector<std::set<std::pair<T1, T2>>> getInjectiveFunctions(const std::vector<std::set<std::pair<T1, T2>>>& allFunctions)
{
	std::vector<std::set<std::pair<T1, T2>>> functions;
	
	for (const auto& function : allFunctions)
	{
		std::set<T2> range;
		for (const auto& mapping : function)
			if (!range.insert(mapping.second).second)
				break;
		if (range.size() == function.size())
			functions.push_back(function);
	}
	
	return functions;
}

/*
 * Description:
 *	Finds all surjective functions from one set to another.
 * Inputs:
 *	allFunctions: The vector of all possible functions from one set to another.
 * Returns:
 *	A vector with all surjective functions, in set notation.
 */
template<typename T1, typename T2>
std::vector<std::set<std::pair<T1, T2>>> getSurjectiveFunctions(const std::vector<std::set<std::pair<T1, T2>>>& allFunctions, size_t codomainSize)
{
	std::vector<std::set<std::pair<T1, T2>>> functions;
	
	for (const auto& function : allFunctions)
	{
		std::set<T2> range;
		for (const auto& mapping : function)
			range.insert(mapping.second);
		if (range.size() == codomainSize)
			functions.push_back(function);
	}
	
	return functions;
}

/*
 * Description:
 *	Finds all bijective functions from set a to set b.
 * Inputs:
 *	a: The input set.
 *	b: The output set.
 * Returns:
 *	A vector with all bijective functions, in set notation.
 */
template<typename T1, typename T2>
std::vector<std::set<std::pair<T1, T2>>> getBijectiveFunctions(const std::vector<std::set<std::pair<T1, T2>>>& injective, const std::vector<std::set<std::pair<T1, T2>>>& surjective)
{
	std::vector<std::set<std::pair<T1, T2>>> functions;
	if (injective.size() == surjective.size())
		std::set_intersection(injective.begin(), injective.end(), surjective.begin(), surjective.end(), std::inserter(functions, functions.begin()));
	return functions;
}

// Output

/*
 * Description:
 *	Prints the pair to ostr.
 * Inputs:
 *	ostr: The stream to output to.
 *	pair: The pair whose elements will be outputted to ostr.
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

/*
 * Description:
 *	Prints the vector to ostr.
 * Inputs:
 *	ostr: The stream to output to.
 *	vector: The vector whose elements will be outputted to ostr.
 * Returns:
 *	ostr to chain this operator.
 */
template<typename T>
std::ostream& operator<<(std::ostream& ostr, const std::vector<T>& vector)
{
	if (!vector.empty())
	{
		auto it = vector.begin();
		ostr << *it;
		while (++it != vector.end())
			ostr << ' ' << *it;
	}
	return ostr;
}

// Entry Point

int main()
{
	std::cout << '\n';
	std::cout << "******************************\n";
	std::cout << "* Program By: Bruce Cosgrove *\n";
	std::cout << "******************************\n";
	std::cout << '\n';
	
	std::set<std::string> A = getInputSet('A');
	std::set<std::string> B = getInputSet('B');
	
	std::cout << '\n';
	std::cout << "Given A=" << A << '\n';
	std::cout << "Given B=" << B << '\n';
	std::cout << '\n';
	
	auto allFunctions = getAllFunctions(A, B);
	std::cout << "There is/are " << allFunctions.size() << " function(s) from A to B: " << allFunctions << '\n';
	
	auto injectiveFunctions = getInjectiveFunctions(allFunctions);
	std::cout << "There is/are " << injectiveFunctions.size() << " injective function(s) from A to B: " << injectiveFunctions << '\n';
	
	auto surjectiveFunctions = getSurjectiveFunctions(allFunctions, B.size());
	std::cout << "There is/are " << surjectiveFunctions.size() << " surjective function(s) from A to B: " << surjectiveFunctions << '\n';
	
	auto bijectiveFunctions = getBijectiveFunctions(injectiveFunctions, surjectiveFunctions);
	std::cout << "There is/are " << bijectiveFunctions.size() << " bijective function(s) from A to B: " << bijectiveFunctions << '\n';
	
	return 0;
}
