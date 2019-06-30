#ifndef __SP_RECT_H__
#define __SP_RECT_H__
#include <ostream>
#include "Point.h"

namespace RECTPACKING {

struct Rect
{
    Rect(int width_,int height_,Point lb_=(Point){0,0},int networkID_=-1):
        width(width_), height(height_), lb(lb_), networkID(networkID_) {}
    int left() const { return lb.x; }
    int right() const { return lb.x+width; }
    int bottom() const { return lb.y; }
    int top() const { return lb.y+height; }
    bool overlap(const Rect& r) const;
    int width;
    int height;
    Point lb;
    int networkID;

};

std::ostream & operator << (std::ostream &out,const Rect &r);

}

#endif	//__SP_RECT_H__
