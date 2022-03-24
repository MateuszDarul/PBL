#include "KeyboardInputManager.h"

KeyboardInputManager::KeyboardInputManager(GLFWwindow *window)
    :window(window)
{
    keys.resize(350);
}

KeyboardInputManager::~KeyboardInputManager()
{
    keys.clear();
}

bool KeyboardInputManager::IsPressed(KeyboardKey key)
{
    if(glfwGetKey(this->window, key) == GLFW_PRESS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool KeyboardInputManager::OnPressed(KeyboardKey key)
{
    if(glfwGetKey(this->window, key) == GLFW_PRESS)
    {
        this->keys[key].x = true;
    }
    else if(glfwGetKey(window, key) == GLFW_RELEASE)
    {
        this->keys[key].x = false;
    }

    if(this->keys[key].x != this->keys[key].y)
    {
        this->keys[key].y = !this->keys[key].y;

        if (this->keys[key].x)
        {
            return true;
        }
    }
    return false;
}

bool KeyboardInputManager::IsReleased(KeyboardKey key)
{
    if(glfwGetKey(this->window, key) == GLFW_RELEASE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool KeyboardInputManager::OnReleased(KeyboardKey key)
{
    if(glfwGetKey(this->window, key) == GLFW_PRESS)
    {
        this->keys[key].x = true;
        this->keys[key].y = true;
    }
    else if(glfwGetKey(window, key) == GLFW_RELEASE)
    {
        this->keys[key].x = false;
    }

    if(!this->keys[key].x && this->keys[key].y)
    {
        this->keys[key].y = false;
        return true;
    }
    return false;
}