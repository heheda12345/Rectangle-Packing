#ifndef __SP_SPPACKINGSTRATEGY_H__
#define __SP_SPPACKINGSTRATEGY_H__
#include <vector>
#include <utility>
#include "PackingStrategy.h"
#include "Rect.h"

namespace RECTPACKING {
using namespace std;

class Rect;
class SPPackingStrategy : public PackingStrategy
{
public:
  SPPackingStrategy():PackingStrategy() {}
  ~SPPackingStrategy()=default;
  void initialPacking(Layout &layout);
  void compPackingLayout(Layout &layout);
  void nextPackingCommand();
  void prePackingCommand();
};

}

#endif	//__SP_PACKINGSTRATEGY_H__
