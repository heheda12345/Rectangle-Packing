#include "SimulatedAnnealing.h"

#include<fstream>

namespace RECTPACKING {
using namespace std;

SimulatedAnnealing::SimulatedAnnealing(double T_initial_, double T_final_, double dropSpeed, double multiRate_, unsigned int seed_):
        T_initial(T_initial_), T_final(T_final_), speed(dropSpeed), multiRate(multiRate_), seed(seed_), packingStrategy(nullptr)
{
}

SimulatedAnnealing::~SimulatedAnnealing()
{
    if (packingStrategy)
        delete packingStrategy;
}

void SimulatedAnnealing::setParameters(double initialTemperature, double dropSpeed, unsigned int seed_)
{
    T_initial=initialTemperature;
    speed=dropSpeed;
    seed=seed_;
}

void SimulatedAnnealing::setStrategy(PackingStrategy* newStrategy)
{
    if (packingStrategy)
    {
        delete packingStrategy;
    }
    packingStrategy=newStrategy;
}

double rand01()
{
    return (((rand()&32767)<<15)|(rand()&32767))*1.0/((32767<<15)|32767);
}

void SimulatedAnnealing::outputLayout(Layout& layout)
{
    ofstream output("SA_log.txt");
    const vector<Rect>& rects=layout.getRects();
    output<<rects.size()<<endl;
    for (int i=0; i<(int)rects.size(); i++)
        output<<rects[i].left()<<" "<<rects[i].bottom()<<" "<<rects[i].right()<<" "<<rects[i].top()<<" "<<rects[i].networkID<<endl;
    output.close();
}

void SimulatedAnnealing::run(Layout& layout)
{
    if (!packingStrategy)
    {
        cout<<"Error! Please set your packing strategy first!"<<endl;
        return;
    }
    layout.setPackingStrategy(packingStrategy);
    packingStrategy->initialPacking(layout);
    srand(this->seed);
    double T_cur=this->T_initial;
    //packingStrategy->compPackingLayout(object);
    double ans=layout.compCost();
    float area=layout.compArea();
    float wire=layout.compWire();
    int cnt=0;
    //ofstream os("log.txt");
    //vector<Rect> ansRect=layout.getRects();
    //for (int i=1;i<=10;i++) cout<<rand01()<<endl;
    while(T_cur>this->T_final)
    {
        //cout<<T_cur<<" : "<<ans<<endl;
        packingStrategy->nextPackingCommand();
        packingStrategy->compPackingLayout(layout);
        double cost=layout.compCost();
        if(cost<=ans||exp((ans-cost)*multiRate/T_cur)>rand01())
        {
            //if (cost>ans) cout<<"back"<<endl;
            //else if (cost==ans) cout<<"equal"<<endl;
            //else cout<<"better"<<endl;
            //if (cost<ans) outputLayout(layout);
            ans=cost;
            area=layout.compArea();
            wire=layout.compWire();
        }
        else
            packingStrategy->prePackingCommand();
        T_cur*=speed;
        cnt++;
        /*if(cnt%10000==0)
        {
            os<<"round: "<<cnt<<" ans: "<<ans<<" temp: "<<T_cur<<" size: "<<area<<" wire "<<wire<<endl;
            cout<<"round: "<<cnt<<" ans: "<<ans<<" temp: "<<T_cur<<" size: "<<area<<" wire "<<wire<<endl;
        }*/
	}
	packingStrategy->compPackingLayout(layout);
	//os<<"SimulatedAnnealing  Finish! Total rounds: " << cnt <<endl;
	//os<<"ans: "<<ans<<" size: "<<area<<" wire "<<wire<<endl;
	//os.close();

}
}
