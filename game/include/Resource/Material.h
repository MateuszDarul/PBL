#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <iostream>
#include <fstream>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>

class Material
{
public:
    unsigned int diffuse_texture_id; //!< Tesktura diffuse.
    unsigned int specular_texture_id; //!< Tesktura specular.
    unsigned int normal_texture_id; //!< Mapa normalnych.

private:
    /** @brief Wczytuje teksture z pliku i tworzy dla niej buffor.
     * @param id - Gdzie chcemy zapisac nowo utworzona teksture.
     * @param path - Sciezka do pliku z tekstura.
     */
    void LoadTexture(unsigned int &id, const std::string& path);

public:
    /** @brief Konstruktor siatki.
     */
    Material();

    /** @brief Destruktor siatki.
     */
    ~Material();

    /** @brief Wczytuje material z pliku (.mtl).
     * @param objPath - Sciezka do pliku z materialem.
     * @return true - Udalo sie wczytac material.
     * @return false - Wsytapil blad podczas wczytywania materialu.
     */
    bool Load(const std::string& mtlPath);
};

#endif // __MATERIAL_H__