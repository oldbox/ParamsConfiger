#ifndef DLGSETPARAMS_H
#define DLGSETPARAMS_H

#define L(x) QString::fromUtf8(x)

#define ElectroChemistry 1 // 电化学
#define Organic 3  // 有机物
#define Colorimetric 4  // 比色法
#define AtomicFluorescence 5  // 原子荧光
#define MultipleParameter 6  //
#define SiliconOxide 7


#include <QDialog>
#include <QVBoxLayout>
#include <QHash>
#include <QDataWidgetMapper>

#include "../core/cmainctrl.h"
#include "mycombobox.h"
#include "../core/cdatabase.h"

namespace Ui {
class dlgSetParams;
}

class dlgSetParams : public QDialog
{
    Q_OBJECT
    
public:
    explicit dlgSetParams(CMainCtrl* pMainCtrl, QWidget *parent = 0);
    ~dlgSetParams();
    
private slots:
    void on_pushButton_clicked();
    void saveParam();

private:
    Ui::dlgSetParams *ui;
    CMainCtrl* m_pMainCtrl;
    QGridLayout *gridlayout;
    void generateEditor(QString nameStr, QString editorName, QString unitStr=QString(""), QWidget* parent=0);
    void generateEditor(QString nameStr, QString editorName, QStringList* strListForComboBox, QString unitStr=QString(""), QWidget* parent=0);
    void generateEditor(QString nameStr, QString editorName, QHash<QString, QString>* dropDownListHash, QString unitStr=QString(""), QWidget* parent=0);

    void initSomeInstance();
    void initLayout();
    void initEditors(int yqType);
    void setupModel();
    void addOneRowToModel(char* czGroup);
    int getYQType();
    QStandardItemModel *model;
    QHash<int, QString>* configHash;
    QDataWidgetMapper* mapper;

    int columnNumber;

    CDBBusiness* mdb;
    QList<char*>* yqIDList;

};

#endif // DLGSETPARAMS_H
