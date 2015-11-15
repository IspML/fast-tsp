#ifndef KANTO_H
#define KANTO_H

#include "Pokemon.h"
#include "TspFinder.h"

#include <iostream>
#include <string>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <cfloat>
#include <ctime>

#include <vector>

using namespace std;


struct MstNode
{
    unsigned pokemon;
    MstNode* parent;
    vector<MstNode*> children;

    MstNode() : pokemon(0), parent(nullptr), children(0) { }
};

class Kanto
{
private:
    unsigned m_count;
    vector<Pokemon> m_pokemon;

public:
    Kanto(istream& is);

    static inline double edgeDistance(const Pokemon& p1, const Pokemon& p2) {
        return sqrt(pow(p1.xPos - p2.xPos, 2) + pow(p1.yPos - p2.yPos, 2));
    }

    static inline unsigned long long edgeDistanceUnnormalized(const Pokemon& p1, const Pokemon& p2) {
        long long xDif = (long long)(p1.xPos) - p2.xPos;
        long long yDif = (long long)(p1.yPos) - p2.yPos;
        return xDif * xDif + yDif * yDif;
        //return pow((long long)(p1.xPos) - p2.xPos, 2) + pow((long long)(p1.yPos) - p2.yPos, 2);
    }

    static inline bool canWalkOrSurf(const Pokemon& p1, const Pokemon& p2) {
        switch (p1.region()) {
        case LAND:    return p2.region() != SEA;
        case SEA:    return p2.region() != LAND;
        default:    return true;
        }
    }

    inline void partA(ostream& os) {
        vector<unsigned> pv;
        double total = mst(pv);

        os << total << "\n";
        for (unsigned i = 1; i < m_count; ++i) {
            if (i < pv[i]) {
                os << i << " " << pv[i] << "\n";
            }
            else {
                os << pv[i] << " " << i << "\n";
            }
        }
    }

    inline void partB(ostream& os) {
        TspFinder tsp(m_pokemon);
        tsp.find(os);
    }

    inline void partC(ostream& os) {
        vector<unsigned> nodes;
        clock_t endTime = clock() + (CLOCKS_PER_SEC * 14.5);

        mstCircuit(nodes);
        double total = twoOpt(nodes, endTime);
        
        os << total << "\n";
        for (unsigned i = 0; i < nodes.size(); ++i) {
            os << nodes[i] << " ";
        }
        os << "\n";
    }

    double mst(vector<unsigned>& pv);

    void mstCircuit(vector<unsigned>& nodes);

    void inOrder(MstNode* root, vector<unsigned>& nodes);

    double twoOpt(vector<unsigned>& nodes, const clock_t& endTime);
}; // class Kanto


#endif // KANTO_H