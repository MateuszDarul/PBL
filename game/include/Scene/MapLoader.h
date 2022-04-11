#ifndef __MAP_LOADER_H__
#define __MAP_LOADER_H__

#include <string>
#include <memory>
#include <fstream>
#include <inttypes.h>

#include "GameApplication.h"
#include "ResourceManager.h"
#include "SceneNode.h"
#include "Components.h"

class MapLoader
{
public:

public:
    static bool Load(std::string path, SceneNode* root);
};

#endif // __MAP_LOADER_H__