#ifndef MYINPUTPANEL_H
#define MYINPUTPANEL_H

#include <QtGui>
#include <QtCore>
#include <iostream>
using namespace std;

#include "ui_inputpanelform.h"

class MyInputPanel : public QWidget
{
    Q_OBJECT

public:
    MyInputPanel();
    bool bUpper;

signals:
    void characterGenerated(QChar character);

private slots:
    void buttonClicked(QWidget *w);
    void on_key_shift_stateChanged(int arg1);
    void on_key_Switch_clicked();

    void on_key_Enter_2_clicked();

private:
    Ui::inputpanelform form;
    QSignalMapper signalMapper;

    void SwithToNum();
};

#endif // MYINPUTPANEL_H
