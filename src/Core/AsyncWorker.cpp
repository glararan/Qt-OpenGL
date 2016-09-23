#include "AsyncWorker.h"

AsyncWorker::AsyncWorker(QObject* parent)
: QObject(parent)
{
    connect(this, &AsyncWorker::finished, this, [&]() -> void
    {
        done = true;
    });
}

AsyncWorker::~AsyncWorker()
{

}

void AsyncWorker::process()
{
    emit finished();
}
