/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>

using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim)
{
    /**
     * @todo Implement this function!
     */
    if (first[curDim] < second[curDim]) {
        return true;
    }
    if (first[curDim] > second[curDim]) {
        return false;
    }
    return first < second;
}

template <int Dim>
bool shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential)
{
    /**
     * @todo Implement this function!
     */

    double distBest = 0;
    double distPot  = 0;

    for (int i = 0; i < Dim; i++) {
        double db = target[i] - currentBest[i];
        double dp = target[i] - potential[i];
        distBest += db * db;
        distPot  += dp * dp;
    }

    if (distPot < distBest) {
        return true;
    }
    if (distPot > distBest) {
        return false;
    }
    return potential < currentBest;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
    root = NULL;
    size = newPoints.size();
    if (size == 0) {
        return;
    }

    vector<Point<Dim>> pts = newPoints;  
    root = buildTree(pts, 0, (int)pts.size() - 1, 0);
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode*
KDTree<Dim>::buildTree(vector<Point<Dim>>& pts, int l, int r, int d)
{
    if (l > r) {
        return NULL;
    }

    int m = (l + r) / 2;

    DimComparator cmp(d);

    select(pts.begin() + l, pts.begin() + r + 1, pts.begin() + m, cmp);

    KDTreeNode* node = new KDTreeNode(pts[m]);

    int dim = (d + 1) % Dim;

    node->left  = buildTree(pts, l, m - 1, dim);
    node->right = buildTree(pts, m + 1, r, dim);

    return node;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
    size = other.size;
    root = copyTree(other.root);
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode*
KDTree<Dim>::copyTree(const KDTreeNode* otherRoot)
{
    if (otherRoot == NULL) {
        return NULL;
    }

    KDTreeNode* node = new KDTreeNode(otherRoot->point);
    node->left  = copyTree(otherRoot->left);
    node->right = copyTree(otherRoot->right);
    return node;
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
    if (this == &rhs) {
        return *this;
    }

    size = rhs.size;
    return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
    /**
     * @todo Implement this function!
     */
    root = NULL;
    size = 0;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */

    Point<Dim> best = root->point;
    findNN(root, query, 0, best);
    return best;
}

template <int Dim>
void KDTree<Dim>::findNN(KDTreeNode* node, const Point<Dim>& query, int dim, Point<Dim>& best) const
{
    if (node == NULL) {
        return;
    }

    bool wentLeft;
    int d = (dim + 1) % Dim;
    if (smallerDimVal(query, node->point, dim)) {
        if (node->left != NULL) {
            findNN(node->left, query, d, best);
        } else {
            findNN(node->right, query, d, best);
        }
        wentLeft = true;
    } else {
        if (node->right != NULL) {
            findNN(node->right, query, d, best);
        } else {
            findNN(node->left, query, d, best);
        }
        wentLeft = false;
    }

    if (shouldReplace(query, best, node->point)) {
        best = node->point;
    }

    double split = (query[dim] - node->point[dim]) * (query[dim] - node->point[dim]);
    
    double bestDist = 0.0;
    for (int i = 0; i < Dim; i++) {
        bestDist += (query[i] - best[i]) * (query[i] - best[i]);
    }

    if (split <= bestDist) {
        if (wentLeft) {
            findNN(node->right, query, d, best);
        } else {
            findNN(node->left, query, d, best);
        }
    }
}


template <typename RandIter, typename Comparator>
RandIter partitionRange(RandIter begin, RandIter end, RandIter pivot, Comparator cmp)
{
    auto pivotVal = *pivot;

    std::iter_swap(pivot, end - 1);

    RandIter store = begin;
    for (RandIter i = begin; i != end - 1; i++) {
        if (cmp(*i, pivotVal)) {
            std::iter_swap(store, i);
            store++;
        }
    }

    std::iter_swap(store, end - 1);
    return store;
}

template <typename RandIter, typename Comparator>
void select(RandIter start, RandIter end, RandIter k, Comparator cmp)
{
    /**
     * @todo Implement this function!
     */    
    while (true) {
        if (end - start <= 1) {
            return;
        }

        RandIter pivot = start + (end - start) / 2;
        RandIter pivotPos = partitionRange(start, end, pivot, cmp);

        if (k == pivotPos) {
            return;
        }
        if (k < pivotPos) {
            end = pivotPos;
        } else {
            start = pivotPos + 1;
        }
    }
}

