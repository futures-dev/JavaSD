//
// Andrei Kolomiets 143-1
// CLion 2016.1.2 MinGW 3.21 GCC 4.9.1
// 21.05.2016 
//

#ifndef HOMEWORK_PATRICIATREE_H
#define HOMEWORK_PATRICIATREE_H

#include "StringContainer.h"
#include <vector>

class PatriciaTree : public StringContainer {
    struct PatriciaTreeNode {
        std::string value;
        std::vector<PatriciaTreeNode *> links;
        bool full;

        PatriciaTreeNode(const std::string &&value) : value(value), full(true) { };

        ~PatriciaTreeNode();
    };

    PatriciaTreeNode *root;

    inline int check(const std::string &word1, int start1, const std::string &word2, int start2) const;

public:
    PatriciaTree() : root(new PatriciaTreeNode("\0")) { }

    void insert(const std::string &word);

    bool search(const std::string &word) const;

    virtual ~PatriciaTree();

};

#endif //HOMEWORK_PATRICIATREE_H
