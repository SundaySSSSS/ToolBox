#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class FileInfo;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_Browse_clicked();

    void on_Start_clicked();

private:
    Ui::Widget *ui;

    //改变文件的编码方式
    //参数： QFileInfo : fileInfo - 要进行编码转换的文件
    //参数： QString : transTo - 要转换到的编码
    bool transEnCode(QFileInfo fileInfo, QString transTo);

};

#endif // WIDGET_H
