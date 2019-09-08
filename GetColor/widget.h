#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private:
    Ui::Widget *ui;
    QTimer m_timer;
    //获取屏幕中, 鼠标位置的颜色
    QColor getColor();
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void slot_timeout();
};

#endif // WIDGET_H
