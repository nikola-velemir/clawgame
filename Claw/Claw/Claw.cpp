#include "Claw.h"
#include <GL/glew.h>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Util.h"
#define NUM_SLICES 40


Claw createClaw(float xMax,float yMax) {
	Claw c{};
	float vertices[(NUM_SLICES + 2) * 5] = {
		0.0f
	};
	c.x = 0.0f;
	c.y = 0.7f;

	const float palmR = 0.15f;
	const float palmG = 0.15f;
	const float palmB = 0.15f;

	vertices[0] = c.x;
	vertices[1] = c.y;
	vertices[2] = palmR;
	vertices[3] = palmG;
	vertices[4] = palmB;

	float r = 0.07f;


	for (int i = 1; i < NUM_SLICES + 2; ++i) {
		float angle = i * 2 * 3.141592653589f / NUM_SLICES;
		vertices[i * 5 + 0] = cos(angle) * r + c.x;
		vertices[i * 5 + 1] = sin(angle) * r + c.y;
		vertices[i * 5 + 2] = palmR;
		vertices[i * 5 + 3] = palmG;
		vertices[i * 5 + 4] = palmB;
	}


	glGenVertexArrays(1, &c.VAO);
	glGenBuffers(1, &c.VBO);

	glBindVertexArray(c.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, c.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(float), (void*)0
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(float), (void*)(2 * sizeof(float))
	);
	glEnableVertexAttribArray(1);

	c.vertexCount = NUM_SLICES + 2;

	float ropeHalfWidth = r / 6;
	float ropeXmax = c.x + ropeHalfWidth;

	const float ropeR = 0.1f;
	const float ropeG = 0.1f;
	const float ropeB = 0.1f;

	float ropeVertices[] = {

		-ropeXmax, c.y, 1.0f,ropeR, ropeG, ropeB,
		ropeXmax,  c.y, 1.0f,ropeR, ropeG, ropeB,
		ropeXmax,yMax * 0.8f, 0.0f,ropeR, ropeG, ropeB,
		-ropeXmax,yMax * 0.8f, 0.0f, ropeR, ropeG, ropeB
	};

	glGenVertexArrays(1, &c.ropeVAO);
	glGenBuffers(1, &c.ropeVBO);

	glBindVertexArray(c.ropeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, c.ropeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ropeVertices), ropeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0); 

	glVertexAttribPointer(
		1, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1); 

	glVertexAttribPointer(
		2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2); 

	c.ropeVertexCount = 4;

	const float fingerR = 0.25;
	const float fingerG = 0.25;
	const float fingerB = 0.25;
	// Left finger - proper pentagon
	float leftFingerVerts[] = {
		-(0.00f+r) + c.x,  0.0f + c.y, fingerR, fingerG, fingerB,   // base center
		-0.14f + c.x, -0.12f + c.y,  fingerR, fingerG, fingerB, // left side
		-0.10f + c.x, -0.26f + c.y, fingerR, fingerG, fingerB,  // left bottom
		-0.05f + c.x, -0.32f + c.y, fingerR, fingerG, fingerB,  // tip (bottom center)
		-0.00f + c.x, -0.16f + c.y,  fingerR, fingerG, fingerB  // right bottom
	};

	// Right finger - mirror of left
	float rightFingerVerts[] = {
	0.00f+r + c.x,  0.0f + c.y,  fingerR, fingerG, fingerB,   // base center (mirror of -0.05f)
	0.14f + c.x, -0.12f + c.y, fingerR, fingerG, fingerB,   // right side
	0.10f + c.x, -0.26f + c.y, fingerR, fingerG, fingerB,   // right bottom
	0.05f + c.x, -0.32f + c.y,  fingerR, fingerG, fingerB,  // tip (bottom center)
	0.00f + c.x, -0.16f + c.y,  fingerR, fingerG, fingerB   // left bottom
	};

	c.leftFinger = createFinger(leftFingerVerts, xMax, yMax);
	c.rightFinger = createFinger(rightFingerVerts, xMax, yMax);
	return c;
}
void renderCircle(const Claw& c, GLuint shader) {
	glUseProgram(shader);
	glBindVertexArray(c.VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, c.vertexCount);
}
void Claw::render(GLuint shader, GLuint ropeShader, GLuint fingerShader, const glm::mat4& projection, float aspect)
{
	glUseProgram(ropeShader);

	glUniform1f(glGetUniformLocation(ropeShader, "uXOffset"), uX);
	glUniform1f(glGetUniformLocation(ropeShader, "uYOffset"), uY);

	GLuint ropeScaleLoc = glGetUniformLocation(ropeShader, "uScale");
	glUniform2f(ropeScaleLoc, 1.0f, 1.0f);

	GLuint ropeProjLoc = glGetUniformLocation(ropeShader, "projection");
	glUniformMatrix4fv(ropeProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(this->ropeVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, this->ropeVertexCount);

	// Render palm
	glUseProgram(shader);

	GLuint offsetLoc = glGetUniformLocation(shader, "uOffset");
	glUniform2f(offsetLoc, uX, uY);

	GLuint scaleLoc = glGetUniformLocation(shader, "uScale");
	float radius = 1.0f;
	glUniform2f(scaleLoc, radius, radius);

	GLuint projLoc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, this->vertexCount);

	this->leftFinger.render(
		fingerShader, 
		this->openingAngle,
		true,
		this->x,
		this->y,
		this->uX,
		this->uY,
		projection, 
		aspect);

	this->rightFinger.render(
		fingerShader,
		this->openingAngle,
		false,
		this->x,
		this->y,
		this->uX,
		this->uY,
		projection,
		aspect);
}
