#include<QPainter>
#include<QDebug>
#include "mainwindow.h"
using namespace std;
MainWindow::MainWindow(int n_,vector<int> a_[5],QWidget *parent)
    : QMainWindow(parent),n(n_)
{
    for (int i=0;i<5;i++) a[i]=a_[i];
    for (int j=0;j<4;j++)
        for (int i=0;i<n;i++)
            a[j][i]*=15;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    int X=0,Y=0;
    for (int i=0;i<n;i++)
        X=max(X,a[2][i]);
    for (int i=0;i<n;i++)
        Y=max(Y,a[3][i]);
    //resize(X+10,Y+10);
    //qDebug()<<"window size"<<X+10<<" "<<Y+10<<endl;
    QPainter painter(this);
    const int R=30;
    for (int i=0; i<n; i++)
    {
        painter.setPen(Qt::gray);
        painter.setBrush(Qt::gray);
        painter.drawRect(a[0][i]/R,a[1][i]/R,(a[2][i]-a[0][i])/R,(a[3][i]-a[1][i])/R);
        //painter.setBrush(QColor(0,0,0));
        //painter.drawText((a[0][i]+a[2][i])/2/R,(a[1][i]+a[3][i])/2/R,QString::number(a[4][i]));
    }
}

MainWindow::~MainWindow()
{

}

