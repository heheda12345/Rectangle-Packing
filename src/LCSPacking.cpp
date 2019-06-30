#include <cstdlib>
#include <iostream>
#include <typeinfo>
#include <cassert>
#include <algorithm>

#include "LCSPackingCommand.h"
#include "Layout.h"
#include "MyPriorityQueue.h"

namespace RECTPACKING {
using namespace std;

static void OneDirectionPacking(Layout &layout, std::vector<int> &s1,std::vector<int> &s2, bool hor)
{
  vector<Rect> &rects = layout.getRects();
  const int rectNum = rects.size();
  MyPriorityQueue pq(rectNum);
  std::vector<int> match; match.resize(rectNum);
  std::vector<int> pos2; pos2.resize(rectNum);
  for (int i=0;i<rectNum;i++)
    pos2[s2[i]]=i;
  for (int i=0; i<rectNum; i++)
    match[i]=pos2[s1[i]];
  std::vector<int> &value=pos2;
  for (int i=0;i<rectNum;i++)
    value[i]=hor?rects[i].width:rects[i].height;
  for (int i=0; i<rectNum; i++)
  {
      int b=s1[i],p=match[i];
      int len=pq.insert(p,value[b]);
      if (hor)
        rects[b].lb.x = len-rects[b].width;
      else
        rects[b].lb.y = len-rects[b].height;
  }

}

void LCSPacking(Layout &layout,std::vector<int> &s1,std::vector<int> &s2)
{
  //cout << "In LCSPacking " << endl;
  bool hor = true;
  //cout << "========== horizontal ==========" << endl;
  OneDirectionPacking(layout, s1, s2, hor);
  hor = false;
  reverse(s1.begin(),s1.end());
  //cout << "========== vertical ==========" << endl;
  OneDirectionPacking(layout, s1, s2, hor);
  reverse(s1.begin(),s1.end());
}

}
