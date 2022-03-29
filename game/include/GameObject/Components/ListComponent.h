#ifndef __LIST_COMPONENT_H__
#define __LIST_COMPONENT_H__

#include "Component.h"

/** @brief Komponent sluzacy do przechowywania wartosci liczbowych.
 */
class ListComponent :public Component
{
private:
    std::vector<double> list; //!< Lista wartosci liczbowych.

public:
    /** @brief Domyslny konstruktor.
     */
    ListComponent();

    /** @brief Zwraca wybrana wartosc.
     * @param index - Ktora wartosc chcemy odczytac.
     * @return double - Wartosc.
     */
    double Get(uint32_t index);

    /** @brief Usuwa wybrana wartosc.
     * @param index - Ktora wartosc chcemy usunac.
     * @return true - Usunieto wartosc.
     * @return false - Podano bledny argument.
     */
    bool Remove(uint32_t index);
    
    /** @brief Dodaje nowa wartosc.
     * @param v - Nowa wartosc.
     */
    void Add(double v);
};

#endif // __LIST_COMPONENT_H__