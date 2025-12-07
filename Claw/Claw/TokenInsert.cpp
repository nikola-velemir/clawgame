#include "TokenInsert.h"
#include "Util.h"

#include <glm/gtc/type_ptr.hpp>

void TokenInsert::render(const GLuint shader, const glm::mat4& projection, float aspect)
{
	glUseProgram(shader);

	// --- PROJECTION ---
	GLint projLoc = glGetUniformLocation(shader, "projection");
	if (projLoc >= 0)
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// --- MODEL MATRIX ---
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(this->x, this->y, 0.0f));
	model = glm::scale(model, glm::vec3(this->width, this->height, 1.0f));


	GLint modelLoc = glGetUniformLocation(shader, "model");
	if (modelLoc >= 0)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// --- TEXTURE ---
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	GLint texLoc = glGetUniformLocation(shader, "uTexture");
	glUniform1i(texLoc, 0);

	// --- DRAW ---
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}

TokenInsert::TokenInsert(float _x, float _y, float _w, float _h) : width(_w), height(_h), x(_x), y(_y)
{
}

TokenInsert createTokenInsert(float _x, float _y, float width, float height, float xMax, float yMax)
{
	TokenInsert ti = TokenInsert(_x, _y, width, height);

	float scaledW = xMax * width;
	float scaledH = yMax * height;

	float vertices[] = {
		// x, y, u, v
		ti.x - scaledW/2, ti.y + scaledH/2, 0.0f, 1.0f,  // top-left
		ti.x - scaledW / 2, ti.y - scaledH / 2, 0.0f, 0.0f,  // bottom-left
		 ti.x + scaledW / 2, ti.y - scaledH / 2, 1.0f, 0.0f,  // bottom-right
		ti.x + scaledW / 2, ti.y + scaledH / 2, 1.0f, 1.0f   // top-right
	};

	glGenVertexArrays(1, &ti.VAO);
	glGenBuffers(1, &ti.VBO);

	glBindVertexArray(ti.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, ti.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	ti.texture = loadImageToTexture("token_insert.png");
	ti.vertexCount = 4;

	return ti;
}
