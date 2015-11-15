Traveling Salesperson (TSP) and Minimum Spanning Tree (MST) solution to UM class assignment.

This application finds optimum TSP solutions, optimum MST solutions, and "fast-TSP" (30 sec time limit) solutions for input graphs provided as (x, y) node coordinate pairs. For both TSP solutions, the input graphs are fully connected. For MST, there is assumed to be a "coastline" at the Q3 border--thus, nodes in Q3 can only be reached by first visiting a node at (0, y) or (x, 0) where y < 0 and x < 0.

Examples of valid command-line arguments are:
./poke -m MST < sampleA.in
./poke -m OPTTSP < opt25.in
./poke -m FASTTSP < fastopt16000.in

The "fast-TSP" solution uses MST to approximate a solution, which is then repeatedly optimized using Two-Opt until it is determined that any additional optimizations will cause the execution time to exceed 30 sec.

Note that this project was developed in < 1 week during a busy semester. Some corners were cut (ok, a lot) with regards to documentation and maintainability, as neither was necessary for full credit.
