#ifndef GLOBAL_H
#define GLOBAL_H

#include "singleton.h"
#include "glmanager.h"

#define GLMgr Singleton<GLManager>::instancePtr()

#endif // GLOBAL_H
