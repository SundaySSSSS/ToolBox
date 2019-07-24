#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <QFileInfo>
#include <QDir>

//循环遍历指定文件夹下的目录，返回所有文件路径
QFileInfoList recursionFileInfoReader(QString path)
{
    QFileInfoList ret;
    QDir dir(path);
    QFileInfoList fileInfoList = dir.entryInfoList();

    for(int i = 0; i < fileInfoList.size(); ++i)
    {
        QFileInfo fileInfo = fileInfoList.at(i);
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
        {
            continue;
        }
        else
        {
            if (fileInfo.isDir())
            {
                ret += recursionFileInfoReader(fileInfo.filePath());
            }
            else
            {
                ret.append(fileInfo);
            }
        }
    }
    return ret;
}

#endif // FILEUTILS_H
