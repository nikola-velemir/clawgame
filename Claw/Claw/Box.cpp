#include "Box.h"

#include <GL/glew.h>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static void applyMatrices(GLuint shader, const glm::mat4& projection);
Box::Box(int w, int h) : width(w), height(h)
{
};

Box::Box()
{
};
Box createBox(float xMax, float yMax)
{
	const float boxR = 1.0;
	const float boxB = 0.0;
	const float boxG = 0.0;
	const float claimR = 0.2;
	const float claimG = 0.2;
	const float claimB = 0.2;
	Box box = Box{};
	float topVertices[] = {

		xMax,yMax * 0.825f, boxR, boxG, boxB,
		-xMax, yMax * 0.825f, boxR, boxG, boxB,
		-xMax, yMax * 0.65f, boxR, boxG, boxB,
		xMax,yMax * 0.65f, boxR, boxG, boxB
	};
	float bottomVertices[] = {
		-xMax, -yMax, boxR, boxG, boxB,  // bottom-left
		 xMax * 0.5f, -yMax, boxR, boxG, boxB,  // bottom-right
		-xMax, -yMax * 0.4f, boxR, boxG, boxB,  // top-left
		xMax * 0.5f, -yMax * 0.4f, boxR, boxG, boxB,   // top-right
	};
	float leftVertices[] = {
	-xMax, -yMax * 0.9f, boxR, boxG, boxB,  // bottom-left
	 -xMax * 0.9f, -yMax * 0.9f, boxR, boxG, boxB,  // bottom-right
	-xMax * 0.9f,yMax * 0.825f , boxR, boxG, boxB, // top-left
	 -xMax * 1.0f,yMax * 0.825f , boxR, boxG, boxB  // top-right
	};
	float rightVertices[] = {
	xMax * 1.0f,yMax * 0.825f, boxR, boxG, boxB,  // bottom-left
	xMax * 0.9f,yMax * 0.825f, boxR, boxG, boxB,  // bottom-right
	xMax * 0.9f,-yMax, boxR, boxG, boxB, // top-left
	xMax * 1.0f,-yMax, boxR, boxG, boxB  // top-right
	};

	float claimZoneVertices[] = {
		xMax * .5f, -yMax * 0.4f, claimR, claimG, claimB,
		xMax * .5f, -yMax, claimR, claimG, claimB,
		xMax, -yMax * 0.4f,  claimR, claimG, claimB,
		xMax, -yMax,  claimR, claimG, claimB
	};

	float claimZoneTopStripVertices[] = {
		xMax * .5f, -yMax * 0.4f, boxR, boxG, boxB,
		xMax * .5f, -yMax * 0.5f, boxR, boxG, boxB,
		xMax, -yMax * 0.4f, boxR, boxG, boxB,
		xMax, -yMax * 0.5f, boxR, boxG, boxB
	};
	float dropZoneVertices[] = {
		xMax * .5f, -yMax * 0.375f, claimR, claimG, claimB,
		xMax * .5f, -yMax * 0.4f, claimR, claimG, claimB,
		xMax, - yMax * 0.375f, claimR, claimG, claimB,
		xMax,  - yMax * 0.4f, claimR, claimG, claimB
	};
	Mesh dropZone = Mesh{};
	glGenVertexArrays(1, &dropZone.VAO);
	glGenBuffers(1, &dropZone.VBO);

	glBindVertexArray(dropZone.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, dropZone.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(dropZoneVertices), dropZoneVertices, GL_STATIC_DRAW);

	// Atribut 0 (pozicija):
	glVertexAttribPointer(
		0, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(float), (void*)0
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(float),
		(void*)(2 * sizeof(float))
	);
	glEnableVertexAttribArray(1);

	dropZone.vertexCount = 4;
	box.dropZone = dropZone;


	Mesh claimZoneStrip = Mesh{};
	glGenVertexArrays(1, &claimZoneStrip.VAO);
	glGenBuffers(1, &claimZoneStrip.VBO);

	glBindVertexArray(claimZoneStrip.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, claimZoneStrip.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(claimZoneTopStripVertices), claimZoneTopStripVertices, GL_STATIC_DRAW);

	// Atribut 0 (pozicija):
	glVertexAttribPointer(
		0, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(float), (void*)0
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(float),
		(void*)(2 * sizeof(float))
	);
	glEnableVertexAttribArray(1);

	claimZoneStrip.vertexCount = 4;
	box.claimZoneStrip = claimZoneStrip;

	Mesh claimZone = Mesh{};
	glGenVertexArrays(1, &claimZone.VAO);
	glGenBuffers(1, &claimZone.VBO);

	glBindVertexArray(claimZone.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, claimZone.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(claimZoneVertices), claimZoneVertices, GL_STATIC_DRAW);

	// Atribut 0 (pozicija):
	glVertexAttribPointer(
		0, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(float), (void*)0
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(float),
		(void*)(2 * sizeof(float))
	);
	glEnableVertexAttribArray(1);

	claimZone.vertexCount = 4;
	box.claimZone = claimZone;

	Mesh top = Mesh{};
	glGenVertexArrays(1, &top.VAO);
	glGenBuffers(1, &top.VBO);

	glBindVertexArray(top.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, top.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(topVertices), topVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(float), (void*)0
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(float),
		(void*)(2 * sizeof(float))
	);
	glEnableVertexAttribArray(1);
	top.vertexCount = 4;

	Mesh bottom = Mesh{};
	glGenVertexArrays(1, &bottom.VAO);
	glGenBuffers(1, &bottom.VBO);

	glBindVertexArray(bottom.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, bottom.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bottomVertices), bottomVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(float), (void*)0
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(float),
		(void*)(2 * sizeof(float))
	);
	glEnableVertexAttribArray(1);

	bottom.vertexCount = 4;
	box.top = top;
	box.bottom = bottom;

	Mesh left = Mesh{};
	glGenVertexArrays(1, &left.VAO);
	glGenBuffers(1, &left.VBO);

	glBindVertexArray(left.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, left.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leftVertices), leftVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(float), (void*)0
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(float),
		(void*)(2 * sizeof(float))
	);
	glEnableVertexAttribArray(1);

	left.vertexCount = 4;
	box.left = left;

	Mesh right = Mesh{};
	glGenVertexArrays(1, &right.VAO);
	glGenBuffers(1, &right.VBO);

	glBindVertexArray(right.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, right.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rightVertices), rightVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(float), (void*)0
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(float),
		(void*)(2 * sizeof(float))
	);
	glEnableVertexAttribArray(1);

	right.vertexCount = 4;
	box.right = right;


	return box;
}

void Box::render(const GLuint boxShader, const GLuint claimZoneShader, const glm::mat4& projection, float aspect)
{
	// Render with box shader
	renderTop(boxShader, projection);
	renderBottom(boxShader, projection);
	renderLeft(boxShader, projection);

	// Claim zone shader
	renderClaimZone(claimZoneShader, projection);
	renderDropZone(claimZoneShader, projection);

	// Back to box shader
	renderRight(boxShader, projection);

	// Claim zone strip (still box shader? if wrong, switch shader here)
	renderClaimZoneStrip(boxShader, projection);

}
void Box::renderDropZone(GLuint shader, const glm::mat4& projection)
{
	applyMatrices(shader, projection);
	glBindVertexArray(dropZone.VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, dropZone.vertexCount);
}

void Box::renderClaimZoneStrip(GLuint shader, const glm::mat4& projection)
{
	applyMatrices(shader, projection);
	glBindVertexArray(claimZoneStrip.VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, claimZoneStrip.vertexCount);
}

void Box::renderClaimZone(GLuint shader, const glm::mat4& projection)
{
	applyMatrices(shader, projection);
	glBindVertexArray(claimZone.VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, claimZone.vertexCount);
}

void Box::renderRight(GLuint shader, const glm::mat4& projection)
{
	applyMatrices(shader, projection);
	glBindVertexArray(right.VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, right.vertexCount);
}

void Box::renderLeft(GLuint shader, const glm::mat4& projection)
{
	applyMatrices(shader, projection);
	glBindVertexArray(left.VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, left.vertexCount);
}

void Box::renderBottom(GLuint shader, const glm::mat4& projection)
{
	applyMatrices(shader, projection);
	glBindVertexArray(bottom.VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, bottom.vertexCount);
}

void Box::renderTop(GLuint shader, const glm::mat4& projection)
{
	applyMatrices(shader, projection);
	glBindVertexArray(top.VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, top.vertexCount);
}

static void applyMatrices(GLuint shader, const glm::mat4& projection) {
	glUseProgram(shader);

	GLuint projLoc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	GLint offsetLoc = glGetUniformLocation(shader, "uOffset");

	glUniform2f(offsetLoc, 0.0f, 0.0f);
}
