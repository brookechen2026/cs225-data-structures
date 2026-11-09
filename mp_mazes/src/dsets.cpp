/* Your code here! */

#include "dsets.h"

DisjointSets::DisjointSets(int num) {
    addElements(num);
}

void DisjointSets::addElements(int num) {
    for (int i = 0; i < num; i++) {
        elems_.push_back(-1);
    }
}

int DisjointSets::find(int elem) {
    if (elems_[elem] < 0) {
        return elem;
    }

    elems_[elem] = find(elems_[elem]);
    return elems_[elem];
}

void DisjointSets::setUnion(int a, int b) {
    int ar = find(a);
    int br = find(b);

    if (ar == br) {
        return;
    }

    int as = -elems_[ar];
    int bs = -elems_[br];

    if (as >= bs) {
        elems_[ar] -= bs;
        elems_[br] = ar;
    } else {
        elems_[br] -= as;   
        elems_[ar] = br; 
    }
}

int DisjointSets::size(int elem) {
    int root = find(elem);
    return -elems_[root];
}

int DisjointSets::getValue(int elem) const {
    return elems_[elem];
}