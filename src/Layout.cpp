#include "Layout.h"

#include <iostream>
#include <climits>
#include <map>
#include <cmath>
#include <algorithm>

namespace RECTPACKING {
Layout::Layout(const Layout& layout, PackingStrategy *s):
    m_rects(layout.m_rects), m_pStrategy(s), m_alpha(layout.m_alpha), m_network(layout.m_network)
{
}

float Layout::compArea()
{
    int lX = INT_MAX, rX = INT_MIN, bY = INT_MAX, tY = INT_MIN;
    for (int i = 0; i < (int)m_rects.size(); ++ i)
    {
        Rect r = m_rects[i];
        if (lX > r.left())
            lX = r.left();
        if (rX < r.right())
            rX = r.right();
        if (bY > r.bottom())
            bY = r.bottom();
        if (tY < r.top())
            tY = r.top();
    }
    return ((rX-lX) * (tY-bY));
}

float Layout::compWire()
{
    vector<int> match;
    match.assign(m_rects.size()>>1,-1);
    float wireLength=0;
    auto midX=[](const Rect& a) { return a.left()+a.width/2.0;};
    auto midY=[](const Rect& a) { return a.bottom()+a.height/2.0;};
    for (int i=0; i<(int)m_rects.size();i++)
    {
      Rect &r1=m_rects[i];
      if (match[r1.networkID]==-1)
      {
        match[r1.networkID]=i;
      }
      else
      {
        Rect &r2=m_rects[match[r1.networkID]];
        wireLength+=fabs(midX(r1)-midX(r2))+fabs(midY(r1)-midY(r2));
      }
    }
    return wireLength;
}

float Layout::compCost()
{

    return compArea()*m_alpha+compWire()*(1-m_alpha);
}

void Layout::addRectPair(const Rect &a, const Rect &b)
{
    m_rects.push_back(a);
    m_rects.push_back(b);
    m_network++;
    auto it=m_rects.end();
    (*--it).networkID=m_network-1;
    (*--it).networkID=m_network-1;
}

struct MyEvent
{
    int id,time,type; // type: 0 insert 1 delete
    MyEvent(int id_,int time_,int type_) : id(id_), time(time_), type(type_) {}
};

bool operator < (const MyEvent &a, const MyEvent &b)
{
    return a.time==b.time? a.type>b.type : a.time<b.time;
}

void Layout::oneDirectionCompConstraints(std::vector<std::pair<int,int>> &cons, bool hor)
{
    vector<MyEvent> events;
    vector<int> left,right; //bottom,top when hor==0
    if (hor)
    {
        for (int i=0; i<(int)m_rects.size(); i++)
        {
            events.push_back(MyEvent(i,m_rects[i].bottom(),0));
            events.push_back(MyEvent(i,m_rects[i].top(),1));
            left.push_back(m_rects[i].left());
            right.push_back(m_rects[i].right());
        }
    }
    else
    {
        vector<int> &bottom=left, &top=right;
        for (int i=0; i<(int)m_rects.size(); i++)
        {
            events.push_back(MyEvent(i,m_rects[i].left(),0));
            events.push_back(MyEvent(i,m_rects[i].right(),1));
            bottom.push_back(m_rects[i].bottom());
            top.push_back(m_rects[i].top());
        }
    }
    sort(events.begin(),events.end());
    map<int,int> rectLeft,rectRight; // left boundary & right boundary
    cons.clear();
    for (const MyEvent &event: events)
        if (event.type==0) // insert
        {
            auto it=rectRight.lower_bound(left[event.id]);
            if (it!=rectRight.begin())
                cons.push_back(make_pair((*--it).second,event.id));
            it=rectLeft.upper_bound(right[event.id]);
            if (it!=rectLeft.end())
                cons.push_back(make_pair(event.id,(*--it).second));
        }
        else // delete
        {
            rectLeft.erase(rectLeft.find(left[event.id]));
            rectRight.erase(rectRight.find(right[event.id]));
        }
}
void Layout::compConstraints(vector<pair<int, int> > &horCons, vector<pair<int, int> > &verCons)
{
  //enumerate each pair of rectangles
  //for each pair of rectangles, there can only be one relationship, either horizontal or vertical
  oneDirectionCompConstraints(horCons,1);
  oneDirectionCompConstraints(verCons,0);
  sort(horCons.begin(),horCons.end());
  // maybe unnecessary
  vector<pair<int,int>> saver;
  for (auto con: verCons)
  {
      if (!binary_search(horCons.begin(),horCons.end(),con))
        saver.push_back(con);
  }
  verCons=saver;
}

ostream & operator << (ostream &out, Layout &l)
{
  out << "============= Layout information =============" << endl;
  out << "------------- Rectangles -------------" << endl;
  for (int i = 0; i < (int)l.m_rects.size(); ++ i)
  {
    out << l.m_rects[i] << " in network "<<l.m_rects[i].networkID<<endl;
  }
  out << "------------- Packing code -------------" << endl;
  out << *(l.m_pStrategy->getPackingCommand());

  return out;
}

}
