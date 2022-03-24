#include "InputManager.h"

InputManager::InputManager(GLFWwindow *window)
{
    mouseInputManager = new MouseInputManager(window);
    keyboardInputManager = new KeyboardInputManager(window);
}

InputManager::~InputManager()
{
    delete mouseInputManager;
    delete keyboardInputManager;
}

MouseInputManager* InputManager::Mouse()
{
    return mouseInputManager;
}

KeyboardInputManager* InputManager::Keyboard()
{
    return keyboardInputManager;
}