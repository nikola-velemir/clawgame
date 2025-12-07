#ifndef TOY_H
#define TOY_H
#include "Mesh.h"
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include "Util.h"
#include "ToyState.h"

struct Toy :public Mesh {
	GLuint texture;
	float x = 0.0f;
	float y = 0.0f;
	float uX;
	float uY;
	float width;
	float height;
	ToyState state = ToyState::Idle;
	float velocityY = 0.0f;
	void render(const GLuint shader, const glm::mat4& projection, float aspect);
	Toy(const float _x, const float _y, const float _w, const float _h);
};

Toy createToy(const float x, const float y, const float width, const float height, const float xMax, const float yMax, const char* image);


#endif // !TOY_H
