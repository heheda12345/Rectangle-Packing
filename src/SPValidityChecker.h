#ifndef __SP_VALIDITY_CHECKER__
#define __SP_VALIDITY_CHECKER__
#include<vector>
#include"Rect.h"
namespace RECTPACKING {
namespace checker {
    bool noOverlaps(const std::vector<Rect>&);
}
}
#endif // __SP_VALIDITY_CHECKER__
