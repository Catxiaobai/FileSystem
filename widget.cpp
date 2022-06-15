#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->copyName->setVisible(false);
    QStandardItemModel* model = new QStandardItemModel(ui->treeView);
    model->setHorizontalHeaderLabels(QStringList()<<"名称"<<"类型");

    QMap<QString,QIcon> m_publicIconMap;
    m_publicIconMap[QStringLiteral("treeItem_Computer")] =QIcon(QStringLiteral(":/Icon/Image/Computer.png"));
    m_publicIconMap[QStringLiteral("treeItem_Disk")] =QIcon(QStringLiteral(":/Icon/Image/Disk.png"));
    m_publicIconMap[QStringLiteral("treeItem_Project")] =QIcon(QStringLiteral(":/Icon/Image/Project.png"));
    m_publicIconMap[QStringLiteral("treeItem_Unknownfile")] =QIcon(QStringLiteral(":/Icon/Image/Unknownfile.png"));
    m_publicIconMap[QStringLiteral("treeItem_txt")] =QIcon(QStringLiteral(":/Icon/Image/txt.png"));


    QVector<QString> myDisks={"C盘","D盘","E盘"};

    QStandardItem* myComputer = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_Computer")],QStringLiteral("我的电脑"));
    model->appendRow(myComputer);
    model->setItem(0,1,new QStandardItem(QStringLiteral("system")));


    for(int i=0;i<myDisks.size();i++){
        QStandardItem* myDisk = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_Disk")],myDisks[i]);
        myComputer->appendRow(myDisk);
        myComputer->setChild(i,1,new QStandardItem(QStringLiteral("驱动器")));
        for(int j=1;j<3;j++){
            QStandardItem* myProject = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_Project")],QStringLiteral("文件夹")+QString("%1").arg(j));
            myDisk->appendRow(myProject);
            myDisk->setChild(j-1,1,new QStandardItem(QStringLiteral("文件夹")));

            QString fileName="文件.txt";
            QStringList strList = fileName.split(".");
            if(strList.size()<2||strList[1]!="txt"){
                QStandardItem* myFile = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_file")],fileName);
                myProject->appendRow(myFile);
                myProject->setChild(0,1,new QStandardItem("未知文件"));
            }else{
                QStandardItem* myFile = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_txt")],fileName);
                myProject->appendRow(myFile);
                myProject->setChild(0,1,new QStandardItem(strList[1]+QStringLiteral("文件")));
            }
        }
    }



    ui->treeView->setModel(model);
    ui->treeView->header()->resizeSection(0,300);
    //ui->treeView->expandAll();

    //右键菜单
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);



}


//析构函数
Widget::~Widget()
{
    delete ui;
}

//鼠标点击事件
void Widget::on_treeView_clicked(const QModelIndex &index)
{
    show_path();
}

//显示当前路径
QString Widget::show_path(){
    QModelIndex index = ui->treeView->currentIndex();
    QString name,info;
    if(index.column() == 0)
        {
            name = index.data().toString();
            info = index.sibling(index.row(),1).data().toString();
        }
        else
        {
            name = index.sibling(index.row(),0).data().toString();
            info = index.data().toString();
        }
    QString currentPath = "";
    if(info=="system"||info=="驱动器"||info=="文件夹"){
        currentPath = QStringLiteral("%1").arg(index.data().toString()); //当前路径
        while(index.parent().isValid()){
            index=index.parent();
            currentPath=QStringLiteral("%1").arg(index.data().toString())+"/"+currentPath;
        }
    }else{
        while(index.parent().isValid()){
            index=index.parent();
            currentPath=QStringLiteral("%1").arg(index.data().toString())+"/"+currentPath;
        }
    }
    ui->label->setText(currentPath);
    return info;
}

//右键菜单
void Widget::on_treeView_customContextMenuRequested(const QPoint &pos)
{

    QString copyFile = ui->copyName->text();
    if(show_path()=="system"||show_path()=="驱动器"||show_path()==""){

    }else if(show_path()=="文件夹"){
        //右键菜单
              QMenu *menu = new QMenu(ui->treeView);
              menu->addAction("新建文件夹",this,&Widget::new_project);
              menu->addAction("新建文件",this,&Widget::new_file);
              menu->addAction("删除",this,&Widget::delete_project);
              if(copyFile=="已复制文件")
                menu->addAction("粘贴")->setEnabled(false);
              else
                menu->addAction("粘贴",this,&Widget::paste_file)->setEnabled(true);
              menu->exec(ui->treeView->mapToGlobal(pos));

    }else{
        QMenu *menu = new QMenu(ui->treeView);
        menu->addAction("新建文件夹",this,&Widget::new_project);
        menu->addAction("新建文件",this,&Widget::new_file);
        menu->addAction("删除",this,&Widget::delete_file);
        menu->addAction("复制",this,&Widget::copy_file);
        if(copyFile=="已复制文件")
          menu->addAction("粘贴")->setEnabled(false);
        else
          menu->addAction("粘贴",this,&Widget::paste_file)->setEnabled(true);
        menu->exec(ui->treeView->mapToGlobal(pos));
    }

}

//新建文件夹
void Widget::new_project(){
    qint64 timestamp = QDateTime::currentSecsSinceEpoch();
    QString time=tr("%1").arg(timestamp);
    QStandardItem* myProject = new QStandardItem(QIcon(QStringLiteral(":/Icon/Image/Project.png")),QStringLiteral("新建文件夹")+time);
    QStandardItemModel*   model  =   static_cast< QStandardItemModel*>( ui-> treeView-> model());
    QModelIndex   currentIndex  =   ui-> treeView-> currentIndex();
    QStandardItem*   currentItem  =   model-> itemFromIndex( currentIndex.parent() );
    currentItem->appendRow(myProject);
    currentItem->setChild(myProject->index().row(),1,new QStandardItem(QStringLiteral("文件夹")));
    ui->treeView->setCurrentIndex(myProject->index());
}

//新建文件
void Widget::new_file(){
    qint64 timestamp = QDateTime::currentSecsSinceEpoch();
    QString time=tr("%1").arg(timestamp);
    QStandardItem* myFile = new QStandardItem(QIcon(QStringLiteral(":/Icon/Image/txt.png")),"新建文件"+time+".txt");
    QStandardItemModel*   model  =   static_cast< QStandardItemModel*>( ui-> treeView-> model());
    QModelIndex   currentIndex  =   ui-> treeView-> currentIndex();
    QStandardItem*   currentItem  =   model-> itemFromIndex( currentIndex );
    if(show_path()!="文件夹"){
        currentItem  =   model-> itemFromIndex( currentIndex.parent() );
    }
    currentItem->appendRow(myFile);
    currentItem->setChild(myFile->index().row(),1,new QStandardItem(QStringLiteral("txt文件")));
    //ui->treeView->setExpanded(currentIndex,1);
    ui->treeView->setCurrentIndex(myFile->index());
}

//删除文件夹
void Widget::delete_project(){
    QStandardItemModel*   model  =   static_cast< QStandardItemModel*>( ui-> treeView-> model());
    QModelIndex   currentIndex  =   ui-> treeView-> currentIndex();
    QStandardItem*   currentItem  =   model-> itemFromIndex( currentIndex.parent() );
    currentItem->removeRow(currentIndex.row());
}

//删除文件
void Widget::delete_file(){
    QStandardItemModel*   model  =   static_cast< QStandardItemModel*>( ui-> treeView-> model());
    QModelIndex   currentIndex  =   ui-> treeView-> currentIndex();
    QStandardItem*   currentItem  =   model-> itemFromIndex( currentIndex.parent() );
    currentItem->removeRow(currentIndex.row());
}

//复制文件
void Widget::copy_file(){
    QModelIndex   currentIndex  =   ui-> treeView-> currentIndex();
    ui->copyName->setText( QStringLiteral("%1").arg(currentIndex.data().toString()));
}

//粘贴文件
void Widget::paste_file(){
    qint64 timestamp = QDateTime::currentSecsSinceEpoch();
    QString time=tr("%1").arg(timestamp);
    QStandardItem* myFile = new QStandardItem(QIcon(QStringLiteral(":/Icon/Image/txt.png")),ui->copyName->text());
    QStandardItemModel*   model  =   static_cast< QStandardItemModel*>( ui-> treeView-> model());
    QModelIndex   currentIndex  =   ui-> treeView-> currentIndex();
    QStandardItem*   currentItem  =   model-> itemFromIndex( currentIndex );
    if(show_path()!="文件夹"){
        currentItem  =   model-> itemFromIndex( currentIndex.parent() );
    }
    currentItem->appendRow(myFile);
    currentItem->setChild(myFile->index().row(),1,new QStandardItem(QStringLiteral("txt文件")));
    //ui->treeView->setExpanded(currentIndex,1);
    ui->copyName->setText("已复制文件");
    ui->treeView->setCurrentIndex(myFile->index());
}
