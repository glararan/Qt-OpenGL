#ifndef SINGLETON_H
#define SINGLETON_H

#include <QtGlobal>
#include <QAtomicInt>
#include <QThread>
#include <QThreadStorage>

enum ECallOnce
{
    CO_Request,
    CO_InProgress,
    CO_Finished
};

Q_GLOBAL_STATIC(QThreadStorage<QAtomicInt*>, once_flag)

template<class Function>
inline static void qCallOnce(Function func, QBasicAtomicInt& flag)
{
#if QT_VERSION < 0x050000
    int protectFlag = flag.fetchAndStoreAcquire(flag);
#elif QT_VERSION >= 0x050000
    int protectFlag = flag.fetchAndStoreAcquire(flag.load());
#endif

    if (protectFlag == CO_Finished)
        return;

    if (protectFlag == CO_Request && flag.testAndSetRelaxed(protectFlag, CO_InProgress))
    {
        func();
        flag.fetchAndStoreRelease(CO_Finished);
    }
    else
    {
        do
        {
            QThread::yieldCurrentThread();
        }
        while (!flag.testAndSetAcquire(CO_Finished, CO_Finished));
    }
}

template<class Function>
inline static void qCallOncePerThread(Function func)
{
    if (!once_flag()->hasLocalData())
    {
        once_flag()->setLocalData(new QAtomicInt(CO_Request));

        qCallOnce(func, *once_flag()->localData());
    }
}

template<class T>
class Singleton
{
public:
    static T& instance()
    {
        qCallOnce(init, flag);

        return *tptr;
    }

    static T* instancePtr()
    {
        qCallOnce(init, flag);

        return &(*tptr);
    }

    static void init()
    {
        tptr.reset(new T);
    }

private:
    Q_DISABLE_COPY(Singleton)

    Singleton()
    {
    }
    ~Singleton()
    {
    }

    static QScopedPointer<T> tptr;
    static QBasicAtomicInt flag;
};

template<class T> QScopedPointer<T> Singleton<T>::tptr(0);
template<class T> QBasicAtomicInt Singleton<T>::flag = Q_BASIC_ATOMIC_INITIALIZER(CO_Request);

#endif // SINGLETON_H
