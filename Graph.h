#ifndef GRAPH_H
#define GRAPH_H

#include "Kanto.h"
#include "Pokemon.h"

#include <iostream>
#include <string>
#include <cmath>
#include <cfloat>

using namespace std;


template <typename T>
class Graph
{
private:
	T** m_data;
	unsigned m_sizeMinus1;
	unsigned m_edges;

public:
	Graph(unsigned vertices) :
		m_sizeMinus1(vertices - 1),
		m_edges(vertices * m_sizeMinus1 / 2) {

		m_data = new T*[m_sizeMinus1];
		for (unsigned i = 0; i < m_sizeMinus1; ++i) {
			m_data[i] = new T[m_sizeMinus1 - i];
		}
	}

	~Graph() {
		for (unsigned i = 0; i < m_sizeMinus1; ++i) {
			delete[] m_data[i];
		}

		delete[] m_data;
	}

	inline T operator()(unsigned n1, unsigned n2) const {
		return (n1 < n2 ? m_data[n1][n2 - n1 - 1] : m_data[n2][n1 - n2 - 1]);
	}

	inline T* operator[](unsigned i) {
		return m_data[i];
	}

	inline T edge(unsigned i) const {
		return m_data[m_sizeMinus1 % i][m_sizeMinus1 / i];
	}

	inline unsigned sizeE() {
		return m_edges;
	}

	inline unsigned sizeV() {
		return m_sizeMinus1 + 1;
	}
}; // class Graph


#endif // GRAPH_H