#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int n_,std::vector<int> a_[5],QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *e);

private:
    int n;
    std::vector<int> a[5];
};

#endif // MAINWINDOW_H
