#include "dlgsetparams.h"
#include "ui_dlgsetparams.h"
#include <QScrollArea>
#include <QLineEdit>
dlgSetParams::dlgSetParams(CMainCtrl *pMainCtrl, QWidget *parent) :
    QDialog(parent), m_pMainCtrl(pMainCtrl),
    ui(new Ui::dlgSetParams)
{
    ui->setupUi(this);

    initSomeInstance();

    initLayout();

    initEditors(getYQType());

    setupModel();

    connect(ui->paramsNamelistView, SIGNAL(clicked(QModelIndex)),
            mapper, SLOT(setCurrentModelIndex(QModelIndex)));

    mapper->toFirst();

}

dlgSetParams::~dlgSetParams()
{
    delete ui;
    delete mdb;
}

// 根据参数类型生成相应的属性编辑控件
void dlgSetParams::initEditors(int yqType)
{
    switch(yqType)
    {
    case ElectroChemistry:
    {
        generateEditor(L("参数名称"), L("Name")); //参数名称需固定为第一位，其他的可以随意

        generateEditor(L("Modbus地址"), L("SlaveID"));
        generateEditor(L("小数位数"), L("DataDigits"));
        generateEditor(L("寻峰下限"), L("PeakSearchDown"));
        generateEditor(L("寻峰上限"), L("PeakSearchUp"));
        generateEditor(L("标样核查浓度"), L("CheckChroma"));
        generateEditor(L("空白浓度"), L("BlankChroma"));
        generateEditor(L("校准系数"), L("SampleAdjust"));
        generateEditor(L("检出上限"), L("TopLimit"), L("mg/L"));
        generateEditor(L("检出下限"), L("LowerLimit"), L("mg/L"));

        // 各种计算模式对应的数字
        QHash<QString, QString>* dropDownListHash = new QHash<QString, QString>();
        dropDownListHash->insert(L("波长"), L("0"));
        dropDownListHash->insert(L("峰高"), L("1"));
        dropDownListHash->insert(L("峰面积"), L("2"));
        dropDownListHash->insert(L("双波长"), L("3"));
        generateEditor(L("计算模式"), L("CalcWay"), dropDownListHash);

        generateEditor(L("标样浓度"), L("SampleChroma"));

        break;
    }
    case Organic:
    {
        generateEditor(L("参数名称"), L("Name")); //参数名称需固定为第一位，其他的可以随意

        break;
    }
    default:
        break;
    }
}

void dlgSetParams::initSomeInstance()
{
    configHash = new QHash<int, QString>();
    mapper = new QDataWidgetMapper(this);
    model = new QStandardItemModel(this);
    mdb = new CDBBusiness("yqdata", "yq.db");
    yqIDList = new QList<char*>();
    mapper->setModel(model);
    ui->paramsNamelistView->setModel(model);
    ui->paramsNamelistView->setModelColumn(0);
    columnNumber = 0;
}

// 使用LineEdit编辑
void dlgSetParams::generateEditor(QString nameStr, QString editorName, QString unitStr, QWidget* parent)
{
    if(parent==0){
        parent = ui->scrollAreaWidget;
    }
    QLineEdit* editor = new QLineEdit(parent);
    editor->setObjectName(editorName);
    QLabel* nameLabel = new QLabel(nameStr, parent);
    nameLabel->setAlignment(Qt::AlignRight);
    QLabel* unitLabel = new QLabel(unitStr, parent);
    unitLabel->setAlignment(Qt::AlignLeft);
    gridlayout->addWidget(nameLabel, columnNumber, 0);
    gridlayout->addWidget(editor, columnNumber, 1);
    gridlayout->addWidget(unitLabel, columnNumber, 2);

    configHash->insert(columnNumber, editorName);

    mapper->addMapping(editor, columnNumber);

    columnNumber++;

}

// 使用下拉框编辑
void dlgSetParams::generateEditor(QString nameStr, QString editorName, QStringList* strListForComboBox, QString unitStr, QWidget* parent)
{
    if(parent==0){
        parent = ui->scrollAreaWidget;
    }


    MyComboBox* editor = new MyComboBox(strListForComboBox, parent);
    editor->setObjectName(editorName);

    QLabel* nameLabel = new QLabel(nameStr, parent);
    nameLabel->setAlignment(Qt::AlignRight);
    QLabel* unitLabel = new QLabel(unitStr, parent);
    unitLabel->setAlignment(Qt::AlignLeft);

    gridlayout->addWidget(nameLabel, columnNumber, 0);
    gridlayout->addWidget(editor, columnNumber, 1);
    gridlayout->addWidget(unitLabel, columnNumber, 2);

    configHash->insert(columnNumber, editorName);

    mapper->addMapping(editor, columnNumber, "currentText");

    columnNumber++;
}

// 使用自定义的下拉列表控件编辑
void dlgSetParams::generateEditor(QString nameStr, QString editorName, QHash<QString, QString>* dropDownListHash, QString unitStr, QWidget* parent)
{
    if(parent==0){
        parent = ui->scrollAreaWidget;
    }

    MyComboBox* editor = new MyComboBox(dropDownListHash ,parent);
    editor->setObjectName(editorName);

    QLabel* nameLabel = new QLabel(nameStr, parent);
    nameLabel->setAlignment(Qt::AlignRight);
    QLabel* unitLabel = new QLabel(unitStr, parent);
    unitLabel->setAlignment(Qt::AlignLeft);

    gridlayout->addWidget(nameLabel, columnNumber, 0);
    gridlayout->addWidget(editor, columnNumber, 1);
    gridlayout->addWidget(unitLabel, columnNumber, 2);

    configHash->insert(columnNumber, editorName);

    mapper->addMapping(editor, columnNumber, "currentValue");
    columnNumber++;
}

void dlgSetParams::initLayout()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    gridlayout = new QGridLayout(this);
    gridlayout->setSizeConstraint(QGridLayout::SetMinAndMaxSize);
    ui->scrollAreaWidget->setLayout(gridlayout);
}

// 读取配置文件中的参数属性放入model中
void dlgSetParams::setupModel()
{

    sqlite_data yqDB;

    if(mdb->GetYQCode(yqDB))
    {
        for (int i=1; i <= yqDB.RowCount(); i++)
        {
            yqIDList->append((char*)yqDB.GetData(i,1));
        }
     }

    if(yqIDList->size() > 0)
    {
        for (int i=0; i < yqIDList->size(); i++)
        {
            char czGroup[24];memset(czGroup,0,24);
            sprintf(czGroup,"%s_SET",yqIDList->at(i));
            addOneRowToModel(czGroup);
        }
    }
}

void dlgSetParams::addOneRowToModel(char* czGroup)
{

    char value[24]; memset(value, 0, sizeof(value));
    char itemStr[24]; memset(itemStr, 0, sizeof(itemStr));
    QList<QStandardItem*> qlist;

    QHashIterator<int, QString> i(*configHash);
    while (i.hasNext()) {
        i.next();
        memset(value, 0, sizeof(value));
        memset(itemStr, 0, sizeof(itemStr));
        strcpy(itemStr, i.value().toLocal8Bit().data());
        GetAfxConfig()->GetIni(czGroup, itemStr, value);

        QStandardItem* item = new QStandardItem(QString::fromLocal8Bit(value));
        qlist.append(item);
    }

    model->appendRow(qlist);
}

int dlgSetParams::getYQType()
{
    char yqType[8]; memset(yqType, 0, sizeof(yqType));
    GetAfxConfig()->GetIni("INFO", "YQType", yqType);
    return(atoi(yqType));

}


void dlgSetParams::on_pushButton_clicked()
{
    qDebug(model->data(model->index(0,1)).toByteArray().data());
    qDebug(model->data(model->index(0,1)).toByteArray().data());
}


void dlgSetParams::saveParam()
{
    QModelIndex index = ui->paramsNamelistView->currentIndex();
//    QString czID =
    int columnCount = model->columnCount();
    for(int i=0; i < columnCount; i++)
    {
//        GetAfxConfig()->SetIni();
    }
}


