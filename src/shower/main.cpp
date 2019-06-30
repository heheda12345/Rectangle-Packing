#include "mainwindow.h"
#include <QApplication>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);
    //if (argc!=2) cout<<"incorrect format!"<<endl;
    //ifstream file(argv[1]);
    //ifstream file("D:/a/SP/longest_path.log");
    ifstream file("D:/a/SP/LCS0.log");
    if (!file) { cout<<"can not open"<<endl; return a.exec();}
    vector<int> num[5];
    int n;
    file>>n;
    for (int i=0;i<5;i++) num[i].resize(n);
    for (int i=0;i<n;i++)
        file>>num[0][i]>>num[1][i]>>num[2][i]>>num[3][i]>>num[4][i];

    MainWindow *dia = new MainWindow(n,num);
    dia->show();

    return a.exec();
}
