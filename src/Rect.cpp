#include <iostream>
#include "Rect.h"

namespace RECTPACKING {
using namespace std;

ostream & operator << (ostream &out,const Rect &r)
{
  out << "[" << r.lb.x << "," << r.lb.y << "]-[" << r.lb.x+r.width << "," << r.lb.y+r.height << "]";
  return out;
}

bool intersect(int l1,int r1,int l2,int r2)
{
    return (l1<=l2&&l2<r1)||(l1<r2&&r2<=r1);
}

bool Rect::overlap(const Rect& r) const
{
    return intersect(this->left(),this->right(),r.left(),r.right())&&intersect(this->bottom(),this->top(),r.bottom(),r.top());
}

}
