#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QMainWindow>
#include <QWheelEvent>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainClass; }
QT_END_NAMESPACE

class MainClass : public QMainWindow
{
    Q_OBJECT

public:
    MainClass(QWidget *parent = nullptr);
    ~MainClass();

private slots:
    void on_OpenSignal_button_clicked();
    void mousePress();
    void mouseWheel(QWheelEvent *event);

private:
    Ui::MainClass *ui;
};

#endif 
