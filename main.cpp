#include "Kanto.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <getopt.h>
#include <cstring>

using namespace std;


const string HELP_MESSAGE = "usage: --mode [MST|OPTTSP|FASTTSP]";

enum Mode
{
    NONE,
    MST,
    OPTTSP,
    FASTTSP
};


int main(int argc, char* argv[])
{
    ios_base::sync_with_stdio(false);

    Mode mode = NONE;

    struct option longOpts[] =
    {
        { "mode", required_argument, nullptr, 'm' },
        { "help", no_argument, nullptr, 'h' },
        { 0, 0, 0, 0 }
    };

    int opt = 0, index = 0;
    opterr = false;
    while ((opt = getopt_long(argc, argv, "m:h", longOpts, &index)) != -1) {
        switch (opt) {
        case 'h':
            cout << HELP_MESSAGE << endl;
            return 0;

        case 'm':
            if (strcmp(optarg, "MST") == 0) {
                mode = MST;
            }
            else if (strcmp(optarg, "OPTTSP") == 0) {
                mode = OPTTSP;
            }
            else if (strcmp(optarg, "FASTTSP") == 0) {
                mode = FASTTSP;
            }
            else {
                cerr << "Bad input\n" + HELP_MESSAGE << endl;
                return 1;
            }
        }
    }

    ostringstream oss;
    oss << setprecision(2) << fixed;

    Kanto k(cin);

    try {
        switch (mode) {
        case MST:
            k.partA(oss);
            break;

        case OPTTSP:
            k.partB(oss);
            break;

        case FASTTSP:
            k.partC(oss);
            break;

        default:
            cerr << "No mode selected\n" + HELP_MESSAGE << endl;
            return 1;
        }
    }
    catch (...) {
        cerr << "Cannot construct MST" << endl;
        return 1;
    }

    cout << oss.str() << flush;

    return 0;
}