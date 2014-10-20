#include "dlgsetparams2.h"
#include "ui_dlgsetparams2.h"

dlgSetParams2::dlgSetParams2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgSetParams2)
{
    ui->setupUi(this);

    initSomeInstance();
    initLayout();
    addProperties();
    generateModels();

    initConnect();
}

dlgSetParams2::~dlgSetParams2()
{
    delete ui;
}

void dlgSetParams2::initSomeInstance()
{
    methodsCounts = 7;

    configHashList = new QList<QHash<int, QString>*>();
    for (int i=0; i <= methodsCounts; i++)
    {
        QHash<int, QString>* configHash = new QHash<int, QString>();
        configHashList->append(configHash);
    }

    modelList = new QList<QStandardItemModel *>();
    for (int i=0; i <= methodsCounts; i++)
    {
        QStandardItemModel* model = new QStandardItemModel(this);
        modelList->append(model);
    }

    columnNumberList = new QList<int>();
    for (int i=0; i <= methodsCounts; i++)
    {
        int columnNumber = 0;
        columnNumberList->append(columnNumber);
    }

    mapperList = new QList<QDataWidgetMapper*>();
    for (int i=0; i <= methodsCounts; i++)
    {
        QDataWidgetMapper* mapper = new QDataWidgetMapper(this);
        mapper->setModel(modelList->at(i));
        mapper->toFirst();
        mapperList->append(mapper);
    }

    mdb = new CDBBusiness("yqdata", "yq.db");
    yqIDList = new QList<QString>();
    modelForNameList = new QStandardItemModel(this);
    sqlite_data yqDB;
    if(mdb->GetYQCode(yqDB))
    {
        for (int i=1; i <= yqDB.RowCount(); i++)
        {
            QString IDCode = L(yqDB.GetData(i, 1));
            yqIDList->append(IDCode);
        }
     }

    if(yqIDList->size() > 0)
    {
        QList<QStandardItem*> qlist;
        QStandardItem* item = new QStandardItem();
        for (int i=0; i < yqIDList->size(); i++)
        {
            qlist.clear();
            char czGroup[24];memset(czGroup,0,sizeof(czGroup));
            char czValue[16];memset(czValue, 0, sizeof(czValue));
            sprintf(czGroup,"%s_SET",yqIDList->at(i).toLocal8Bit().data());

            GetAfxConfig()->GetIni(czGroup, "Name", czValue);
            item = new QStandardItem(QString::fromLocal8Bit(czValue));
            qlist.append(item);

            memset(czValue, 0, sizeof(czValue));
            GetAfxConfig()->GetIni(czGroup, "ProcessType", czValue);
            item = new QStandardItem(QString::fromLocal8Bit(czValue));
            qlist.append(item);

            item = new QStandardItem(yqIDList->at(i));
            qlist.append(item);

            modelForNameList->appendRow(qlist);
        }
    }

}

void dlgSetParams2::initLayout()
{
    //  参数名称
    QListView* paramsNameList = new QListView(ui->paramNameGroupBox);
    paramsNameList->setGeometry(ui->paramNameGroupBox->geometry());
    paramsNameList->setObjectName("paramsNameList");

    QVBoxLayout* nameVboxlayout = new QVBoxLayout(ui->paramNameGroupBox);
    nameVboxlayout->addWidget(paramsNameList);

    paramsNameList->setModel(modelForNameList);
    paramsNameList->setModelColumn(0);

    ui->paramNameGroupBox->setLayout(nameVboxlayout);

    // 参数属性
    QStackedWidget* propertyListStackedWidget = new QStackedWidget(ui->paramsPropetiesGroupBox);
    propertyListStackedWidget->setGeometry(ui->paramsPropetiesGroupBox->geometry());
    propertyListStackedWidget->setObjectName("propertyListStackedWidget");
    propertyListStackedWidgetGlobal = propertyListStackedWidget;

    QVBoxLayout* propertyVboxlayout = new QVBoxLayout(ui->paramsPropetiesGroupBox);
    propertyVboxlayout->addWidget(propertyListStackedWidget);

    ui->paramsPropetiesGroupBox->setLayout(propertyVboxlayout);

    // 在 propertyListStackedWidget 中为每个方法插入一个widget（page）
    for(int i=1; i <= methodsCounts; i++)
    {
        QWidget* widget = new QWidget(this);
        QString widgetName = QString("widgetForMethod%1").arg(i); // 为widget设置ObjectName
        widget->setObjectName(widgetName);
        QWidget* widgetInner = new QWidget(widget);
        QScrollArea* scrollArea = new QScrollArea(widget);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        QVBoxLayout* vboxLayout = new QVBoxLayout(widget);
        QGridLayout* gridLayout = new QGridLayout(this);
        gridLayout->setSizeConstraint(QGridLayout::SetMinAndMaxSize);
        QString gridLayoutName = QString("layourtForMethod%1").arg(i); // 为gridlayout设置ObjectName
        gridLayout->setObjectName(gridLayoutName);

        vboxLayout->addWidget(scrollArea);
        scrollArea->setWidget(widgetInner);
        widgetInner->setLayout(gridLayout);

        propertyListStackedWidget->addWidget(widget); // 添加到stackWidget中
    }
}

void dlgSetParams2::addProperties()
{
    // 用下拉列表选择的属性
    // -- 计算模式  及 对应的数字
    QHash<QString, QString>* dropDownListHash = new QHash<QString, QString>();
    dropDownListHash->insert(L("波长"), L("0"));
    dropDownListHash->insert(L("峰高"), L("1"));
    dropDownListHash->insert(L("峰面积"), L("2"));
    dropDownListHash->insert(L("双波长"), L("3"));
    // -- 单位 及 对应的数字
    QHash<QString, QString>* UnitHash = new QHash<QString, QString>();
    UnitHash->insert(L("μg/L"), L("0"));
    UnitHash->insert(L("mg/L"), L("1"));
    // -- 测试优先级
    QStringList testPriorityList;
    testPriorityList << "1" << "2" << "3" << "4" << "5";

    //  公共属性
    for(int i=1; i <= methodsCounts; i++)
    {
        generateEditor(L("参数代码"), L("ID"), i); //参数代码需固定为第一位，其他的可以随意
        generateEditor(L("参数名称"), L("Name"), i);
        generateEditor(L("测试方法"), L("ProcessType"), i);
        generateEditor(L("Modbus地址"), L("SlaveID"), i);
        generateEditor(L("小数位数"), L("DataDigits"), i);
        generateEditor(L("检出上限"), L("TopLimit"), i, L("mg/L"));
        generateEditor(L("检出下限"), L("LowerLimit"), i, L("mg/L"));
    }

    // 电化学
    int i = ElectroChemistry;
    generateEditor(L("寻峰下限"), L("PeakSearchDown"), i);
    generateEditor(L("寻峰上限"), L("PeakSearchUp"), i);
    generateEditor(L("标样核查浓度"), L("CheckChroma"), i);
    generateEditor(L("空白浓度"), L("BlankChroma"), i);
    generateEditor(L("校准系数"), L("SampleAdjust"), i);
    generateEditor(L("计算模式"), L("CalcWay"), dropDownListHash, i);
    generateEditor(L("标样浓度"), L("SampleChroma"), i);

    // 有机物
    i = Organic;
    generateEditor(L("最小值"), L("Min"), i);
    generateEditor(L("最大值"), L("Max"), i);
    generateEditor(L("标样核查浓度"), L("CheckChroma"), i);
    generateEditor(L("校准系数"), L("SampleAdjust"), i);
    generateEditor(L("计算模式"), L("CalcWay"), dropDownListHash, i);

    // 比色法
    i = Colorimetric;
    generateEditor(L("量程上限"), L("Min"), i);
    generateEditor(L("量程下限"), L("Max"), i);
    generateEditor(L("单位"), L("UnitType"), UnitHash, i);
    generateEditor(L("计算模式"), L("CalcWay"), dropDownListHash, i);

    // 原子荧光
    i = AtomicFluorescence;
    generateEditor(L("最小值"), L("Min"), i);
    generateEditor(L("最大值"), L("Max"), i);
    generateEditor(L("标样核查浓度"), L("CheckChroma"), i);
    generateEditor(L("空白浓度"), L("BlankChroma"), i);
    generateEditor(L("校准系数"), L("SampleAdjust"), i);
    generateEditor(L("计算模式"), L("CalcWay"), dropDownListHash, i);
    generateEditor(L("寻峰下限"), L("PeakSearchDown"), i);
    generateEditor(L("寻峰上限"), L("PeakSearchUp"), i);
    generateEditor(L("标样浓度"), L("SampleChroma"), i);
    generateEditor(L("浓度单位"), L("UnitType"), UnitHash, i);

    // 多参数
    i = MultipleParameter;
    generateEditor(L("最小值"), L("Min"), i);
    generateEditor(L("最大值"), L("Max"), i);
    generateEditor(L("标样核查浓度"), L("CheckChroma"), i);
    generateEditor(L("空白浓度"), L("BlankChroma"), i);
    generateEditor(L("基线波长"), L("BaseWaveLength"), i);
    generateEditor(L("校准系数"), L("SampleAdjust"), i);
    generateEditor(L("计算模式"), L("CalcWay"), dropDownListHash, i);
    generateEditor(L("左基点"), L("PeakSearchDown"), i);
    generateEditor(L("右基点"), L("PeakSearchUp"), i);
    generateEditor(L("标样浓度"), L("SampleChroma"), i);

    // 二氧化硅
    i = SiliconOxide;
    generateEditor(L("测试优先级"), L("priority"), &testPriorityList, i);
    generateEditor(L("单位"), L("UnitType"), UnitHash, i);
    generateEditor(L("计算模式"), L("CalcWay"), dropDownListHash, i);
}

void dlgSetParams2::generateModels()
{
    int size = yqIDList->size();
    for (int i = 0; i < size; i++)
    {
        char czGroup[24];memset(czGroup,0,24);
        sprintf(czGroup,"%s_SET",yqIDList->at(i).toLocal8Bit().data());
        char processType[16]; memset(processType, 0, sizeof(processType));
        GetAfxConfig()->GetIni(czGroup, "ProcessType", processType);
        addOneRowToModel(czGroup, atoi(processType));
    }
}

void dlgSetParams2::addOneRowToModel(char *czGroup, int processType)
{
    QStandardItemModel* model = modelList->at(processType);
    QHash<int, QString>* configHash = configHashList->at(processType);

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

void dlgSetParams2::initConnect()
{
    QListView* paramsNameList = ui->paramNameGroupBox->findChild<QListView *>("paramsNameList");
    connect(paramsNameList, SIGNAL(clicked(QModelIndex)),
            this, SLOT(mapperToProperModelAndRow(QModelIndex)));
}

void dlgSetParams2::mapperToProperModelAndRow(QModelIndex index)
{
    int row = index.row();
    QModelIndex processTypeIndex = modelForNameList->index(row, 1);
    QModelIndex czIDIndex = modelForNameList->index(row, 2);
    int processType = modelForNameList->data(processTypeIndex).toInt();
    QString czID = modelForNameList->data(czIDIndex).toString();

    QString widgetName = QString("widgetForMethod%1").arg(processType);
    QWidget* widget = this->findChild<QWidget*>(widgetName);
    QDataWidgetMapper* mapper = mapperList->at(processType);
    QStandardItemModel* model = modelList->at(processType);

    propertyListStackedWidgetGlobal->setCurrentWidget(widget);
    QList<QStandardItem*> resultList;
    resultList = model->findItems(czID, Qt::MatchExactly, 0);
    if(resultList.isEmpty())
//        appendOneRowToTheModel();
        ;
    else
    {
        QStandardItem* item = resultList.at(0);
        QModelIndex resultIndex = model->indexFromItem(item);
        mapper->setCurrentModelIndex(resultIndex);
    }
}

void dlgSetParams2::generateEditor(QString nameStr, QString editorName, int methodCode, QString unitStr, QWidget *parent)
{

    QString gridLayoutName = QString("layourtForMethod%1").arg(methodCode);
    QGridLayout* gridlayout = this->findChild<QGridLayout*>(gridLayoutName);

    QString widgetName = QString("widgetForMethod%1").arg(methodCode);
    QWidget* widget = this->findChild<QWidget*>(widgetName);
    int columnNumber = columnNumberList->at(methodCode);

    if(parent==0){
        parent = widget;
    }

//    QHash<int, QString>* configHash = configHashList[methodCode];
    QHash<int, QString>* configHash = configHashList->at(methodCode);
    QDataWidgetMapper* mapper = mapperList->at(methodCode);

    QLineEdit* editor = new QLineEdit(parent);
    editor->setObjectName(editorName);
    editor->setMinimumWidth(120);

    QLabel* nameLabel = new QLabel(nameStr, parent);
    nameLabel->setAlignment(Qt::AlignRight);
    QLabel* unitLabel = new QLabel(unitStr, parent);
    unitLabel->setAlignment(Qt::AlignLeft);

    gridlayout->addWidget(nameLabel, columnNumber, 0);
    gridlayout->addWidget(editor, columnNumber, 1);
    gridlayout->addWidget(unitLabel, columnNumber, 2);

    configHash->insert(columnNumber, editorName);

    mapper->addMapping(editor, columnNumber);


    columnNumberList->replace(methodCode, columnNumber+1);
}

void dlgSetParams2::generateEditor(QString nameStr, QString editorName, QStringList *strListForComboBox, int methodCode, QString unitStr, QWidget *parent)
{
    QString gridLayoutName = QString("layourtForMethod%1").arg(methodCode);
    QGridLayout* gridlayout = this->findChild<QGridLayout*>(gridLayoutName);

    QString widgetName = QString("widgetForMethod%1").arg(methodCode);
    QWidget* widget = this->findChild<QWidget*>(widgetName);
    int columnNumber = columnNumberList->at(methodCode);

    if(parent==0){
        parent = widget;
    }

    QHash<int, QString>* configHash = configHashList->at(methodCode);
    QDataWidgetMapper* mapper = mapperList->at(methodCode);


    MyComboBox* editor = new MyComboBox(strListForComboBox, parent);
    editor->setObjectName(editorName);
    editor->setMinimumWidth(120);

    QLabel* nameLabel = new QLabel(nameStr, parent);
    nameLabel->setAlignment(Qt::AlignRight);
    QLabel* unitLabel = new QLabel(unitStr, parent);
    unitLabel->setAlignment(Qt::AlignLeft);

    gridlayout->addWidget(nameLabel, columnNumber, 0);
    gridlayout->addWidget(editor, columnNumber, 1);
    gridlayout->addWidget(unitLabel, columnNumber, 2);

    configHash->insert(columnNumber, editorName);

    mapper->addMapping(editor, columnNumber, "currentText");

    columnNumberList->replace(methodCode, columnNumber+1);
}

void dlgSetParams2::generateEditor(QString nameStr, QString editorName, QHash<QString, QString>* dropDownListHash, int methodCode, QString unitStr, QWidget *parent)
{
    QString gridLayoutName = QString("layourtForMethod%1").arg(methodCode);
    QGridLayout* gridlayout = this->findChild<QGridLayout*>(gridLayoutName);

    QString widgetName = QString("widgetForMethod%1").arg(methodCode);
    QWidget* widget = this->findChild<QWidget*>(widgetName);
    int columnNumber = columnNumberList->at(methodCode);

    if(parent==0){
        parent = widget;
    }

    QHash<int, QString>* configHash = configHashList->at(methodCode);
    QDataWidgetMapper* mapper = mapperList->at(methodCode);


    MyComboBox* editor = new MyComboBox(dropDownListHash, parent);
    editor->setObjectName(editorName);
    editor->setMinimumWidth(120);

    QLabel* nameLabel = new QLabel(nameStr, parent);
    nameLabel->setAlignment(Qt::AlignRight);
    QLabel* unitLabel = new QLabel(unitStr, parent);
    unitLabel->setAlignment(Qt::AlignLeft);

    gridlayout->addWidget(nameLabel, columnNumber, 0);
    gridlayout->addWidget(editor, columnNumber, 1);
    gridlayout->addWidget(unitLabel, columnNumber, 2);

    configHash->insert(columnNumber, editorName);

    mapper->addMapping(editor, columnNumber, "currentValue");

    columnNumberList->replace(methodCode, columnNumber+1);
}
