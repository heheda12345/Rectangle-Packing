#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <utility>
#include <cmath>

#include "LCSPackingCommand.h"
#include "GraphPacking.h"
#include "Layout.h"
#include "LCSPacking.h"

namespace RECTPACKING {
using namespace std;
//strange solution
void LCSPacking(Layout &layout,std::vector<int> &s1,std::vector<int> &s2);
void LCSPackingCommand::interpretToLayout(Layout &layout)
{
 // cout << "In LCSPackingCommand::interpretToLayout" << endl;
  //compute the horizontal constraint pairs and vertical constraint pairs
  LCSPacking(layout,m_s1,m_s2);
}

void LCSPackingCommand::change(vector<int> &m_s)
{
  int size = m_s.size();
  int i1 = rand() % size;
  int i2 = rand() % size;
  while (i1 == i2)
  {
    i2 = rand() % size;
  }
  int tmp = m_s[i1];
  m_s[i1] = m_s[i2];
  m_s[i2] = tmp;
}

void LCSPackingCommand::next()
{
  pre_s1=m_s1;
  pre_s2=m_s2;
  if (rand() % 2)
    change(m_s1);
  else
    change(m_s2);
}

void LCSPackingCommand::pre()
{
    m_s1=pre_s1;
    m_s2=pre_s2;
}

void LCSPackingCommand::dump(ostream &out)
{
  out << "Sequence Pair:";
  for (int i = 0; i < (int)m_s1.size(); ++ i)
  {
    out << " " << m_s1[i];
  }
  out << ",";
  for (int i = 0; i < (int)m_s2.size(); ++ i)
  {
    out << " " << m_s2[i];
  }
  out << endl;
}

}
