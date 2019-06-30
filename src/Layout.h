#ifndef __SP_LAYOUT_H__
#define __SP_LAYOUT_H__
#include <ostream>
#include <vector>
#include <utility>
#include "Rect.h"
#include "PackingStrategy.h"

namespace RECTPACKING {
class PackingStrategy;
class Layout
{
public:
  Layout(float alpha=1):m_pStrategy(0),m_alpha(alpha),m_network(0) {}
  Layout(const Layout&, PackingStrategy *);
  Layout(const Layout&) =delete;
  ~Layout()=default;

  void addRectPair(const Rect &a, const Rect &b);

  std::vector<Rect> &getRects() { return m_rects; }
  int getRectNum() { return m_rects.size(); }
  Rect &getRect(int i) { return m_rects[i]; }

  PackingStrategy *getPackingStrategy () const { return m_pStrategy; }
  void setPackingStrategy(PackingStrategy *s) {
    if (m_pStrategy)
      delete m_pStrategy;
    m_pStrategy = s;
  }

  float compArea();
  float compWire();
  float compCost();
  void setAlpha(float alpha) { m_alpha=alpha;}

  void compConstraints(std::vector<std::pair<int, int> > &horCons, std::vector<std::pair<int, int> > &verCons);
  friend std::ostream & operator << (std::ostream &out, Layout &l);

protected:
  void oneDirectionCompConstraints(std::vector<std::pair<int,int>> &cons, bool hor);

private:
  Layout& operator = (const Layout&);
  void addRect(Rect &r) { m_rects.push_back(r); }
  std::vector<Rect> m_rects;
  PackingStrategy *m_pStrategy;
  float m_alpha;
  int m_network;
};

}

#endif	//__SP_LAYOUT_H__
