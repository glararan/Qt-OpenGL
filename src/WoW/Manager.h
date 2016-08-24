#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QDir>

namespace WoW
{
    class Manager : public QObject
    {
        Q_OBJECT

    public:
        explicit Manager(QObject* parent = Q_NULLPTR);

        const QString& getGamePath();
        const QString& getGameLocale();
        const QDir&    getGameDir();

    private:
        QString gamePath;
        QString gameLocale;

        QDir gameDir;

    signals:

    public slots:
    };
}

#endif // MANAGER_H
