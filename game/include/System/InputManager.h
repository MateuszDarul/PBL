#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <GLFW/glfw3.h>

#include "MouseInputManager.h"
#include "KeyboardInputManager.h"

/** @brief Klasa laczaca input managerow.
 */
class InputManager
{
private:
    MouseInputManager* mouseInputManager; //!< Input manager dla myszy.
    KeyboardInputManager* keyboardInputManager; //!< Input manager dla klawiatury.

public:
    /** @brief Tworzy input managerow.
     * @param window -  Okno, dla ktorego tworzymy input managerow.
     */
    InputManager(GLFWwindow *window);
    
    /** @brief Niszczy input managerow.
     */
    ~InputManager();

    /** @brief Daje dostep do managera myszy.
     * @return MouseInputManager* - Wskazanie na managera myszy.
     */
    MouseInputManager* Mouse();

    /** @brief Daje dostep do managera klawiatury.
     * @return MouseInputManager* - Wskazanie na managera klawiatury.
     */
    KeyboardInputManager* Keyboard();
};

#endif // __INPUT_MANAGER_H__