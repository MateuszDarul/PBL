#ifndef __MOUSE_INPUT_MANAGER_H__
#define __MOUSE_INPUT_MANAGER_H__

#include <vector>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "MouseButtons.h"

/** @brief Klasa dostarczająca prosty interfejs do obslugi myszy.
 */
class MouseInputManager
{
private:
    GLFWwindow *window; //<! Okno, z ktorego przyjmujemy zdarzenia.
    std::vector<glm::vec2> buttons; //!< Aktualny stan przyciskow myszy.

public:
    static glm::vec2 cursorPosition; //<! Aktualna pozycja kursora (do pobierania jej wykorzystuj getPosition())
    static short int lastScrollOffset; //<! Wartosc ostatniego obrotu scrolla (do pobierania jej wykorzystuj getScrollOffset())

public:
    /** @brief Tworzy nowego managera.
     * @param window - Okno, dla którego tworzymy managera.
     */
    MouseInputManager(GLFWwindow *window);

    /** @brief Domyślny destruktor.
     */
    ~MouseInputManager();
    
    /** @brief Sprawdza czy wybrany przycisk myszy jest wcisniety.
     * @param button - Wybrany przycisk.
     * @return true - Przycisk jest wcisniety.
     * @return false - Przycisk nie jest wcisniety.
     */
    bool IsPressed(MouseButton button);
    
    /** @brief Sprawdza czy wybrany przycisk myszy nie jest wcisniety.
     * @param button - Wybrany przycisk.
     * @return true - Przycisk nie jest wcisniety.
     * @return false - Przycisk jest wcisniety.
     */
    bool IsReleased(MouseButton button);

    /** @brief Zwraca wartosc w momencie zmiany statusu przycisku.
     * @param button - Wybrany przycisk.
     * @return true - Przycisk zostal wcisniety.
     * @return false - Przycisk nie zmienil swojego statusu.
     */
    bool OnPressed(MouseButton button);

    /** @brief Zwraca wartosc w momencie zmiany statusu przycisku.
     * @param button - Wybrany przycisk.
     * @return true - Przycisk zostal puszczony.
     * @return false - Przycisk nie zmienil swojego statusu.
     */
    bool OnReleased(MouseButton button);

    /**  @brief Pobiera aktualna pozycje kursora na oknie.
     * @return glm::vec2 - Pozycja kursora.
     */
    glm::vec2 GetPosition();

    /** @brief Pobiera wartosc obrotu scrolla.
     * @return short int - wartosc obrotu scrolla (wartosc -x oznacza obrot scrola do siebie a wartosc +x od siebie).
     */
    short int GetScrollOffset();
};

#endif // __MOUSE_INPUT_MANAGER_H__



/** EXAMPLES

1. Wykorzystanie scrolla
{
    short int offset;
    if(offset = mouseManager.GetScrollOffset())
    {
        std::cout << offset << std::endl;
    }
}

*/