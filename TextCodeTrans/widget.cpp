#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QDebug>
#include "fileutils.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->lineEdit->setReadOnly(true);
    ui->comboBox_ToCode->addItem("UTF-8");
    ui->comboBox_ToCode->addItem("UTF-8-BOM");
    ui->comboBox_ToCode->addItem("GB2312");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_Browse_clicked()
{
    QString strDir = QFileDialog::getExistingDirectory(this, tr("打开一个文件夹"),
                                                      "",
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
    ui->lineEdit->setText(strDir);
}

void Widget::on_Start_clicked()
{
    //遍历文件夹
    QString strDir = ui->lineEdit->text();
    QFileInfoList fileList = recursionFileInfoReader(strDir);
    foreach (QFileInfo fileInfo, fileList)
    {
        qDebug() << fileInfo.filePath();
    }
}

bool Widget::transEnCode(QFileInfo fileInfo, QString transTo)
{
    //判定编码格式
    //https://www.cnblogs.com/vectorli/p/7814856.html
    //https://www.cnblogs.com/findumars/p/4941351.html
}
