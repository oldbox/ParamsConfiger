#include "myinputpanel.h"

 MyInputPanel::MyInputPanel()
     : QWidget(0, Qt::Tool | Qt::WindowStaysOnTopHint)
 {
     bUpper = false;
     form.setupUi(this);

     signalMapper.setMapping(form.key_0, form.key_0);
     signalMapper.setMapping(form.key_1, form.key_1);
     signalMapper.setMapping(form.key_2, form.key_2);
     signalMapper.setMapping(form.key_3, form.key_3);
     signalMapper.setMapping(form.key_4, form.key_4);
     signalMapper.setMapping(form.key_5, form.key_5);
     signalMapper.setMapping(form.key_6, form.key_6);
     signalMapper.setMapping(form.key_7, form.key_7);
     signalMapper.setMapping(form.key_8, form.key_8);
     signalMapper.setMapping(form.key_9, form.key_9);
     signalMapper.setMapping(form.key_Q, form.key_Q);
     signalMapper.setMapping(form.key_W, form.key_W);
     signalMapper.setMapping(form.key_E, form.key_E);
     signalMapper.setMapping(form.key_R, form.key_R);
     signalMapper.setMapping(form.key_T, form.key_T);
     signalMapper.setMapping(form.key_Y, form.key_Y);
     signalMapper.setMapping(form.key_U, form.key_U);
     signalMapper.setMapping(form.key_I, form.key_I);
     signalMapper.setMapping(form.key_O, form.key_O);
     signalMapper.setMapping(form.key_P, form.key_P);
     signalMapper.setMapping(form.key_A, form.key_A);
     signalMapper.setMapping(form.key_S, form.key_S);
     signalMapper.setMapping(form.key_D, form.key_D);
     signalMapper.setMapping(form.key_F, form.key_F);
     signalMapper.setMapping(form.key_G, form.key_G);
     signalMapper.setMapping(form.key_H, form.key_H);
     signalMapper.setMapping(form.key_J, form.key_J);
     signalMapper.setMapping(form.key_K, form.key_K);
     signalMapper.setMapping(form.key_L, form.key_L);
     signalMapper.setMapping(form.key_Z, form.key_Z);
     signalMapper.setMapping(form.key_X, form.key_X);
     signalMapper.setMapping(form.key_C, form.key_C);
     signalMapper.setMapping(form.key_V, form.key_V);
     signalMapper.setMapping(form.key_B, form.key_B);
     signalMapper.setMapping(form.key_N, form.key_N);
     signalMapper.setMapping(form.key_M, form.key_M);
//     signalMapper.setMapping(form.key_star, form.key_star);
//     signalMapper.setMapping(form.key_hash, form.key_hash);
     signalMapper.setMapping(form.key_dot, form.key_dot);
     signalMapper.setMapping(form.key_sub, form.key_sub);
     signalMapper.setMapping(form.key_clear, form.key_clear);
     signalMapper.setMapping(form.key_blank, form.key_blank);
     signalMapper.setMapping(form.key_Escape, form.key_Escape);
     signalMapper.setMapping(form.key_backspace, form.key_backspace);

     signalMapper.setMapping(form.key_Enter, form.key_Enter);
     signalMapper.setMapping(form.key_Enter_2, form.key_Enter_2);
     signalMapper.setMapping(form.key_clear_2, form.key_clear_2);


     connect(form.key_0, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_1, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_2, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_3, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_4, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_5, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_6, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_7, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_8, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_9, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_Q, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_W, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_E, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_R, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_T, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_Y, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_U, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_I, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_O, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_P, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_A, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_S, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_D, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_F, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_G, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_H, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_J, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_K, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_L, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_Z, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_X, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_C, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_V, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_B, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_N, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_M, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
//     connect(form.key_star, SIGNAL(clicked()),
//             &signalMapper, SLOT(map()));
//     connect(form.key_hash, SIGNAL(clicked()),
//             &signalMapper, SLOT(map()));
     connect(form.key_clear, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_blank, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_sub, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_dot, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_Escape, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_backspace, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));

     connect(form.key_Enter, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_Enter_2, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));
     connect(form.key_clear_2, SIGNAL(clicked()),
             &signalMapper, SLOT(map()));

     connect(&signalMapper, SIGNAL(mapped(QWidget*)),
             this, SLOT(buttonClicked(QWidget*)));
     SwithToNum();
     this->setWindowModality(Qt::ApplicationModal);
 }

 void MyInputPanel::buttonClicked(QWidget *w)
 {
     QPushButton *pushbtn = (QPushButton*)w;
     QString btntext = pushbtn->text();
     QChar chr;
     if(btntext == "Num")
     {
          chr = '*';
          SwithToNum();
     }
     else if ( btntext == "Enter")
     {
         chr = '~';
         SwithToNum();
     }
     else if (btntext == "Clear")
     {
         chr = '%';
     }
     else if (btntext == "Back")
     {
         chr = '^';
     }
     else if (btntext == "Blank")
     {
         chr = ' ';
     }
     else
     {
         chr = *btntext.data();
     }

     cout << "Current push button is " << btntext.toStdString() << endl;
     emit characterGenerated(chr);
 }

void MyInputPanel::on_key_shift_stateChanged(int arg1)
{
    if (2 == arg1)
    {
        bUpper = true;
    }
    else
    {
        bUpper = false;
    }
}

void MyInputPanel::on_key_Switch_clicked()
{
    form.page_2->hide();
    form.page->show();
}
void MyInputPanel::SwithToNum()
{
    form.page->hide();
    form.page_2->show();
}

void MyInputPanel::on_key_Enter_2_clicked()
{
    this->hide();
}
