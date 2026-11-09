/**
 * @file bst.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V BST<K, V>::find(const K& key)
{
    // your code here
    Node*& res = find(root, key);
    if (res == NULL) {
        return V(); 
    } 
    return res->value;
}

template <class K, class V>
struct BST<K, V>::Node* & BST<K, V>::find(Node* & subtree, const K& key)
{
    // Your code here
    if (subtree == NULL) {
    return subtree;
    }

    if (key == subtree->key) {
        return subtree;
    }

    if (key < subtree->key) {
        return find(subtree->left, key);
    }

    return find(subtree->right, key);
}

template <class K, class V>
void BST<K, V>::insert(const K & key, const V & value)
{
    // your code here
    insert(root, key, value);
}

template <class K, class V>
void BST<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    // your code here
    Node*& spot = find(subtree, key);
    if (spot != NULL) {
        return;
    }
    spot = new Node(key, value);
}

template <class K, class V>
void BST<K, V>::swap(Node*& first, Node*& second)
{
    // your code here
    std::swap(first->key, second->key);
    std::swap(first->value, second->value);
}

template <class K, class V>
void BST<K, V>::remove(const K& key)
{
    // your code here
    remove(root, key);
}

template <class K, class V>
void BST<K, V>::remove(Node*& subtree, const K& key)
{
    // your code here
    Node*& target = find(subtree, key);
    if (target == NULL) {
        return;
    } 

    // 0 child
    if (target->left == NULL && target->right == NULL) {
        delete target;
        target = NULL;
        return;
    }

    // 1 child
    if (target->left == NULL || target->right == NULL) {
        Node* child;
        if (target->left != NULL) {
            child = target->left;
        } else {
            child = target->right;
        }

        Node* old = target;
        target = child;
        delete old;
        return;
    }

    // 2 children: use IOP
    Node** iopPtr = &(target->left);
    while ((*iopPtr)->right != NULL) {
        iopPtr = &((*iopPtr)->right);
    }
    Node*& iop = *iopPtr;

    swap(target, iop);

    remove(iop, key);
}

template <class K, class V>
BST<K, V>::BST(const std::vector<std::pair<K, V>> &inList)
    : root(NULL), _out(&std::cout) {
    // your code here
    for (size_t i = 0; i < inList.size(); i++) {
        insert(inList[i].first, inList[i].second);
    }
}

template <class K, class V>
std::vector<int> allBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    int n = (int)inList.size();
    std::vector<int> hist(n, 0);

    auto cmp = [](const std::pair<K,V>& a, const std::pair<K,V>& b) {
        if (a.first < b.first) {
            return true;
        }
        if (b.first < a.first) {
            return false;
        }
        return a.second < b.second;
    };

    std::sort(inList.begin(), inList.end(), cmp);


    BST<K,V> t;
    for (size_t i = 0; i < inList.size(); i++) {
        t.insert(inList[i].first, inList[i].second);
    }

    int h = t.height();
    if (h >= 0 && h < n) hist[h]++;

    while (std::next_permutation(inList.begin(), inList.end(), cmp)) {

        BST<K,V> t2;

        for (size_t i = 0; i < inList.size(); i++) {
            t2.insert(inList[i].first, inList[i].second);
        }

        int h2 = t2.height();
        if (h2 >= 0 && h2 < n) hist[h2]++;
    }

        return hist;
}