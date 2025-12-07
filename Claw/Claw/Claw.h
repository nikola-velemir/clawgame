#pragma once

#ifndef CLAW_H
#define CLAW_H
#include "Mesh.h"
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include "Finger.h"
#include "Toy.h"

struct Claw : public Mesh {
	float x;
	float y;
	float ropeLenght;
	bool carryingToy;
	bool isOpen;
	Toy* grabbedToy = nullptr;
	float openingAngle = 0.0f;
	unsigned int ropeVAO;
	unsigned int ropeVBO;
	int ropeVertexCount;
	Finger leftFinger;
	Finger rightFinger;
	void render(GLuint shader, GLuint ropeShader, GLuint fingerShader, const glm::mat4& projection, float aspect);
	private: void renderPalm(const GLuint shader, const glm::mat4& projection, float aspect);

};

Claw createClaw(float xMax, float yMax);
#endif