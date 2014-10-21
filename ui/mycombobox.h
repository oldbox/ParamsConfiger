#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H

#include <QComboBox>
#include <QList>
class MyComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(QString currentText READ currentText WRITE setCurrentText NOTIFY currentTextChanged USER true)
    Q_PROPERTY(QString currentValue READ currentValue WRITE setCurrentValue NOTIFY currentValueChanged USER true)
public:
    MyComboBox(QHash<QString, QString> *dropDownListHash, QWidget *parent);
    MyComboBox(QStringList *dropDownList, QWidget *parent);
    ~MyComboBox();
    void setCurrentText(const QString &text);

    QString currentValue();
    void setCurrentValue(const QString &value);
signals:
    void currentTextChanged();
    void currentValueChanged();
    void currentValueChanged(QString currentVal);
private slots:
    void emmitTheCurrentValueChangeSignal();
private:
    QHash<QString, QString>* dropDownHashIn;
};

#endif // MYCOMBOBOX_H
