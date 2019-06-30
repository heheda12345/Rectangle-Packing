#ifndef __LCS_PACKINGSTRATEGY_H__
#define __LCS_PACKINGSTRATEGY_H__
#include <vector>
#include <utility>
#include "PackingStrategy.h"
#include "Rect.h"

namespace RECTPACKING {
using namespace std;

class Rect;
class LCSPackingStrategy : public PackingStrategy
{
public:
  LCSPackingStrategy():PackingStrategy() {}
  ~LCSPackingStrategy()=default;
  void initialPacking(Layout &layout);
  void compPackingLayout(Layout &layout);
  void nextPackingCommand();
  void prePackingCommand();
};

}

#endif	//__LCS_PACKINGSTRATEGY_H__
