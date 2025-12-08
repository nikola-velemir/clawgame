#include "Lights.h"
#include <glm/gtc/type_ptr.hpp>
#include "Util.h"
#define NUM_SLICES 100


void Light::render(const GLuint shader, const glm::mat4& projection, float aspect)
{
	glUseProgram(shader);

	// --- PROJECTION ---
	GLint projLoc = glGetUniformLocation(shader, "projection");
	if (projLoc >= 0)
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


	// --- TEXTURE ---
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textures[this->currentState]);

	GLint texLoc = glGetUniformLocation(shader, "uTexture");
	glUniform1i(texLoc, 0);

	// --- DRAW ---
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


Light::Light(float _x, float _y, float _w, float _h) :x(_x), y(_y), width(_w), height(_h)
{
}

Light createLight(float x, float y, float width, float height, float xMax, float yMax)
{

	Light light = Light(x, y, width, height);

	float scaledW = xMax * width;
	float scaledH = xMax * height;

	float vertices[] = {
		// x, y, u, v
		x - scaledW / 2,  y - scaledH / 2,  0.0f, 0.0f,   // bottom-left
		x + scaledW / 2,  y - scaledH / 2,  1.0f, 0.0f,   // bottom-right
		x - scaledW / 2,  y + scaledH / 2,  0.0f, 1.0f,   // top-left
		x + scaledW / 2,  y + scaledH / 2,  1.0f, 1.0f    // top-right
	};


	glGenVertexArrays(1, &light.VAO);
	glGenBuffers(1, &light.VBO);

	glBindVertexArray(light.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, light.VBO);
	glBufferData(GL_ARRAY_BUFFER,  sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint blueTexture = loadImageToTexture("light_blue.png");
	GLuint emptyTexture = loadImageToTexture("light_empty.png");
	GLuint redTexture = loadImageToTexture("light_red.png");
	GLuint greenTexture = loadImageToTexture("light_green.png");
	
	light.textures[LightState::Empty] = emptyTexture;
	light.textures[LightState::Red] = redTexture;
	light.textures[LightState::Blue] = blueTexture;
	light.textures[LightState::Green] = greenTexture;

	light.currentState = LightState::Empty;
 
	light.vertexCount = 4;

	return light;
}
