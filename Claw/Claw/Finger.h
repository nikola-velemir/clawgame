#ifndef FINGER_H
#define FINGER_H

#include "Mesh.h"
#include <GL/glew.h>
#include <glm/fwd.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
struct Finger : public Mesh {
	void render(const GLuint shader,
		float rotationAngle,
		bool isLeftFinger,
		float circleX, 
		float circleY,
		float circleUx,
		float circleUy,
		const glm::mat4& projection,
		const float aspect);
};
Finger createFinger(float vertices[], float xMax, float yMax);
#endif // !FINGER_H
