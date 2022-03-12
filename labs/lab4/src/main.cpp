/*
 * Name: Bruce Cosgrove
 * Class: CS 251
 * Date: 13 March 2022
 */

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <list>
#include <set>
#include <vector>

// Structures and Classes

struct Edge
{
	uint32_t startIndex = 0;
	uint32_t endIndex = 0;
	uint32_t weight = 0;
};

bool operator<(const Edge& edge1, const Edge& edge2)
{
	// VERY IMPORTANT:
	// Using <= here instead of < allows for
	// edges with the same weight to be counted
	// as different edges when inserting them
	// into a std::set. This can and does slightly
	// change the MST's edges, but the weight
	// will always be minimized, which was the
	// original goal. This happens because a graph
	// can have multiple distinct MST's, i.e. the
	// same minimized weight, but some edges may
	// exist in some and not in others.
	return edge1.weight <= edge2.weight;
}

class WeightedAdjacencyMatrix
{
public:
	WeightedAdjacencyMatrix(const std::list<Edge>& edges)
	{
		for (const auto& edge : edges)
		{
			uint32_t size = std::max(edge.startIndex, edge.endIndex);
			if (m_Size < size)
				m_Size = size;
		}
		
		m_Size++;
		m_WAM = new uint32_t[m_Size * m_Size]();
		
		for (const auto& edge : edges)
		{
			(*this)(edge.startIndex, edge.endIndex) = edge.weight;
			(*this)(edge.endIndex, edge.startIndex) = edge.weight;
		}
	}
	
	WeightedAdjacencyMatrix(uint32_t size)
		: m_Size(size), m_WAM(new uint32_t[m_Size * m_Size]()) {}
	
	~WeightedAdjacencyMatrix()
	{
		delete[] m_WAM;
	}
public:
	uint32_t& operator()(uint32_t row, uint32_t column) { return m_WAM[row * m_Size + column]; }
	uint32_t operator()(uint32_t row, uint32_t column) const { return m_WAM[row * m_Size + column]; }
	uint32_t size() const noexcept { return m_Size; }
private:
	uint32_t m_Size = 0;
	uint32_t* m_WAM;
};

std::ostream& operator<<(std::ostream& ostream, const WeightedAdjacencyMatrix& wam)
{
	for (uint32_t row = 0; row < wam.size(); row++)
	{
		for (uint32_t column = 0; column < wam.size(); column++)
		{
			// A little bit of ANSI to make it easier to see non-zero weights.
			uint32_t weight = wam(row, column);
			char attribute = weight ? '1' : '2';
			ostream << "\033[" << attribute << 'm' << std::right << std::setw(8) << weight << "\033[0m";
		}
		ostream << '\n';
	}
	return ostream;
}

// User Input

std::list<Edge> getRawInput()
{
	std::cout << "Enter the vertices and the weight of each edge in the form \"a b c\" where:\n";
	std::cout << "\ta is the starting vertex index, non-negative\n";
	std::cout << "\tb is the ending vertex index, non-negative\n";
	std::cout << "\tc is the edge weight, positive\n";
	std::cout << "Enter -1 to stop inputting.\n";
	
	std::list<Edge> edges;
	Edge edge;
	while (true)
	{
		std::cin >> edge.startIndex;
		if (edge.startIndex == -1)
			return edges;
		std::cin >> edge.endIndex >> edge.weight;
		edges.push_back(edge);
	}
}

// Kruskal

struct KVertex
{
	KVertex() : parent(this) {}
	
	KVertex* getParent()
	{
		return parent == this ? this : parent->getParent();
	}
	
	KVertex* parent;
};

WeightedAdjacencyMatrix findMSTKruskal(const WeightedAdjacencyMatrix& wam, uint32_t& totalWeight)
{
	totalWeight = 0;
	WeightedAdjacencyMatrix mst(wam.size());
	
	// Initialize vertices.
	std::vector<KVertex> vertices(wam.size());
	
	// Get all edges in the upper triangle and automatically sort using std::set.
	std::set<Edge> edges;
	for (uint32_t row = 0; row < wam.size() - 1; row++)
		for (uint32_t column = row + 1; column < wam.size(); column++)
			if (uint32_t weight = wam(row, column); weight)
				edges.insert({ row, column, weight });
	
	// Add as many edges as possible while making sure to not create a circuit.
	uint32_t edgeCount = 0;
	for (const auto& edge : edges)
	{
		KVertex* start = &vertices[edge.startIndex];
		KVertex* end = &vertices[edge.endIndex];
		KVertex* startParent = start->getParent();
		
		if (startParent != end->getParent())
		{
			std::cout << "Adding Edge: " << edge.startIndex << '-' << edge.endIndex << ':' << edge.weight << '\n';
			
			mst(edge.startIndex, edge.endIndex) = edge.weight;
			mst(edge.endIndex, edge.startIndex) = edge.weight;
			totalWeight += edge.weight;
			
			end->parent = startParent;
			if (++edgeCount >= vertices.size() - 1)
				break;
		}
	}
	
	return mst;
}

// Prim

WeightedAdjacencyMatrix findMSTPrim(const WeightedAdjacencyMatrix& wam, uint32_t& totalWeight)
{
	totalWeight = 0;
	WeightedAdjacencyMatrix mst(wam.size());
	
	std::vector<bool> visited(wam.size(), false);
	std::vector<uint32_t> parents(wam.size(), 0);
	std::vector<uint32_t> weights(wam.size(), -1);
	weights[0] = 0;
	
	for (uint32_t i = 0, edgeCount = 0; edgeCount < wam.size() - 1; edgeCount++)
	{
		uint32_t minIndex = 0;
		uint32_t minWeight = -1;
		for (uint32_t j = 0; j < wam.size(); j++)
		{
			if (!visited[j] && weights[j] < minWeight)
			{
				minWeight = weights[j];
				minIndex = j;
			}
		}
		
		visited[minIndex] = true;
		
		for (uint32_t j = 0; j < wam.size(); j++)
		{
			if (uint32_t weight = wam(minIndex, j); !visited[j] && weight && weight < weights[j])
			{
				parents[j] = minIndex;
				weights[j] = weight;
			}
		}
	}
	
	for (uint32_t i = 0; i < wam.size(); i++)
	{
		uint32_t parent = parents[i];
		uint32_t weight = wam(parent, i);
		if (weight != 0)
		{
			std::cout << "Adding Edge: " << parent << '-' << i << ':' << weight << '\n';
			
			mst(parent, i) = weight;
			mst(i, parent) = weight;
			totalWeight += weight;
		}
	}
	
	return mst;
};

// Entry Point

int main()
{
	std::cout << '\n';
	std::cout << "******************************\n";
	std::cout << "* Program By: Bruce Cosgrove *\n";
	std::cout << "******************************\n";
	std::cout << '\n';
	
	WeightedAdjacencyMatrix wam(getRawInput());
	std::cout << "\nInputted WAM:\n" << wam << '\n';
	
	{
		uint32_t weightKruskal;
		WeightedAdjacencyMatrix mstKruskal = findMSTKruskal(wam, weightKruskal);
		std::cout << "MST via Kruskal's Algorithm: weight=" << weightKruskal << ", WAM:\n" << mstKruskal << '\n';
	}
	
	{
		uint32_t weightPrim;
		WeightedAdjacencyMatrix mstPrim = findMSTPrim(wam, weightPrim);
		std::cout << "MST via Prim's Algorithm: weight=" << weightPrim << ", WAM:\n" << mstPrim << '\n';
	}
	
	return 0;
}
