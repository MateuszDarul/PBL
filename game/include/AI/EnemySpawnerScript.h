#ifndef __ENEMY_SPAWNER_SCRIPT_H__
#define __ENEMY_SPAWNER_SCRIPT_H__

#include "ScriptComponent.h"
#include "CollidersManager.h"
#include "Scene.h"
#include "Path.h"

/** @brief Skrypt trzymadła na ścieżki natarcia
 */
class EnemySpawnerScript : public Script
{
private:

    std::vector<glm::vec3> wayPoints;

    CollidersManager* collidersManager;
    std::shared_ptr<ShaderComponent> displShader;
    std::shared_ptr<ShaderComponent> particleShader;
    Scene* scene;
    MultiToolController* multiToolScript;
    std::shared_ptr<GameObject> playerGO;
    int enemyEnumerator = 0;
    SceneNode* root;

public:

    /**
    * @brief Konstruktor
    *
    */
    EnemySpawnerScript(
        SceneNode* rootNode,
        std::shared_ptr<ShaderComponent> shader,
        std::shared_ptr<ShaderComponent> particleShader,
        CollidersManager* colManager, 
        Scene* scn, 
        MultiToolController* mtoolScript, 
        std::shared_ptr<GameObject> p
    ) 
    {
        collidersManager = colManager;
        displShader = shader;
        this->particleShader = particleShader;
        scene = scn;
        multiToolScript = mtoolScript;
        playerGO = p;
        root = rootNode;
    }

    /**
    * @brief Destruktor
    *
    */
    ~EnemySpawnerScript() {

    }

    /**
     * @brief Metoda wykonywana raz przy tworzeniu instancji skryptu
     *
     */
    void Start();

    /**
     * @brief Metoda wykonywana raz na klatke
     *
     * @param dt - krok czasu
     */
    void Update(float dt);

    //adds a waypoint to the end of the path
    void AddWayPoint(glm::vec3 new_point)
    {
        wayPoints.push_back(new_point);
    }

    std::vector<glm::vec3> GetWayPoints()
    {
        return wayPoints;
    }

    void SpawnEnemy(int nr);
};

#endif