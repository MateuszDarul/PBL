#ifndef __KEYBOARD_INPUT_MANAGER_H__
#define __KEYBOARD_INPUT_MANAGER_H__

#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "KeyboardKeys.h"

/** @brief Klasa dostarczająca prosty interfejs do obslugi klawiatury.
 */
class KeyboardInputManager
{
private:
    GLFWwindow* window; //!< Okno, z ktorego przyjmujemy zdarzenia.
    std::vector<glm::vec2> keys; //!< Aktualny stan przyciskow na klawiaturze.

public:
    /** @brief Tworzy nowego managera.
     * @param window - Okno, dla którego tworzymy managera.
     */
    KeyboardInputManager(GLFWwindow *window);

    /** @brief Domyslny destruktor.
     */
    ~KeyboardInputManager();

    /** @brief Sprawdza czy wybrany przycisk na klawiaturze jest wcisniety.
     * @param key - Wybrany przycisk
     * @return true - Przycisk jest wcisniety.
     * @return false - Przycisk nie jest wcisniety.
     */
    bool IsPressed(KeyboardKey key);

    /** @brief Sprawdza czy wybrany przycisk na klawiaturze zostal wcisniety.
     * @param key - Wybrany przycisk
     * @return true - Przycisk zostal wcisniety.
     * @return false - Przycisk nie zostal wcisniety.
     */
    bool OnPressed(KeyboardKey key);

    /** @brief Sprawdza czy wybrany przycisk na klawiaturze nie jest wcisniety.
     * @param key - Wybrany przycisk
     * @return true - Przycisk nie jest wcisniety.
     * @return false - Przycisk jest wcisniety.
     */
    bool IsReleased(KeyboardKey key);

    /** @brief Sprawdza czy wybrany przycisk na klawiaturze zostal puszczony.
     * @param key - Wybrany przycisk
     * @return true - Przycisk zostal puszczony.
     * @return false - Przycisk nie zostal puszczony.
     */
    bool OnReleased(KeyboardKey key);
};

#endif //__KEYBOARD_INPUT_MANAGER_H__