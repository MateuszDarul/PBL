#include "MouseInputManager.h"

glm::vec2 MouseInputManager::cursorPosition = {0,0};
void CursorPosition(GLFWwindow* window, double xpos, double ypos)
{
    MouseInputManager::cursorPosition.x = xpos;
    MouseInputManager::cursorPosition.y = ypos;
}

short int MouseInputManager::lastScrollOffset = 0;
void ScrollStatus(GLFWwindow* window, double xoffset, double yoffset)
{
    MouseInputManager::lastScrollOffset = yoffset;
}

MouseInputManager::MouseInputManager(GLFWwindow *window)
    :window(window)
{
    glfwSetCursorPosCallback(this->window, CursorPosition);
    glfwSetScrollCallback(this->window, ScrollStatus);

    buttons.resize(8);
}

MouseInputManager::~MouseInputManager()
{
    glfwSetCursorPosCallback(this->window, nullptr);
    glfwSetScrollCallback(this->window, nullptr);
}

bool MouseInputManager::IsPressed(MouseButton button)
{
    if(glfwGetMouseButton(this->window, button) == GLFW_PRESS)
    {
        return true;
    }
    return false;
}

bool MouseInputManager::OnPressed(MouseButton button)
{
    if(glfwGetMouseButton(this->window, button) == GLFW_PRESS)
    {
        this->buttons[button].x = true;
    }
    else if(glfwGetMouseButton(this->window, button) == GLFW_RELEASE)
    {
        this->buttons[button].x = false;
    }

    if(this->buttons[button].x != this->buttons[button].y)
    {
        this->buttons[button].y = !this->buttons[button].y;

        if(this->buttons[button].x)
        {
            return true;
        }
    }
    return false;
}

bool MouseInputManager::IsReleased(MouseButton button)
{
    if(glfwGetMouseButton(this->window, button) == GLFW_RELEASE)
    {
        return true;
    }
    return false;
}

bool MouseInputManager::OnReleased(MouseButton button)
{
    if(glfwGetMouseButton(this->window, button) == GLFW_PRESS)
    {
        this->buttons[button].x = true;
        this->buttons[button].y = true;
    }
    else if(glfwGetMouseButton(window, button) == GLFW_RELEASE)
    {
        this->buttons[button].x = false;
    }

    if(!this->buttons[button].x && this->buttons[button].y)
    {
        this->buttons[button].y = false;
        return true;
    }
    return false;
}

glm::vec2 MouseInputManager::GetPosition()
{
    return MouseInputManager::cursorPosition;
}

short int MouseInputManager::GetScrollOffset()
{
    short int result = MouseInputManager::lastScrollOffset;
    MouseInputManager::lastScrollOffset = 0;
    return result;
}