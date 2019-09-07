#ifndef THREADTOOL_H
#define THREADTOOL_H

#include <QThread>

typedef void (*PTRFUN)(void*);

class ThreadTool : public QThread
{
public:
    ThreadTool(PTRFUN pFunction, void* pFuncParam, unsigned int uIntervalMs = 0, QObject* parent = nullptr)
        : QThread(parent),
        m_pFunc(pFunction),
        m_pFuncParam(pFuncParam),
        m_uIntervalMs(uIntervalMs),
        m_bRunning(false) { }

    virtual ~ThreadTool() {}

    void run()
    {
        m_bRunning = true;
        while (m_bRunning)
        {
            m_pFunc(m_pFuncParam);
            msleep(m_uIntervalMs);
        }
    }
    void stop() { m_bRunning = false; }

private:
    PTRFUN m_pFunc;     //回调函数指针
    void* m_pFuncParam; //回调函数参数
    unsigned int m_uIntervalMs;    //调用间隔
    bool m_bRunning;
};

#endif // THREADTOOL_H
