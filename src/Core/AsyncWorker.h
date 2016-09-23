#ifndef ASYNCWORKER_H
#define ASYNCWORKER_H

#include <QObject>

class AsyncWorker : public QObject
{
    Q_OBJECT

public:
    explicit AsyncWorker(QObject* parent = Q_NULLPTR);
    ~AsyncWorker();

    bool isFinished() const { return done; }

private:
    bool done = false;

public slots:
    virtual void process();

signals:
    void finished();
    void error(const QString& err);
};

#endif // ASYNCWORKER_H
