#ifndef __DOUBLY_LINKED_LIST__
#define __DOUBLY_LINKED_LIST__
#include<vector>

namespace RECTPACKING {

struct DllNode
{
    int pre,next,value;
};

class DoublyLinkedList
{
public:
    DoublyLinkedList(int n) { nodes.resize(n); nodes[0].pre=nodes[0].next=nodes[0].value=0; }
    int insertBefore(int newNode,int p,int dValue) { return insertBetween(newNode,preNode(p),p,dValue); }
    int insertAfter(int newNode,int p,int dValue) { return insertBetween(newNode,p,nextNode(p),dValue); }
    int nextNode(int id) const { return nodes[id].next; }
    int preNode(int id) const { return nodes[id].pre; }
    int value(int id) const { return nodes[id].value; }
    void erase(int id)
    {
        int pre=nodes[id].pre, next=nodes[id].next;
        nodes[pre].next=next; nodes[next].pre=pre;
    }
protected:
    int insertBetween(int newNode,int p,int q,int dValue)
    {
        nodes[newNode].pre=p; nodes[p].next=newNode;
        nodes[newNode].next=q; nodes[q].pre=newNode;
        return nodes[newNode].value=nodes[p].value+dValue;
    }
    std::vector<DllNode> nodes;
};

}

#endif // __DOUBLY_LINKED_LIST__
