#ifndef __MOUSE_BUTTONS_H__
#define __MOUSE_BUTTONS_H__

/** @brief Obslugiwane przyciski myszy.
*/
enum MouseButton
{ 
    Unknown_MB = -1, //!< Niezdefiniowany przycisk.
    Button1_MB = 0, //!< Przycisk 0 (Lewy przycisk myszy).
    Button2_MB = 1, //!< Przycisk 1 (Prawy przycisk myszy).
    Button3_MB = 2, //!< Przycisk 2 (Srodkowy przycisk myszy / Scroll).
    Button4_MB = 3, //!< Przycisk 3.
    Button5_MB = 4, //!< Przycisk 4.
    Button6_MB = 5, //!< Przycisk 5.
    Button7_MB = 6, //!< Przycisk 6.
    Button8_MB = 7, //!< Przycisk 7.
    Left_MB = Button1_MB, //!< Lewy przycisk myszy.
    Right_MB = Button2_MB, //!< Prawy przycisk myszy.
    Middle_MB = Button3_MB //!< Srodkowy przycisk myszy.
};

#endif // __MOUSE_BUTTONS_H__