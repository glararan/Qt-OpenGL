#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>

class SettingsManager : public QSettings
{
public:
    explicit SettingsManager(QObject* parent = Q_NULLPTR);

    void setValue(const QString& key, const QVariant& value, const bool& sync = true);
};

#endif // SETTINGSMANAGER_H
