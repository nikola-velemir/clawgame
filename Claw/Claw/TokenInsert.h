#pragma once
#ifndef TOKEN_INSERT_H
#define TOKEN_INSERT_H

#include "Mesh.h"
#include <GL/glew.h>
#include <glm/fwd.hpp>

struct TokenInsert :public Mesh {
	GLuint texture;
	float width;
	float height;
	float x;
	float y;
	void render(const GLuint shader, const glm::mat4& projection, float aspect);
	TokenInsert(float _x, float _y, float _w, float h);
};

TokenInsert createTokenInsert(float _x, float _y, float width, float height, float maxX, float maxY);
#endif TOKEN_INSERT_H
