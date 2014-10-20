#ifndef MYINPUTPANELCONTEXT_H
 #define MYINPUTPANELCONTEXT_H

 #include <QtGui/QInputContext>

 #include "myinputpanel.h"

 class MyInputPanelContext : public QInputContext
 {
     Q_OBJECT

 public:
     MyInputPanelContext();
     ~MyInputPanelContext();

     bool filterEvent(const QEvent* event);

     QString identifierName();
     QString language();

     bool isComposing() const;

     void reset();
     void close();
 private slots:
     void sendCharacter(QChar character);

 private:
     void updatePosition();

 private:
     MyInputPanel *inputPanel;
     QWidget *m_wiget;
//     QLineEdit *curline;
     bool closePanel;
 };

 #endif // MYINPUTPANELCONTEXT_H
