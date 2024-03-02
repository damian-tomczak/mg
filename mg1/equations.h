#pragma once

#include "glm/glm.hpp"

glm::mat4 calculateDPrim(glm::vec3 t, glm::vec3 s, glm::vec3 r, float a, float b, float c);
float elipsoidZ(float x, float y, const glm::mat4& dPrim);
glm::vec3 normalVector(glm::vec3 v, const glm::mat4& dPrim);