#ifndef __ENEMY_SCRIPT_H__
#define __ENEMY_SCRIPT_H__

#include "ScriptComponent.h"
#include "StateMachine.h"
#include "SceneNode.h"
#include "SteeringBehaviours.h"
#include "Path.h"
#include "Scripts/Health.h"
#include "Scripts/MultiToolController.h"

/** @brief Skrypt obiektu przeciwnika
 */
class EnemyScript : public Script
{

public:

    Health* health;
    MultiToolController* multitool;
    std::shared_ptr<StateMachine<GameObject>> stateMachine;

private:

    glm::vec3 velocity; //!< speed of an object.

    glm::vec3 heading; //!< a normalized vector pointing in the direction the entity is heading

    glm::vec3 side; //!< a side vector perpendicular to the heading vector

    glm::vec3 top; //!< a top vector perpendicular to the heading vector

    float maxSpeed; //!< the maximum speed at which this entity may travel.

    float maxForce; //!< the maximum force this entity can produce to power itself

    float maxTurnRate; //!< the maximum rate (radians per second) at which this vehicle can rotate

    std::shared_ptr<SceneNode> world; //!< a pointer to the world data

    SteeringBehaviours* steering; //!< a pointer to the steering behaviour class

    std::shared_ptr<Path> path; //!< a pointer to a path to follow

    std::shared_ptr<GameObject> player;

public:

    /**
    * @brief Konstruktor
    *
    */
    EnemyScript(std::shared_ptr<SceneNode> wrld, std::shared_ptr<GameObject> p) {

        world = wrld;
        player = p;

    }

    /**
    * @brief Destruktor
    *
    */
    ~EnemyScript() {
        delete steering;
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

    void SetPath(std::shared_ptr<Path> p)
    {
        path = p;
    }

    glm::vec3 GetVelocity() {
        return velocity;
    }

    void SetVelocity(glm::vec3 vel) {
        velocity = vel;
    }

    glm::vec3 GetHeading() {
        return heading;
    }

    void SetHeading(glm::vec3 head) {
        heading = head;
    }

    glm::vec3 GetSide() {
        return side;
    }

    void SetSide(glm::vec3 sid) {
        side = sid;
    }

    float GetMaxSpeed() {
        return maxSpeed;
    }

    void SetMaxSpeed(float max) {
        maxSpeed = max;
    }

    float GetMaxForce() {
        return maxForce;
    }

    void SetMaxForce(float max) {
        maxForce = max;
    }

    float GetMaxTurnRate() {
        return maxTurnRate;
    }

    void SetMaxTurnRate(float max) {
        maxTurnRate = max;
    }

    SteeringBehaviours* GetSteering() {
        return steering;
    }
};

#endif

