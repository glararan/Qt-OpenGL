#include "Manager.h"

#include <QSettings>

namespace WoW
{
    Manager::Manager(QObject* parent)
    : QObject(parent)
    {

    }

    const QString& Manager::getGamePath()
    {
        if(gamePath.isEmpty())
        {
#ifdef Q_OS_WIN
            // todo if settings has saved

            QStringList registryPaths;
            registryPaths << "HKEY_LOCAL_MACHINE\\SOFTWARE\\Blizzard Entertainment\\World of Warcraft"
                          << "HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Blizzard Entertainment\\World of Warcraft"
                          << "HKEY_CURRENT_USER\\Software\\Classes\\VirtualStore\\MACHINE\\SOFTWARE\\Wow6432Node\\Blizzard Entertainment\\World of Warcraft";

            for(const QString& registryPath : registryPaths)
            {
                QSettings registry(registryPath, QSettings::NativeFormat);

                if(!registry.value("InstallPath").toString().isEmpty())
                {
                    gamePath = registry.value("InstallPath").toString();

                    break;
                }
            }

            // todo set to settings

#elif Q_OS_MAC
            gamePath = "/Applications/World of Warcraft/";
#endif
        }

        return gamePath;
    }

    const QString& Manager::getGameLocale()
    {
        if(gameLocale.isEmpty())
        {
            // todo if settings has saved

            QStringList locales;
            locales << "enGB" << "enUS" << "deDE" << "koKR" << "frFR"
                    << "zhCN" << "zhTW" << "esES" << "esMX" << "ruRU";

            for(const QString& locale : locales)
            {
                if(getGameDir().exists("Data/" + locale))
                {
                    gameLocale = locale;

                    break;
                }
            }

            // todo set to settings
        }

        return gameLocale;
    }

    const QDir& Manager::getGameDir()
    {
        if(gameDir.absolutePath().isEmpty())
            gameDir = QDir(getGamePath());

        return gameDir;
    }
}
