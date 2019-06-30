#ifndef __LCS_H__
#define __LCS_H__
#include "DoublyLinkedList.h"

namespace RECTPACKING {

struct pqNode
{
    int indexInDll;
    bool exist;
};

class MyPriorityQueue
{
public:
    MyPriorityQueue(int n_);
    int insert(int id,int value);
protected:
    void build(int l,int r,int index);
    void treeInsert(int x);
    void treeErase(int x);
    int nearestLeaf(int x);
    int n;
    std::vector<pqNode> node;
    DoublyLinkedList dll;
    std::vector<int> leafIndex;
};

}

#endif // __LCS_H__
