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
#include "CollidersManager.h"

class ShadowsManager;

/** @brief Klasa odpowiedzialana za wczytanie zewnetrznego pliku z ukladem sceny i odtworzeniem tej sceny.
 */
class MapLoader
{
public:

public:
    /** @brief Wczytuje uklad GameObject'ow z pliku i odtwarza go w SceneNode.
     * @param path - Sciezka do pliku z danymi o ukladzie sceny.
     * @param root - Wskazanie do SceneNode'a, w ktorym ma zostac odtworzona struktura z pliku.
     * @return true - Poprawnie wczytano uklad SceneNodow.
     * @return false - Blad podczas wczytywania SceneNodow.
     */
    static bool Load(std::string path, SceneNode* root, std::shared_ptr<cmp::Shader> shader, CollidersManager* collisionManager, ShadowsManager* shadowsManager);
};

#endif // __MAP_LOADER_H__