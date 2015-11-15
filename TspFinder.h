#ifndef TSP_FINDER_H
#define TSP_FINDER_H

#include "Graph.h"
#include "Pokemon.h"

#include <algorithm>

#include <vector>

using namespace std;


class TspFinder
{
private:
    unsigned m_size;
    unsigned m_graphWidth;

    double m_upperBound;
    unsigned m_endLimit;

    Graph<double> m_graph;
    vector<unsigned> m_upperBoundPath;

    #ifdef ANALYZE
    vector<unsigned> m_prunes;
    #endif

    template <typename MASK>
    struct Node {
        unsigned index;
        MASK mask;
        double cost;
        double bound;

        Node(unsigned i, MASK m, double c, double b)
            : index(i), mask(m), cost(c), bound(b + c) { }
    };

    template <typename MASK>
    struct NodeCompare : public binary_function<Node<MASK>, Node<MASK>, bool> {
        inline bool operator()(const Node<MASK>& lhs, const Node<MASK>& rhs) {
            return lhs.bound < rhs.bound;
        }
    };

public:
    TspFinder(const vector<Pokemon>& pokemon);

    template <typename MASK>
    double lowerBound(unsigned start, MASK mask) const;

    template <typename MASK>
    void next(const Node<MASK>& node, unsigned added, vector<unsigned>& path);

    template <typename MASK>
    void finish(const Node<MASK>& node, unsigned added, vector<unsigned>& path);

    void find(ostream& os);
    
}; // class TspFinder

#endif // TSP_FINDER_H
