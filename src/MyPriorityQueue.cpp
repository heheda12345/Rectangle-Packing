#include "MyPriorityQueue.h"

namespace RECTPACKING {

void MyPriorityQueue::build(int l,int r,int index)
{
    node[index].exist=0;
    if (l==r)
    {
        node[index].indexInDll=l;
        leafIndex[l]=index;
        return;
    }
    int mid=(l+r)>>1;
    build(l,mid,index<<1);
    build(mid+1,r,index<<1|1);
    node[index].indexInDll=-1;
}

MyPriorityQueue::MyPriorityQueue(int n_) : n(n_+1), dll(n+1)
{
    int m=1;
    while (m<n) m<<=1;
    node.resize(m<<1);
    leafIndex.resize(n);
    build(0,n-1,1);
    treeInsert(0);
}

int MyPriorityQueue::nearestLeaf(int x)
{
    x=leafIndex[x];
    while (x&&!node[x].exist) x>>=1;
    if (!x) return -1; //empty tree
    if (node[x<<1].exist)
    {
        while (node[x].indexInDll==-1)
            if (node[x<<1|1].exist) x=x<<1|1; else x=x<<1;
    }
    else
    {
        while (node[x].indexInDll==-1)
            if (node[x<<1].exist) x=x<<1; else x=x<<1|1;
    }
    return node[x].indexInDll;
}

void MyPriorityQueue::treeInsert(int x)
{
    x=leafIndex[x];
    while (x&&!node[x].exist) node[x].exist=1,x>>=1;
}

void MyPriorityQueue::treeErase(int x)
{
    x=leafIndex[x];
    node[x].exist=0;
    while (x&&!node[x^1].exist) x>>=1,node[x].exist=0;
}

int MyPriorityQueue::insert(int id,int dValue)
{
    id++;
    int p=nearestLeaf(id),value;
    /*if (p==-1)
    {
        value=dll.insertToEmpty(id,dValue);
    }
    else
    {*/
    if (p<id)
        value=dll.insertAfter(id,p,dValue);
    else
        value=dll.insertBefore(id,p,dValue);
    treeInsert(id);
    while (1)
    {
        int p=dll.nextNode(id);
        if (p<=id||dll.value(p)>value) break;
        dll.erase(p);
        treeErase(p);
    }
    return value;
}
}
