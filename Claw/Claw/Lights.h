#ifndef LIGHTS_H
#define LIGHTS_H
#include "Mesh.h"

#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include "LightState.h"

struct Light:public Mesh {
	float x;
	float y;
	float width;
	float height;

	std::unordered_map<LightState, GLuint> textures;
	LightState currentState = LightState::Empty;
	void render(const GLuint outerShader,  const glm::mat4& projection, float aspect);
	
	Light(float _x, float _y, float _w, float h);
};

Light createLight(float x,float y, float width, float height, float xMax, float yMax);

#endif // !LIGHTS_H
