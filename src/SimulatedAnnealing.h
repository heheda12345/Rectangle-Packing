#ifndef __SIMULATED_ANNEALING_H__
#define __SIMULATED_ANNEALING_H__
#include <ctime>
#include <cmath>
#include "PackingStrategy.h"
#include "Layout.h"

namespace RECTPACKING {

class SimulatedAnnealing
{
public:
    SimulatedAnnealing(double T_initial_, double T_final_=0.02, double dropSpeed=0.8, double multiRate=1, unsigned int seed_=time(0));
    ~SimulatedAnnealing();

    void setParameters(double initialTemperature, double dropSpeed, unsigned int seed_=time(0));
	void run(Layout& layout);
	void setStrategy(PackingStrategy*);

protected:
    void outputLayout(Layout& layout);
	double T_initial,T_final,speed,multiRate;
	unsigned int seed;
	PackingStrategy* packingStrategy;

private:
    SimulatedAnnealing(const SimulatedAnnealing&); //avoid copy constructor
    SimulatedAnnealing& operator = (const SimulatedAnnealing&); //avoid copy assignment
};

}
#endif //__SIMULATED_ANNEALING_H__
