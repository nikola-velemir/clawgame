#pragma once
#ifndef OVERLAY_H
#define OVERLAY_H



#include "Mesh.h"
#include <GL/glew.h>
#include <glm/fwd.hpp>

struct Overlay : public Mesh {
	GLuint texture;
	void render(const GLuint overLayShader, const glm::mat4& projection, float aspect, const float transparency);

};
Overlay createOverlay(float xMax, float yMax);

#endif // !OVERLAY_H
