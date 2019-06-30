#include<vector>

#include"Rect.h"
#include"SPValidityChecker.h"

namespace RECTPACKING {
using namespace std;


namespace checker {
bool noOverlaps(const std::vector<Rect>& rects)
{
    for (int i=0; i<(int)rects.size(); i++)
        for (int j=i+1; j<(int)rects.size();j++)
            if (rects[i].overlap(rects[j]))
                return 0;
    return 1;
}
}
}
