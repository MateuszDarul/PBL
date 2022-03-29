#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <string>
#include <vector>

#include "Mesh.h"
#include "Material.h"

/** @brief Klasa zarzadzajaca zasobami w silniku.
 */
class ResourceManager
{
private:
    std::vector<std::pair<size_t, Mesh*>> loadedMesh; //!< Lista wczytanych siatek i ich kluczy.
    std::vector<std::pair<size_t, Material*>> loadedMaterial; //!< Lista wczytanych materialow i ich kluczy.
    //std::vector<ResourceNode<Texture>> loadedTexture;

public:
    /** @brief Domyslny konstruktor.
     */
    ResourceManager();

    /** @brief Destruktor, ktory zwalnia pamiec i usuwa wszystkie wczytane zasoby.
     */
    ~ResourceManager();

    /** @brief Zwraca wskaznik do siatki o podanej sciezce, lub tworzy nowy zasob, jesli jeszcze nie istnial.
     * @param path - Sciezka do siatki (pliku .obj).
     * @return Mesh* - Wskazanie na obiekt siatki.
     * @return nullptr - Jesli sciezka byla bledna.
     */
    Mesh* GetMesh(const std::string& path);

    /** @brief Usuwa siatke o podanej sciezce i zwalnia pamiec.
     * @param path - Sciezka do zasobu, ktory chcemy usunac z managera (plik .obj).
     * @return true - Pomyslnie usunieto siatke i zwolniono pamiec.
     * @return false - Nie udalo sie usunac siatki i zwolnic pamieci.
     */
    bool RemoveMesh(const std::string& path);

    /** @brief Zwraca wskaznik do materialu o podanej sciezce, lub tworzy nowy zasob, jesli jeszcze nie istnial.
     * @param path - Sciezka do materialu (pliku .mtl).
     * @return Mesh* - Wskazanie na obiekt materialu.
     * @return nullptr - Jesli sciezka byla bledna.
     */
    Material* GetMaterial(const std::string& path);

    /** @brief Usuwa material o podanej sciezce i zwalnia pamiec.
     * @param path - Sciezka do zasobu, ktory chcemy usunac z managera (plik .obj).
     * @return true - Pomyslnie usunieto material i zwolniono pamiec.
     * @return false - Nie udalo sie usunac materialu i zwolnic pamieci.
     */
    bool RemoveMaterial(const std::string& path);
};

#endif // __RESOURCE_MANAGER_H__