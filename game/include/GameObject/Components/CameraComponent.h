#ifndef __CAMERA_COMPONENT_H__
#define __CAMERA_COMPONENT_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"

#include "InputManager.h"
#include "Frustum.h"
class GameObject;

class CameraComponent :public Component
{
public:
    /** @brief Kierunki w ktorych mozna przemieszczac kamere.
     */
    enum Movement
    {
        UP = 0,         //!< Gora.
        DOWN = 1,       //!< Dol.
        LEFT = 2,       //!< Lewo.
        RIGHT = 3,      //!< Prawo.
        FORWARD = 4,    //!< Przod.
        BACKWARD = 5    //!< Tyl.
    };

    /** @brief Zwraca kierunek patrzenia kamery
     */
    glm::vec3 GetForward()
    {
        return front;
    }

    glm::vec3 GetRight()
    {
        return right;
    }

    glm::vec3 GetUp()
    {
        return up;
    }

    /** @brief Zwraca kąt obrotu kamery w osi Y
     */
    float GetYaw()
    {
        return yaw;

    }

    /** @brief Zwraca kąt obrotu kamery w osi X
     */
    float GetPitch()
    {
        return pitch;
    }

    void RestartMovement(float x, float y, float z, float pitch = 0.0f, float yaw = 0.0f);
    void RestartMovement(const glm::vec3& pos, const glm::vec2& rot);

private:
    glm::vec3 position; //!< Aktualna pozycja kamery.

    float verticalVelocity;
    float gravity;
    float jumpVelocity;
    float jumpHeight;
    float jumpTimeToPeak;
    bool isEnabledMovement;
    bool isEnabledRotation;
    bool isGrounded;
    bool isMoving;

    float shakeTimer;
    float shakeAmount;
    float shakeSpeed;
    float shakeSlowFactor;

    glm::mat4 view; //!< Macierz widoku kamery.

    bool needUpdate; //!< Wartosc informujaca kiedy nalezy obliczyc nowa macierz widoku.

    glm::vec3 up; //!< Wektor pokazujacy kierunek w gore.
    glm::vec3 right; //!< Wektor pokazujacy kierunek w prawo.
    glm::vec3 front; //!< Wektor pokazujacy kierunek w przod.

    float yaw; //!< Kat obrotu kamery (lewo/prawo).
    float pitch; //!< Kat obrotu kamery (gora/dol).

    float yawOffset;
    float pitchOffset;

    float mouseSensitivity; //!< Czulosc obrotu kamery przy pomocy myszy.
    float speedPerSec; //!< Predkosc poruszania sie kamery na sekunde przy pomocy klawiatury.

    bool firstMouseMovement; //!< Wartosc pomagajaca w konfiguracji kamery.
    glm::vec2 lastCursorPosition; //!< Poprzednia pozycja kamery, uzywana do pbliczania obrotu kamery.

private:
    /** @brief Aktualizuje wektory kierunkowe (gora, prawo, przod).
     */
    void UpdateVectors();

    void CalculateJumpParams();

public:
    /** @brief Konstruktor ustawiajacy parametry poczatkowe kamery.
     */
    CameraComponent();
    
    /** @brief Domyslny destruktor.
     */
    ~CameraComponent();

    /** @brief Tworzy nowa kamere.
     * @param position - Pozycja kamery w swiecie.
     * @return true - Stworzono kamere.
     * @return false - Blad podczas tworzenia kamery.
     */
    bool Create(const glm::vec3& position);

    /** @brief Poruszanie kamera w swiecie gry.
     * @param inputManager - Wskazanie na managera obslugujacego klawiature i mysz.
     * @param deltaTime - Jeden przez liczbe klatek na sekunde.
     */
    void Update(InputManager* inputManager, const float& deltaTime);

    /** @brief Pobiera pozycje kamery.
     * @return glm::vec3 - Pozycja kamery.
     */
    glm::vec3 GetPosition();

    /** @brief Ustawia nowa pozycje kamery.
     * @param position - Nowa pozycja kamery.
     */
    void SetPosition(const glm::vec3& position);

    /** @brief Pobiera czulosc myszy.
     * @return float - czulosc myszy.
     */
    float GetMouseSensitivity();

    /** @brief Ustawia nowa czulosc myszy.
     * @param mouseSensitivity - Nowa czulosc myszy.
     */
    void SetMouseSensitivity(const float& mouseSensitivity);

    /** @brief Zwraca warosc, o jaka moze przesunac sie kamera w ciagu jednej sekundy.
     * @return float - Predkosc na sekunde.
     */
    float GetSpeed();

    /** @brief Ustawia warosc, o jaka kamera moze sie przesunac w ciagu jednej sekundy.
     * @param speedPerSec - Nowa predkosc na sekunde.
     */
    void SetSpeed(const float& speedPerSec);

    
    float GetGravity();
    float GetJumpVelocity();
    
    float GetJumpHeight();
    void  SetJumpHeight(float newHeight);
    
    float GetJumpTimeToPeak();
    void  SetJumpTimeToPeak(float newTime);

    bool GetMovementEnabled();
    void SetMovementEnable(bool enabled);
    
    bool GetRotationEnabled();
    void SetRotationEnable(bool enabled);

    bool GetIsGrounded();
    void SetIsGrounded(bool grounded);

    bool IsMoving() const;

    float GetPitch() const;
    float GetYaw() const;
    void SetRotation(float pitch, float yaw);
    void SetRotationOffset(float pitch, float yaw);


    void ShakeCamera(float time, float amount, float speed, float slowFactor = 0.9f);
    float GetShakeTime() { return shakeTimer; }
    float GetShakeSpeed() { return shakeSpeed; }
    float GetShakeAmount() { return shakeAmount; }


    /** @brief Pozwala na przemieszczanie kamery w wybranym kierunku.
     * @param direction - Kierunek w krorym kamera zostanie przesunieta.
     * @param offset - Wartosc o jaka ma zostac przesunieta.
     * @param deltaTime - W jakim czasie ma zostac przesunieta o wybrana wartosc.
     */
    void Move(Movement direction, const float& offset, const float &deltaTime);

    /** @brief Pozwala na obrot kamery przy pomocy myszy.
     * @param offsetX - O jaka wartosc nalezy obrocic kamere w osi X.
     * @param offsetY - O jaka wartosc nalezy obrocic kamere w osi Y.
     */
    void ProcessMouseMovement(const float& offsetX, const float& offsetY);

    /** @brief Pozwala na obrot kamery przy pomocy myszy.
     * @param offset - O jaka wartosc nalezy obrocic kamere.
     */
    void ProcessMouseMovement(const glm::vec2& offset);

    /** @brief Zwraca macierz widoku kamery.
     * @return glm::mat4 - Macierz widoku.
     */
    const glm::mat4& GetView();

    Frustum GetFrustum(float aspect, float fov, float nearPlane, float farPlane);
};

#endif // __CAMERA_COMPONENT_H__