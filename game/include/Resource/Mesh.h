#ifndef __MESH_H__
#define __MESH_H__

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/** @brief Reprezentuje siatke modelu w silniku.
 */
class Mesh
{
public:
    /** @brief Reprezentacja jednego wierzcholka.
     */
    struct Vertex
    {
        glm::vec3 position; //!< Pozycja wierzcholka.
        glm::vec3 normal; //!< Wartosc wektora normalnego wierzcholka.
        glm::vec2 texCoords; //!< Punkt tekstury dla wierzcholka.
    };

public:
    std::vector<Vertex> vertices; //!< Lista wszystkich wierzcholkow siatki.

public:
    /** @brief Konstruktor siatki.
     */
    Mesh();

    /** @brief Destruktor siatki.
     */
    ~Mesh();

    /** @brief Wczytuje siatke modelu z pliku (.obj).
     * @param objPath - Sciezka do pliku z modelem.
     * @return true - Udalo sie wczytac siatke.
     * @return false - Wsytapil blad podczas wczytywania siatki.
     */
    bool Load(const std::string& objPath);
}; 

#endif // __MESH_H__