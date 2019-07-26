#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

    typedef enum _NumType
    {
        NT_INT8 = 0,
        NT_UINT8,
        NT_INT16,
        NT_UINT16,
        NT_INT32,
        NT_UINT32,
        NT_INT64,
        NT_UINT64,
        NT_FLOAT,
        NT_DOUBLE,
    }
    NumType;

    typedef enum _ShowType
    {
        BIN,
        HEX,
    }
    ShowType;

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QPushButton* m_pBtnBin2Num;
    QPushButton* m_pBtnNum2Bin;
    QComboBox* m_pCbbNumType;
    QLineEdit* m_pLineEditBinHex;
    QLineEdit* m_pLineEditNumber;

private:
    Ui::Widget *ui;

    /* number -> bin/hex */
    //将数据转换为二进制或十六进制字符串
    QString changeNumStr2BinHexStr(QString numStr, ShowType showType,
                                NumType numType);

    //将二进制或十六进制字符串转换为数据
    QByteArray changeBinHexStr2NumStr(QString str, NumType numType);

    //将二进制数据转为字符串
    QString changeByteArray2BinHexStr(QByteArray data, ShowType showType);

    //将数字(存放于unsigned char)转换为二进制字符串
    QString changeChar2BinStr(unsigned char c);

    /* bin/hex -> number */
    //将二进制十六进制字符串转为数字字符串
    QString changeBinHex2NumStr(QString binHexStr, ShowType showType,
                                NumType numType);
    //将单个二进制字符转为数字(char)
    char changeSingleBinStr2Num(QString singleBinStr);

    //将两个字符(十六进制)转为数字(char)
    char changeSingleHexStr2Num(QString singleHexStr);


    /* 工具函数 */
    QString binStr2HexStr(QString binStr);
    QString hexStr2BinStr(QString hexStr);

    //将数字转为十六进制字符串 char 3 ---> string 0x03
    QString changeNum2HexStr(unsigned char num);

private slots:
    //将数字转为二进制/十六进制
    void num2BinSlot();
    //将二进制/十六进制转为数字
    void bin2NumSlot();
    void on_radioButton_bin_toggled(bool checked);
    void on_radioButton_Hex_toggled(bool checked);
};

#endif // WIDGET_H
