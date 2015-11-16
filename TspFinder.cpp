#include "TspFinder.h"

#include <iostream>
#include <string>
#include <cfloat>
#include <climits>

#ifdef ANALYZE
#include <ctime>
#endif


TspFinder::TspFinder(const vector<Pokemon>& pokemon)  :
        m_size(pokemon.size()),
        m_graphWidth(m_size - 1),
        m_upperBound(DBL_MAX),
        m_endLimit(5 < m_size ? m_size - 5 : 0),
        m_graph(m_size)
{
    for (unsigned i = 0; i < m_graphWidth; ++i) {
        for (unsigned j = i + 1; j < m_size; ++j) {
            m_graph[i][j - i - 1] = sqrt(
                pow(pokemon[i].xPos - pokemon[j].xPos, 2) +
                pow(pokemon[i].yPos - pokemon[j].yPos, 2));
        }
    }
}

template <typename MASK>
double TspFinder::lowerBound(unsigned start, MASK mask) const
{
    vector<double> dv(m_size, DBL_MAX);

    unsigned parent = 0;
    double min1 = DBL_MAX, min2 = DBL_MAX, temp, total;

    MASK done = (1 << m_size) - 1;

    dv[0] = 0.0;

    for (unsigned i = 1; i < m_size; ++i) {
        if ((mask & (1 << i)) == 0) {
            parent = i;

            temp = m_graph(0, i);
            if (temp < min1) {
                min1 = temp;
            }

            temp = m_graph(start, i);
            if (temp < min2) {
                min2 = temp;
            }
        }
    }

    total = min1 + min2;

    mask |= (1 << parent);
    while (mask < done) {
        for (unsigned i = 1; i < m_size; ++i) {
            if ((mask & (1 << i)) == 0) {
                min1 = m_graph(parent, i);

                if (min1 < dv[i]) {
                    dv[i] = min1;
                }
            }
        }

        min1 = DBL_MAX;
        for (unsigned i = 1; i < m_size; ++i) {
            if ((mask & (1 << i)) == 0 && dv[i] < min1) {
                min1 = dv[i];
                parent = i;
            }
        }

        total += min1;
        mask |= (1 << parent);
    }

    return total;
}

template <typename MASK>
void TspFinder::next(const Node<MASK>& node, unsigned added, vector<unsigned>& path)
{
    if (m_endLimit < added) {
        finish(node, added, path);
    }
    else {
        vector<Node<MASK>> nodes;
        for (unsigned i = 1; i < m_size; ++i) {
            MASK nextMask = node.mask | (1 << i);

            if (nextMask != node.mask) {
                nodes.emplace_back(i, nextMask, node.cost + m_graph(node.index, i), lowerBound(i, nextMask));
            }
        }

        std::sort(nodes.begin(), nodes.end(), NodeCompare<MASK>());

        for (unsigned i = 0; i < nodes.size(); ++i) {
            if (m_upperBound < nodes[i].bound) {
                #ifdef ANALYZE
                m_prunes[added] += nodes.size() - i;
                #endif

                return;
            }

            path[added] = nodes[i].index;
            next(nodes[i], added + 1, path);
        }
    }
}

template <typename MASK>
void TspFinder::finish(const Node<MASK>& node, unsigned added, vector<unsigned>& path)
{
    if (added == m_size) {
        double cost = node.cost + m_graph(path[0], node.index);

        if (cost < m_upperBound) {
            m_upperBound = cost;
            m_upperBoundPath = path;
        }
    }
    else {
        for (unsigned i = 1; i < m_size; ++i) {
            MASK nextMask = node.mask | (1 << i);

            if (nextMask != node.mask) {
                path[added] = i;
                finish(Node<MASK>(i, nextMask, node.cost + m_graph(node.index, i), 0.0), added + 1, path);
            }
        }
    }
}

void TspFinder::find(ostream& os)
{
    #ifdef ANALYZE
    m_prunes.resize(m_size, 0U);
    clock_t start = clock();
    #endif

    vector<unsigned> path(m_size, 0U);
    m_upperBound = DBL_MAX;

    if (m_size < 8) {
        next(Node<unsigned char>(0, 1, 0, 0), 1U, path);
    }
    else if (m_size < 32) {
        next(Node<unsigned int>(0, 1, 0, 0), 1U, path);
    }
    else {
        next(Node<unsigned long long>(0, 1, 0, 0), 1U, path);
    }

    os << m_upperBound << "\n";
    for (unsigned p : m_upperBoundPath) {
        os << p << " ";
    }

    #ifdef ANALYZE
    clock_t end = clock();
    os << "\n\nPrunes at level:\n";
    for (unsigned i = 0; i < m_prunes.size(); ++i) {
        os << i << ": " << m_prunes[i] << "\n";
    }
    os << "\nRuntime: " << (end - start) / (double)(CLOCKS_PER_SEC) * 1000 << " ms\n";
    #endif
}
