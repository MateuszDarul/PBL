#ifndef __MOUSE_BUTTONS_H__
#define __MOUSE_BUTTONS_H__

/** @brief Obslugiwane przyciski myszy.
*/
enum MouseButton
{ 
    Unknown = -1, //!< Niezdefiniowany przycisk.
    Button1 = 0, //!< Przycisk 0 (Lewy przycisk myszy).
    Button2 = 1, //!< Przycisk 1 (Prawy przycisk myszy).
    Button3 = 2, //!< Przycisk 2 (Srodkowy przycisk myszy / Scroll).
    Button4 = 3, //!< Przycisk 3.
    Button5 = 4, //!< Przycisk 4.
    Button6 = 5, //!< Przycisk 5.
    Button7 = 6, //!< Przycisk 6.
    Button8 = 7, //!< Przycisk 7.
    Left = Button1, //!< Lewy przycisk myszy.
    Right = Button2, //!< Prawy przycisk myszy.
    Middle = Button3 //!< Srodkowy przycisk myszy.
};

#endif // __MOUSE_BUTTONS_H__