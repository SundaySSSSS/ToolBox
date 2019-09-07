#include "widget.h"
#include "ui_widget.h"
#include "ThreadTool.h"

void funcCount(void* pParam)
{
    static int iCount = 0;
    if (pParam != nullptr)
    {
        Widget* pWidget = static_cast<Widget*>(pParam);
        pWidget->changeCount(iCount);
    }
    ++iCount;
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::changeCount(int iCount)
{
    ui->label_num->setText(QString::number(iCount));
}

void Widget::on_pushButton_start_clicked()
{
    static ThreadTool thread(funcCount, this, 1000, this);
    thread.start();
}
