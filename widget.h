#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QIcon>
#include <QMap>
#include <QColor>
#include <QMenu>
#include <QTime>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_treeView_customContextMenuRequested(const QPoint &pos);

    void new_project();
    void new_file();
    void delete_project();
    void delete_file();
    void copy_file();
    void paste_file();
    QString show_path();

private:
    Ui::Widget *ui;

};
#endif // WIDGET_H
