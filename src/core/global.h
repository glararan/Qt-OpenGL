#ifndef GLOBAL_H
#define GLOBAL_H

#include "GLManager.h"
#include "Singleton.h"
#include "ShaderManager.h"

#define GLMgr     Singleton<GLManager>::instancePtr()
#define ShaderMgr Singleton<ShaderManager>::instancePtr()

#endif // GLOBAL_H
