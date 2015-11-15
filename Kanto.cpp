#include "Kanto.h"

#include <exception>
#include <cfloat>
#include <climits>
#include <algorithm>

using namespace std;


Kanto::Kanto(istream& is)
	: m_count(0)
{
	int x, y;

	is >> m_count;

	m_pokemon.reserve(m_count);
	for (unsigned i = 0; i < m_count; ++i) {
		is >> x >> y;
		m_pokemon.emplace_back(x, y);
	}
}

double
Kanto::mst(vector<unsigned>& pv)
{
	if (m_count < 2) {
		return 0;
	}

	vector<bool> kv(m_count, false);
	vector<double> dv(m_count, DBL_MAX);

	unsigned parent = 0;
	double min;
	double total = 0;

	pv.resize(m_count, 0);

	kv[0] = true;
	dv[0] = 0.0;

	for (unsigned i = 1; i < m_count; ++i) {
		if (canWalkOrSurf(m_pokemon[parent], m_pokemon[i])) {
			dv[i] = edgeDistance(m_pokemon[0], m_pokemon[i]);
		}
	}

	unsigned count = 2;
	while (true) {
		min = DBL_MAX;
		for (unsigned i = 1; i < m_count; ++i) {
			if (!kv[i] && dv[i] < min) {
				min = dv[i];
				parent = i;
			}
		}

		total += min;

		if (count == m_count) {
			break;
		}

		kv[parent] = true;
		++count;

		for (unsigned i = 1; i < m_count; ++i) {
			if (!kv[i] && canWalkOrSurf(m_pokemon[parent], m_pokemon[i])) {
				min = edgeDistance(m_pokemon[parent], m_pokemon[i]);

				if (min < dv[i]) {
					dv[i] = min;
					pv[i] = parent;
				}
			}
		}
	}

	return total;
}

void
Kanto::mstCircuit(vector<unsigned>& nodes)
{
	vector<bool> kv(m_count, false);
	vector<unsigned long long> dv(m_count, ULLONG_MAX);
	vector<MstNode> pv(m_count);

	unsigned parent = 0;
	unsigned long long min;

	kv[0] = true;
	dv[0] = 0.0;

	for (unsigned i = 1; i < m_count; ++i) {
		pv[i].pokemon = i;
		pv[i].parent = &pv[0];
		dv[i] = edgeDistanceUnnormalized(m_pokemon[parent], m_pokemon[i]);
	}

	unsigned count = 2;
	while (true) {
		min = ULLONG_MAX;
		for (unsigned i = 0; i < m_count; ++i) {
			if (!kv[i] && dv[i] < min) {
				min = dv[i];
				parent = i;
			}
		}

		kv[parent] = true;
		pv[parent].parent->children.push_back(&pv[parent]);

		if (count == m_count) {
			break;
		}

		for (unsigned i = 0; i < m_count; ++i) {
			if (!kv[i]) {
				min = edgeDistanceUnnormalized(m_pokemon[parent], m_pokemon[i]);

				if (min < dv[i]) {
					dv[i] = min;
					pv[i].parent = &pv[parent];
				}
			}
		}

		++count;
	}

	kv.resize(0);
	dv.resize(0);

	nodes.clear();
	nodes.reserve(m_count);
	inOrder(&pv[0], nodes);
}

void
Kanto::inOrder(MstNode* root, vector<unsigned>& nodes)
{
	nodes.push_back(root->pokemon);
	for (auto child : root->children) {
		inOrder(child, nodes);
	}
}


double
Kanto::twoOpt(vector<unsigned>& nodes, const clock_t& endTime)
{
	clock_t iterationTime, currentTime;
	const unsigned count = nodes.size();

	vector<double> edges(count);
	for (unsigned i = 0; i < count - 1; ++i) {
		edges[i] = edgeDistance(m_pokemon[nodes[i]], m_pokemon[nodes[i + 1]]);
	}
	edges[count - 1] = edgeDistance(m_pokemon[nodes.back()], m_pokemon[nodes[0]]);
	
	auto iterV = nodes.begin();
	auto iterE = edges.begin();

	bool madeSwap;
	unsigned i1, i2, j1, j2;
	double i1j1, i2j2;

	do {
		iterationTime = clock();
		madeSwap = false;

		// corner case: i = last node
		i1 = count - 1;
		i2 = 0;
		j1 = 1;
		j2 = 2;

		// for non-adjacent edge j
		for ( ; j2 < count; ++j1, ++j2) {
			i1j1 = edgeDistance(m_pokemon[nodes[i1]], m_pokemon[nodes[j1]]);
			i2j2 = edgeDistance(m_pokemon[nodes[i2]], m_pokemon[nodes[j2]]);

			if (i1j1 + i2j2 < edges[i1] + edges[j1]) {
				reverse(iterV + j2, nodes.end());
				reverse(iterE + j2, iterE + i1);

				edges[i1] = i2j2;
				edges[j1] = i1j1;

				madeSwap = true;
			}
		}

		// non-corner cases
		i1 = 0, i2 = 1;

		// for edge i
		for ( ; i2 < count - 2; ++i1, ++i2) {
			j1 = i2 + 1;
			j2 = j1 + 1;
			
			// for non-adjacent edge j
			for ( ; j2 < count; ++j1, ++j2) {
				i1j1 = edgeDistance(m_pokemon[nodes[i1]], m_pokemon[nodes[j1]]);
				i2j2 = edgeDistance(m_pokemon[nodes[i2]], m_pokemon[nodes[j2]]);

				if (i1j1 + i2j2 < edges[i1] + edges[j1]) {
					reverse(iterV + i2, iterV + j2);
					reverse(iterE + i2, iterE + j1);

					edges[i1] = i1j1;
					edges[j1] = i2j2;

					madeSwap = true;
				}
			}
		}

		currentTime = clock();
		iterationTime = currentTime - iterationTime;
	} while (madeSwap && (iterationTime + currentTime < endTime));

	double total = 0;
	for (unsigned i = 0; i < edges.size(); ++i) {
		total += edges[i];
	}

	return total;
}
