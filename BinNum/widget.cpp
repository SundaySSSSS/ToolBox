#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_pBtnNum2Bin = ui->pushButton_Down;
    m_pBtnBin2Num = ui->pushButton_Up;
    m_pCbbNumType = ui->comboBox_NumType;
    m_pLineEditBinHex = ui->lineEdit_Bin_Hex;
    m_pLineEditNumber = ui->lineEdit_Number;
    m_pCbbNumType->addItem("int8", NT_INT8);
    m_pCbbNumType->addItem("uint8", NT_UINT8);
    m_pCbbNumType->addItem("int16", NT_INT16);
    m_pCbbNumType->addItem("uint16", NT_UINT16);
    m_pCbbNumType->addItem("int32", NT_INT32);
    m_pCbbNumType->addItem("uint32", NT_UINT32);
    m_pCbbNumType->addItem("int64", NT_INT64);
    m_pCbbNumType->addItem("uint64", NT_UINT64);
    m_pCbbNumType->addItem("float", NT_FLOAT);
    m_pCbbNumType->addItem("double", NT_DOUBLE);

    ui->radioButton_bin->setChecked(true);
    ui->radioButton_LittleEndian->setChecked(true);

    connect(m_pBtnNum2Bin, SIGNAL(clicked()), this, SLOT(num2BinSlot()));
    connect(m_pBtnBin2Num, SIGNAL(clicked()), this, SLOT(bin2NumSlot()));
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::changeNumStr2BinHexStr(QString numStr, ShowType showType, Widget::NumType numType)
{
    QByteArray data;
    switch (numType)
    {
    case NT_INT8:
    {
        bool ok = false;
        int num = numStr.toInt(&ok);
        if (num > 127 || num < -128)
        {   //超出了char的范围
            ok = false;
        }
        if (ok)
        {
            char c = num;
            data = QByteArray(&c, 1);
        }
    }
        break;
    case NT_UINT8:
    {
        bool ok = false;
        int num = numStr.toInt(&ok);
        if (num < 0 || num > 255)
        {   //超出了char的范围
            ok = false;
        }
        if (ok)
        {
            unsigned char c = num;
            data = QByteArray((char*)&c, 1);
        }
    }
        break;
    case NT_INT16:
    {
        bool ok = false;
        short num = numStr.toShort(&ok);
        if (ok)
        {
            data = QByteArray((char*)&num, 2);
        }
    }
        break;
    case NT_UINT16:
    {
        bool ok = false;
        unsigned short num = numStr.toUShort(&ok);
        if (ok)
        {
            data = QByteArray((char*)&num, 2);
        }
    }
        break;
    case NT_INT32:
    {
        bool ok = false;
        int num = numStr.toInt(&ok);
        if (ok)
        {
            data = QByteArray((char*)&num, 4);
        }
    }
        break;
    case NT_UINT32:
    {
        bool ok = false;
        unsigned int num = numStr.toUInt(&ok);
        if (ok)
        {
            data = QByteArray((char*)&num, 4);
        }
    }
        break;
    case NT_INT64:
    {
        bool ok = false;
        long long num = numStr.toLongLong(&ok);
        if (ok)
        {
            data = QByteArray((char*)&num, 8);
        }
    }
        break;
    case NT_UINT64:
    {
        bool ok = false;
        unsigned long long num = numStr.toULongLong(&ok);
        if (ok)
        {
            data = QByteArray((char*)&num, 8);
        }
    }
        break;
    case NT_FLOAT:
    {
        bool ok = false;
        float num = numStr.toFloat(&ok);
        if (ok)
        {
            data = QByteArray((char*)&num, 4);
        }
    }
        break;
    case NT_DOUBLE:
    {
        bool ok = false;
        double num = numStr.toDouble(&ok);
        if (ok)
        {
            data = QByteArray((char*)&num, 8);
        }
    }
    default:
        break;
    }
    if (data == "")
    {
        QMessageBox::information(this, "Information",
                                 "Can not change the number to bin/hex");
        return "";
    }

    QString ret = changeByteArray2BinHexStr(data, showType);
    return ret;
}

QString Widget::changeByteArray2BinHexStr(QByteArray data, Widget::ShowType showType)
{
    QString ret;
    int size = data.size();
    for (int i = 0; i < size; ++i)
    {
        unsigned char c = data[i];
        if (showType == BIN)
        {
            ret += changeChar2BinStr(c);
        }
        else
        {
            QString temp;
            temp.sprintf("%02x", c);
            ret += temp;
        }
        ret += " ";
    }
    return ret;
}

QString Widget::changeChar2BinStr(unsigned char c)
{
    QString ret;
    unsigned char walker = 0x80;
    for (int i = 0; i < 8; ++i)
    {
        if ((walker & c) != 0)
        {
            ret += "1";
        }
        else
        {
            ret += "0";
        }
        walker = walker >> 1;
    }
    return ret;
}

QString Widget::changeBinHex2NumStr(QString binHexStr, Widget::ShowType showType, Widget::NumType numType)
{
    //去掉空格, 逗号
    binHexStr = binHexStr.replace(QString(" "), QString(""));
    binHexStr = binHexStr.replace(QString(","), QString(""));

    QByteArray data;
    if (showType == BIN)
    {
        if (binHexStr.size() % 8 != 0)
        {   //格式化后不正常, 二进制字符的长度必须能被8整除
            QMessageBox::information(this, "Information",
                                     "Can not change the bin to number");
            return "";
        }
        for (int i = 0; i < binHexStr.size() / 8; ++i)
        {
            //读取8个字节的数据
            char c = 0;
            c = changeSingleBinStr2Num(binHexStr.mid(i * 8, 8));
            data.append(c);
        }
    }
    else
    {
        if (binHexStr.size() % 2 != 0)
        {   //格式化后不正常, 十六进制字符的长度必须能被2整除
            QMessageBox::information(this, "Information",
                                     "Can not change the hex to number");
            return "";
        }
        for (int i = 0; i < binHexStr.size() / 2; ++i)
        {
            //读取2个字节的数据
            char c = 0;
            c = changeSingleHexStr2Num(binHexStr.mid(i * 2, 2));
            data.append(c);
        }
    }
    QString ret;
    switch (numType)
    {
    case NT_INT8:
    {
        char num = 0;
        if (data.size() == 1)
        {
            memcpy(&num, data.data(), 1);
            ret.sprintf("%d", num);
        }
    }
        break;
    case NT_UINT8:
    {
        unsigned char num = 0;
        if (data.size() == 1)
        {
            memcpy(&num, data.data(), 1);
            ret.sprintf("%d", num);
        }
    }
        break;
    case NT_INT16:
    {
        short num = 0;
        if (data.size() == 2)
        {
            memcpy(&num, data.data(), 2);
            ret.sprintf("%d", num);
        }
    }
        break;
    case NT_UINT16:
    {
        unsigned short num = 0;
        if (data.size() == 2)
        {
            memcpy(&num, data.data(), 2);
            ret.sprintf("%d", num);
        }
    }
        break;
    case NT_INT32:
    {
        int num = 0;
        if (data.size() == 4)
        {
            memcpy(&num, data.data(), 4);
            ret.sprintf("%d", num);
        }
    }
        break;
    case NT_UINT32:
    {
        unsigned int num = 0;
        if (data.size() == 4)
        {
            memcpy(&num, data.data(), 4);
            ret.sprintf("%d", num);
        }
    }
        break;
    case NT_INT64:
    {
        long long num = 0;
        if (data.size() == 8)
        {
            memcpy(&num, data.data(), 8);
            ret.sprintf("%lld", num);
        }
    }
        break;
    case NT_UINT64:
    {
        unsigned long long num = 0;
        if (data.size() == 8)
        {
            memcpy(&num, data.data(), 8);
            ret.sprintf("%lld", num);
        }
    }
        break;
    case NT_FLOAT:
    {
        float num = 0;
        if (data.size() == 4)
        {
            memcpy(&num, data.data(), 4);
            ret.sprintf("%f", num);
        }
    }
        break;
    case NT_DOUBLE:
    {
        double num = 0;
        if (data.size() == 8)
        {
            memcpy(&num, data.data(), 8);
            ret.sprintf("%f", num);
        }
    }
    default:
        break;
    }
    return ret;
}

char Widget::changeSingleBinStr2Num(QString singleBinStr)
{
    if (singleBinStr.length() != 8)
    {
        qDebug() << "Error!!!";
        return 0;
    }
    char ret = 0;
    for (int i = 0; i < 8; ++i)
    {
        if (singleBinStr[i] == '1')
        {
            ret |= (0x80 >> i);
        }
    }
    return ret;
}

char Widget::changeSingleHexStr2Num(QString singleHexStr)
{
    if (singleHexStr.length() != 2)
    {
        qDebug() << "Error!!!";
        return 0;
    }
    char ret = 0;
    //记录数字的低位和高位
    int lowNum = 0;
    int highNum = 0;
    for (int i = 0; i < 2; ++i)
    {
        char c = singleHexStr.at(i).toLatin1();
        int num = 0;
        switch (c)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            num = c - 48;
            break;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            num = (c - 97) + 10;
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            num = (c - 65) + 10;
            break;
        default:
            qDebug() << "invalid char";
            break;
        }
        if (i == 0)
        {
            highNum = num;
        }
        else
        {
            lowNum = num;
        }
    }
    ret = highNum * 16 + lowNum;
    return ret;
}

QString Widget::hexStr2BinStr(QString hexStr)
{
    hexStr = hexStr.replace(QString(" "), QString(""));
    hexStr = hexStr.replace(QString(","), QString(""));
    if (hexStr.size() % 2 != 0)
    {
        return "";
    }
    QString binStr;
    for (int i = 0; i < hexStr.size() / 2; ++i)
    {
        QString singleBinStr = hexStr.mid(i * 2, 2);
        bool ok = false;
        int hexNum = singleBinStr.toInt(&ok, 16);
        if (ok)
        {
            unsigned char hexChar = hexNum;
            binStr += changeChar2BinStr(hexChar);
            binStr += " ";
        }
        else
        {
            return "";
        }
    }
    return binStr;
}

QString Widget::changeNum2HexStr(unsigned char num)
{
    QString ret;
    ret.sprintf("%02x", num);
    return ret;
}

QString Widget::binStr2HexStr(QString binStr)
{
    QString hexStr;
    if (binStr.size() % 8 != 0)
    {
        return "";
    }
    for (int i = 0; i < binStr.size() / 8; ++i)
    {
        QString singleBinStr = binStr.mid(i * 8, 8);
        unsigned char num = changeSingleBinStr2Num(singleBinStr);
        hexStr += changeNum2HexStr(num);
        hexStr += " ";
    }
    return hexStr;
}

void Widget::num2BinSlot()
{
    ShowType showType = BIN;
    if (ui->radioButton_Hex->isChecked())
    {
        showType = HEX;
    }
    NumType numType = (NumType)m_pCbbNumType->currentData().toInt();
    QString binHexStr = changeNumStr2BinHexStr(m_pLineEditNumber->text(),
                                            showType, numType);
    if (binHexStr != "")
    {
        m_pLineEditBinHex->setText(binHexStr);
    }
}

void Widget::bin2NumSlot()
{
    ShowType showType = BIN;
    if (ui->radioButton_Hex->isChecked())
    {
        showType = HEX;
    }
    NumType numType = (NumType)m_pCbbNumType->currentData().toInt();
    QString binHexStr = changeBinHex2NumStr(m_pLineEditBinHex->text(),
                                            showType, numType);
    if (binHexStr != "")
    {
        m_pLineEditNumber->setText(binHexStr);
    }
}

void Widget::on_radioButton_bin_toggled(bool checked)
{
    if (!checked)
    {
        return;
    }
    qDebug() << "bin toggled";
    QString content = m_pLineEditBinHex->text();
    content.replace(" ", "");
    content.replace(",", "");
    if (content != "")
    {
        QString binStr = hexStr2BinStr(content);
        m_pLineEditBinHex->setText(binStr);
    }
}

void Widget::on_radioButton_Hex_toggled(bool checked)
{
    if (!checked)
    {
        return;
    }
    qDebug() << "hex toggled";
    QString content = m_pLineEditBinHex->text();
    content.replace(" ", "");
    content.replace(",", "");
    if (content != "")
    {
        QString hexStr = binStr2HexStr(content);
        m_pLineEditBinHex->setText(hexStr);
    }
}
