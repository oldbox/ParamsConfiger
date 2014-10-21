#ifndef DLGSETPARAMS2_H
#define DLGSETPARAMS2_H

#define L(x) QString::fromUtf8(x)

#define ElectroChemistry 1 // 电化学
#define Organic 3  // 有机物
#define Colorimetric 4  // 比色法
#define AtomicFluorescence 5  // 原子荧光
#define MultipleParameter 6  // 多参数
#define SiliconOxide 7  // 二氧化硅

#include <QDialog>
#include <QList>
#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include "../core/cdbbusiness.h"
#include "../core/cconfig.h"

#include <QListView>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QLabel>
#include "ui/mycombobox.h"

namespace Ui {
class dlgSetParams2;
}

class dlgSetParams2 : public QDialog
{
    Q_OBJECT
    
public:
    explicit dlgSetParams2(QWidget *parent = 0);
    ~dlgSetParams2();
private slots:
    void mapperToProperModelAndRow(QModelIndex index);
    void mapperToProperModelAndRow(QString index);
    
private:
    Ui::dlgSetParams2 *ui;
    QList<QHash<int, QString>*>* configHashList;
    QList<QStandardItemModel *>* modelList;
    QList<int>* columnNumberList;
    QList<QDataWidgetMapper *>* mapperList;
    int methodsCounts;
    CDBBusiness* mdb;
    QList<QString>* yqIDList;
    QStandardItemModel* modelForNameList;
    QStackedWidget* propertyListStackedWidgetGlobal;
    QListView* paramsNameListGlobal;
    MyComboBox* methodSelectComboboxGlobal;
    void initSomeInstance();
    void initLayout();
    void initConnect();

    QList<int> parseAvailableMethods();

    void generateEditor(QString nameStr, QString editorName, int methodCode, \
                        QString unitStr=QString(""),  QWidget* parent=0);
    void generateEditor(QString nameStr, QString editorName, QStringList* strListForComboBox, int methodCode, \
                        QString unitStr=QString(""), QWidget* parent=0);
    void generateEditor(QString nameStr, QString editorName, QHash<QString, QString>* dropDownListHash, int methodCode,\
                        QString unitStr=QString(""), QWidget* parent=0);

    void addProperties();
    void generateModels();
    void addOneRowToModel(char* czGroup, int processType);


};

#endif // DLGSETPARAMS2_H
