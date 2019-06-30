#include <fstream>
#include <iostream>
#include <sys/time.h>
#include <sys/timeb.h>
#include <windows.h>
#include <vector>

#include "Rect.h"
#include "SimulatedAnnealing.h"
#include "SPPackingStrategy.h"
#include "LCSPackingStrategy.h"
#include "Layout.h"
#include "SPValidityChecker.h"

using namespace std;
using namespace RECTPACKING;

const int MAX=100;

void generateLayout(int pairs)
{
    srand(time(0));
    ofstream file("input.txt");
    file << pairs << endl;
    for (int i=0; i<pairs; i++)
        file << rand()%MAX+1 <<" "<<rand()%MAX+1<<" "<<rand()%MAX+1<<" "<<rand()%MAX+1<<endl;
    //print 4 integer in each line, represents (width1,height1),(width2,height2) of one pair of rectangles
    //printf("generateLayout end\n");
    file.close();

}

void loadLayout(const string &fileName,Layout& layout)
{
    ifstream input(fileName);
    int n,w1,h1,w2,h2;
    input >> n;
    for (int i=0; i<n; i++)
    {
        input >> w1 >> h1 >> w2 >> h2;
        layout.addRectPair(Rect(w1,h1),Rect(w2,h2));
    }
    input.close();
}

void executeSA(SimulatedAnnealing &sp, Layout &layout, const string& functionName, const string& fileName)
{
    LARGE_INTEGER winFreq;
    LARGE_INTEGER winStart, winNow;

    if (!QueryPerformanceFrequency (&winFreq))
        printf ("QueryPerformanceFrequency failed\n");

    cout<<"begin execute "<<functionName<<endl;

    if (!QueryPerformanceCounter (&winStart))
        printf ("QueryPerformanceCounter failed\n");

    sp.run(layout);

    if (!QueryPerformanceCounter (&winNow))
        printf ("QueryPerformanceCounter failed\n");

    cout<<"end execute "<<functionName<<endl;

    if (!checker::noOverlaps(layout.getRects()))
        cout <<"Wrong answer! There are overlaps between the rectangles!" <<endl;

    double t= (double)(winNow.QuadPart - winStart.QuadPart) / (double)winFreq.QuadPart;
    cout <<"runtime "<<t<<endl;

    ofstream output(fileName);
    vector<Rect>& rects=layout.getRects();
    output<<rects.size()<<endl;
    for (int i=0; i<(int)rects.size(); i++)
        output<<rects[i].left()<<" "<<rects[i].bottom()<<" "<<rects[i].right()<<" "<<rects[i].top()<<" "<<rects[i].networkID<<endl;
    output<<"runTime "<<t<<endl;
    output.close();

}

void calculateSA()
{
    Layout layout(0.5);
    loadLayout("input.txt", layout);

    //SimulatedAnnealing SA(1,0.1,0.999999,1,0);
    SimulatedAnnealing SA(1,0.1,0.9,1,0);

    //execute block placement by longest path
    SA.setStrategy(new SPPackingStrategy());
    Layout SP_layout(layout,0);
    executeSA(SA,SP_layout,"block placement by longest path","longest_path.log");

    //execute block placement by LCS
    SA.setStrategy(new LCSPackingStrategy());
    Layout LCS_layout(layout,0);
    executeSA(SA,LCS_layout,"block placement by LCS","LCS.log");
}

double executePacking(PackingStrategy &ps, Layout &layout)
{
    LARGE_INTEGER winFreq;
    LARGE_INTEGER winStart, winNow;

    if (!QueryPerformanceFrequency (&winFreq))
        printf ("QueryPerformanceFrequency failed\n");

    //printf("begin execution\n");

    if (!QueryPerformanceCounter (&winStart))
        printf ("QueryPerformanceCounter failed\n");

    ps.initialPacking(layout);
    for (int i=1; i<100; i++)
    {
        ps.nextPackingCommand();
        ps.compPackingLayout(layout);
    }

    if (!QueryPerformanceCounter (&winNow))
        printf ("QueryPerformanceCounter failed\n");

    double t=winNow.QuadPart - winStart.QuadPart;
    t/=double(winFreq.QuadPart);
    cout <<"runtime "<<t<<endl;
    return t;
}

void calculatePackingStrategy()
{
    int m=7;
    int a[]={6,10,20,50,100,200,500};
    for (int i=0; i<m; i++)
    {
        double time_SP=0,time_LCS=0;
        printf("now calculating n = %d\n", a[i]);
        for (int j=0; j<5; j++)
        {
            generateLayout(a[i]);
            Layout layout(0.5);
            loadLayout("input.txt", layout);
            SPPackingStrategy SP_packing;
            LCSPackingStrategy LCS_packing;
            time_SP+=executePacking(SP_packing,layout);
            time_LCS+=executePacking(LCS_packing,layout);
        }
        printf("time of %d : %f %f %f\n",a[i], time_SP, time_LCS, time_SP/time_LCS);
    }
}

int main()
{
    generateLayout(10);
    calculateSA();
    calculatePackingStrategy();
    return 0;
}

