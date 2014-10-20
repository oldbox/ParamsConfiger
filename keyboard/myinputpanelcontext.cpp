#include <QtCore>

 #include "myinputpanelcontext.h"

 MyInputPanelContext::MyInputPanelContext()
 {
     closePanel = false;
     inputPanel = new MyInputPanel;
     m_wiget = NULL;
     connect(inputPanel, SIGNAL(characterGenerated(QChar)), this,SLOT(sendCharacter(QChar)));
 }

 MyInputPanelContext::~MyInputPanelContext()
 {
     delete inputPanel;
 }

 bool MyInputPanelContext::filterEvent(const QEvent* event)
 {
     if (event->type() == QEvent::RequestSoftwareInputPanel)
     {
         if (closePanel)
         {
             inputPanel->hide();
             closePanel = false;
         }
         else
         {
             closePanel = true;
             updatePosition();
             inputPanel->show();
         }

         return true;
     }
//     else if (event->type() == QEvent::CloseSoftwareInputPanel)
//     {
//         inputPanel->hide();
//         return true;
//     }

     return false;
 }


 QString MyInputPanelContext::identifierName()
 {
     return "MyInputPanelContext";
 }

 void MyInputPanelContext::reset()
 {
 }
 void MyInputPanelContext::close()
 {
     inputPanel->hide();
     closePanel = false;
 }

 bool MyInputPanelContext::isComposing() const
 {
     return false;
 }

 QString MyInputPanelContext::language()
 {
     return "en_US";
 }


 void MyInputPanelContext::sendCharacter(QChar character)
 {
     if (!m_wiget)
     {
         return;
     }
     if(!m_wiget->isWidgetType())
     {
         inputPanel->hide();
         closePanel = false;
         return;
     }

     if (character == '*')//退出键处理
     {
         return;
     }
     else if (character == '~')//退出键处理
     {
         inputPanel->hide();
         closePanel = false;
         return;
     }
     else if (character == '%')//清空键处理
     {
         if(m_wiget->inherits("QLineEdit"))
         {
              ((QLineEdit*)m_wiget)->setText("");
         }
         else if(m_wiget->inherits("QComboBox"))
         {
             ((QComboBox*)m_wiget)->setEditText("");
         }
     }
     else if (character == '^')//退格键处理
     {
         QString curtext;
         if(m_wiget->inherits("QLineEdit"))
         {
             curtext = ((QLineEdit*)m_wiget)->text();
         }
         else if(m_wiget->inherits("QComboBox"))
         {
             curtext =  ((QComboBox*)m_wiget)->currentText();
         }
         else
         {
             return ;
         }
         cout << curtext.toStdString() << endl;
         curtext.chop(1);
         if(m_wiget->inherits("QLineEdit"))
         {
             ((QLineEdit*)m_wiget)->setText(curtext);
         }
         else if(m_wiget->inherits("QComboBox"))
         {
              ((QComboBox*)m_wiget)->setItemText(0,curtext);
         }
     }
     else
     {
         if (!inputPanel->bUpper)//Shift键处理
         {
             character = character.toLower();
         }

         if (!m_wiget)
         {
             return;
         }

         QKeyEvent keyPress(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString(character));
         QApplication::sendEvent(m_wiget, &keyPress);



         QKeyEvent keyRelease(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString());
         QApplication::sendEvent(m_wiget, &keyRelease);
     }
 }

 void MyInputPanelContext::updatePosition()
 {
     m_wiget = focusWidget();
     if (!m_wiget)
     {
         return;
     }
     QRect widgetRect = m_wiget->rect();
     QPoint panelPos = QPoint(widgetRect.left(), widgetRect.bottom());
     panelPos = m_wiget->mapToGlobal(panelPos);
//     if (panelPos.ry() > 480)//pc机
//     {
//         panelPos.ry() -= (panelPos.ry() - 390);
//     }
//     if (panelPos.rx() > 640)
//     {
//         panelPos.rx() -= (panelPos.rx() - 590);
//     }
     //184为键盘的高，35为QLineEdit的高
     if (panelPos.ry() > 332)//arm版
     {
         panelPos.ry() = panelPos.ry() - 148 -35;
     }
     if (panelPos.rx() > 275)
     {
         panelPos.rx() = 275;
     }
     inputPanel->move(panelPos);
 }
