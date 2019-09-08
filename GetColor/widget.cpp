#include "widget.h"
#include "ui_widget.h"
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QKeyEvent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(slot_timeout()));
    m_timer.start(50);

    ui->lineEdit_RGB_now->setReadOnly(true);
    ui->lineEdit_RGB_marked->setReadOnly(true);
    setFocusPolicy(Qt::StrongFocus);
}

Widget::~Widget()
{
    delete ui;
}

QColor Widget::getColor()
{
    QColor color = Qt::black;
    //获取图片位置
    QPoint p = QCursor::pos();
    //抓取鼠标位置一像素的pixmap
    QPixmap pixmap = QGuiApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId(), p.x(), p.y(), 1, 1);
    if (!pixmap.isNull())
    {
        QImage image = pixmap.toImage();
        if (!image.isNull())
        {
            if (image.valid(0, 0))
            {
                color = QColor(image.pixel(0, 0));
            }
        }
    }
    return color;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if (Qt::Key_Space == event->key())
    {
        QString strStyleSheet = ui->label_color->styleSheet();
        ui->label_color_marked->setStyleSheet(strStyleSheet);
        QString strRgb = ui->lineEdit_RGB_now->text();
        ui->lineEdit_RGB_marked->setText(strRgb);
    }
}

void Widget::slot_timeout()
{
    QColor color = getColor();
    QString strStyleSheet;
    strStyleSheet = "background-color: rgb(" + QString::number(color.red()) + "," +
            QString::number(color.green()) + "," +
            QString::number(color.blue()) + ");";
    ui->label_color->setStyleSheet(strStyleSheet);

    QString strRgb = QString::number(color.red()) + "," +
            QString::number(color.green()) + "," +
            QString::number(color.blue());
    ui->lineEdit_RGB_now->setText(strRgb);
}
