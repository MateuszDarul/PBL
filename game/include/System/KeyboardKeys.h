#ifndef __KEYBOARD_KEYS_H__
#define __KEYBOARD_KEYS_H__

/** @brief Obslugiwane przyciski na klawiaturze.
 */
enum KeyboardKey
{
    Unknown = -1,      //!< Niezdefiniowany przycisk.
    Space = 32,        //!< Spacja.
    Apostrophe = 39,   //!< Apostrof (').
    Comma = 44,        //!< Przecinek (,).
    Minus = 45,        //!< Minus (-).
    Dot = 46,          //!< Kropka (.).
    Slash = 47,        //!< Slash (/).
    Nr0 = 48,          //!< Cyrfa 0 na klawiaturze qwert (0).
    Nr1 = 49,          //!< Cyrfa 1 na klawiaturze qwert (1).
    Nr2 = 50,          //!< Cyrfa 2 na klawiaturze qwert (2).
    Nr3 = 51,          //!< Cyrfa 3 na klawiaturze qwert (3).
    Nr4 = 52,          //!< Cyrfa 4 na klawiaturze qwert (4).
    Nr5 = 53,          //!< Cyrfa 5 na klawiaturze qwert (5).
    Nr6 = 54,          //!< Cyrfa 6 na klawiaturze qwert (6).
    Nr7 = 55,          //!< Cyrfa 7 na klawiaturze qwert (7).
    Nr8 = 56,          //!< Cyrfa 8 na klawiaturze qwert (8).
    Nr9 = 57,          //!< Cyrfa 9 na klawiaturze qwert (9).
    Semicolon = 59,    //!< Srednik (;).
    Equal = 61,        //!< Znak rownosci (=).
    A = 65,            //!< Litera A (A).
    B = 66,            //!< Litera B (B).
    C = 67,            //!< Litera C (C).
    D = 68,            //!< Litera D (D).
    E = 69,            //!< Litera E (E).
    F = 70,            //!< Litera F (F).
    G = 71,            //!< Litera G (G).
    H = 72,            //!< Litera H (H).
    I = 73,            //!< Litera I (I).
    J = 74,            //!< Litera J (J).
    K = 75,            //!< Litera K (K).
    L = 76,            //!< Litera L (L).
    M = 77,            //!< Litera M (M).
    N = 78,            //!< Litera N (N).
    O = 79,            //!< Litera O (O).
    P = 80,            //!< Litera p (p).
    Q = 81,            //!< Litera Q (Q).
    R = 82,            //!< Litera R (R).
    S = 83,            //!< Litera S (S).
    T = 84,            //!< Litera T (T).
    U = 85,            //!< Litera U (U).
    V = 86,            //!< Litera V (V).
    W = 87,            //!< Litera W (W).
    X = 88,            //!< Litera X (X).
    Y = 89,            //!< Litera Y (Y).
    Z = 90,            //!< Litera Z (Z).
    LBracket = 91,     //!< Lewy nawias kwadratowy ([).
    Backslash = 92,    //!< Bakclash (\).
    RBracket = 93,     //!< Prawy nawias kwadratowy (]).
    Tilde = 96,        //!< Tylda (~).
    Escape_KB = 256,   //!< Escape (Esc).
    Enter = 257,       //!< Enter (Enter).
    Tab = 258,         //!< Tabulator (Tab).
    Backspace = 259,   //!< Backspace (Backspace).
    Insert = 260,      //!< Insert (Insert).
    Delete = 261,      //!< Delete (Delete).
    Right = 262,       //!< Prawa strzalka.
    Left = 263,        //!< Lewa strzalka.
    Down = 264,        //!< Strzalka w dol.
    Up = 265,          //!< Strzalka w gore.
    PgUp = 266,        //!< Page up (Page Up).
    PgDown = 267,      //!< Page down (Page Down).
    Home = 268,        //!< Home (Home).
    End = 269,         //!< End (End).
    CapsLock = 280,    //!< Caps lock (Caps Lock).
    ScrollLock = 281,  //!< Scroll lock (Scroll Lock).
    NumLock = 282,     //!< Num lock (Num Lock).
    PrintScreen = 283, //!< Print screen (Print Screen).
    Pause = 284,       //!< Pause (Pause).
    F1 = 290,          //!< F1 (F1).
    F2 = 291,          //!< F2 (F2).
    F3 = 292,          //!< F3 (F3).
    F4 = 293,          //!< F4 (F4).
    F5 = 294,          //!< F5 (F5).
    F6 = 295,          //!< F6 (F6).
    F7 = 296,          //!< F7 (F7).
    F8 = 297,          //!< F8 (F8).
    F9 = 298,          //!< F9 (F9).
    F10 = 299,         //!< F10 (F10).
    F11 = 300,         //!< F11 (F11).
    F12 = 301,         //!< F12 (F12).
    NP0 = 320,         //!< Numpad 0 (0).
    NP1 = 321,         //!< Numpad 1 (1).
    NP2 = 322,         //!< Numpad 2 (2).
    NP3 = 323,         //!< Numpad 3 (3).
    NP4 = 324,         //!< Numpad 4 (4).
    NP5 = 325,         //!< Numpad 5 (5).
    NP6 = 326,         //!< Numpad 6 (6).
    NP7 = 327,         //!< Numpad 7 (7).
    NP8 = 328,         //!< Numpad 8 (8).
    NP9 = 329,         //!< Numpad 9 (9).
    NPDel = 330,       //!< Numpad delete (Del).
    NPDivide = 331,    //!< Numpad slash (/).
    NPMultiplu = 332,  //!< Numpad multiply (*).
    NPSubtract = 333,  //!< Numpad minus (-).
    NPAdd = 334,       //!< Numpad plus (+).
    NPEnter = 335,     //!< Numpad enter (Enter).
    NPEqual = 336,     //!< Numpad znak rownosci (=).
    LShift = 340,      //!< Lewy shifh (Shift).
    LControl = 341,    //!< Lewy control (Ctrl).
    LAlt = 342,        //!< Lewy alt (Alt).
    LSuper = 343,      //!< Lewy super/Command/Windows key (Win logo).
    RShift = 344,      //!< Prawy shift (Shift).
    RControl = 345,    //!< Prawy control (Ctrl).
    RAlt = 346,        //!< Prawy alt (Alt).
    RSuper = 347,      //!< Prawy super/Command/Windows key (Win logo).
    Menu = 348         //!< Menu (Menu).
};

#endif // __KEYBOARD_KEYS_H__