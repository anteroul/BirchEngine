#pragma once

#include "Mesh.h"
#include <glm/glm.hpp>
#include <vector>

class Cube : public Mesh {
public:
    // create cube with default color
    Cube(const std::string &texturePath);
    // create cube with color
    Cube(const std::string &texturePath, const glm::vec3 &cubeColor);
    void render(Camera &camera) override;
private:
    static std::vector<float> createVertexBufferData();
    std::vector<float> createUvData();
    void create(std::vector<float> &&vertexBufferData, std::vector<float> &&uvData);
};

