#ifndef __SP_GRAPHPACKING_H__
#define __SP_GRAPHPACKING_H__
#include <vector>
#include <utility>

namespace RECTPACKING {

class LCSPackingCommand;
class Layout;

void LCSPacking(Layout &layout,std::vector<int> &s1,std::vector<int> &s2);

}

#endif	//__SP_GRAPHPACKING_H__
