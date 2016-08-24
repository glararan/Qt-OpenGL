#include "SettingsManager.h"

SettingsManager::SettingsManager(QObject* parent)
: QSettings(parent)
{
}

void SettingsManager::setValue(const QString& key, const QVariant& value, const bool& sync)
{
    QSettings::setValue(key, value);

    if(sync)
        QSettings::sync();
}
