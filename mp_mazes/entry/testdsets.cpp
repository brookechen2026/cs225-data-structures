/**
 * @file testdsets.cpp
 * Performs basic tests of DisjointSets.
 * @date April 2007
 * @author Jonathan Ray
 */

#include <iostream>
#include "dsets.h"

using std::cout;
using std::endl;

int main()
{
    DisjointSets s(50);

    s.setUnion(2, 3);
    s.setUnion(3, 5);
    s.setUnion(5, 7);
    s.setUnion(7, 2);
    s.setUnion(4, 6);

    cout << s.find(7) << endl;
    cout << s.find(5) << endl;
    cout << s.find(6) << endl;

    s.addElements(1);
    
    cout << s.find(50) << endl;

    s.setUnion(50, 3);

    cout << s.find(50) << endl;
    cout << "Disjoint Sets test complete" << endl;

    return 0;
}
