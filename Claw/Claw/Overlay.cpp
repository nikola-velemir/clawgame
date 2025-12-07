#include "Overlay.h"
#include <glm/gtc/type_ptr.hpp>
#include "Util.h"

void Overlay::render(const GLuint overLayShader, const glm::mat4& projection, float aspect, const float transparency)
{
	glUseProgram(overLayShader);
	GLint alphaLoc = glGetUniformLocation(overLayShader, "alpha");
	glUniform1f(alphaLoc, transparency);

	glm::mat4 model = glm::mat4(1.0f); // identity only

	GLuint modelLoc = glGetUniformLocation(overLayShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint projLoc = glGetUniformLocation(overLayShader, "projection");
	if (projLoc >= 0) glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	GLint texLoc = glGetUniformLocation(overLayShader, "uTexture");
	if (texLoc >= 0) glUniform1i(texLoc, 0);

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, this->vertexCount);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

Overlay createOverlay(float xMax, float yMax)
{
	Overlay o = Overlay{};
	float vertices[] = {
		xMax * 0.9f,yMax * 0.65,
		xMax * 0.9f,-yMax * 0.4,
		-xMax * 0.9f, -yMax * 0.4,
		-xMax * 0.9f,yMax * 0.65
	};
	glGenVertexArrays(1, &o.VAO);
	glGenBuffers(1, &o.VBO);

	glBindVertexArray(o.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, o.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Atribut 0 (pozicija):
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	o.texture = loadImageToTexture("glass_texture.png");
	o.vertexCount = 4;
	return o;
}
