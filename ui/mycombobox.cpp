#include "mycombobox.h"

MyComboBox::MyComboBox(QHash<QString, QString> *dropDownListHash,QWidget *parent)
    : QComboBox(parent), dropDownHashIn(dropDownListHash)
{
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(emmitTheCurrentValueChangeSignal()));

  // 初始化下拉列表
  QList<QString> dropDownList = dropDownHashIn->keys();
  for (int i=0; i < dropDownList.size(); i++){
      addItem(dropDownList.at(i));
  }

}

void MyComboBox::emmitTheCurrentValueChangeSignal()
{
    emit currentValueChanged(currentValue());
}

MyComboBox::MyComboBox(QStringList *dropDownList, QWidget *parent)
    : QComboBox(parent)
{
    connect(this, SIGNAL(currentIndexChanged(int)), this, SIGNAL(currentTextChanged()));
    // 初始化下拉列表
    int count = dropDownList->size();
    for (int i =0; i < count; i++)
    {
        addItem(dropDownList->at(i));
    }
}

MyComboBox::~MyComboBox()
{
}
void MyComboBox::setCurrentText(const QString &text)
{
  setCurrentIndex(findText(text));
}

QString MyComboBox::currentValue()
{
    return dropDownHashIn->value(currentText());
}

void MyComboBox::setCurrentValue(const QString &value)
{
    QString key = dropDownHashIn->key(value);
    int index = findText(key);
    setCurrentIndex(index);
}
