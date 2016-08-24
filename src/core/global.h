#ifndef GLOBAL_H
#define GLOBAL_H

#include "GLManager.h"
#include "SettingsManager.h"
#include "Singleton.h"
#include "ShaderManager.h"

#include <WoW/MPQ/ArchiveManager.h>

#define GLMgr       Singleton<GLManager>::instancePtr()
#define ShaderMgr   Singleton<ShaderManager>::instancePtr()
#define ArchiveMgr  Singleton<WoW::ArchiveManager>::instancePtr()
#define SettingsMgr Singleton<SettingsManager>::instancePtr()

#endif // GLOBAL_H
