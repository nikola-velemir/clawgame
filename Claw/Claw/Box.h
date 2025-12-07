#pragma once
#ifndef BOX_H
#define BOX_H

#include "Mesh.h"
#include <GL/glew.h>
#include <glm/fwd.hpp>

struct Box
{	
	int width;
	int height;

	Box(int w, int h);
	Box();
	Mesh left;
	Mesh top;
	Mesh right;
	Mesh bottom;
	Mesh claimZone;
	Mesh claimZoneStrip;
	Mesh dropZone;
	void render(const GLuint boxShader, const GLuint claimZoneShader, const glm::mat4& projection, float aspect);
	void renderTop(GLuint shader, const glm::mat4& projection);
	void renderBottom(GLuint shader, const glm::mat4& projection);
	void renderLeft(GLuint shader, const glm::mat4& projection);
	void renderRight(GLuint shader, const glm::mat4& projection);

	void renderClaimZone(GLuint shader, const glm::mat4& projection);
	void renderClaimZoneStrip(GLuint shader, const glm::mat4& projection);
	void renderDropZone(GLuint shader, const glm::mat4& projection);

};
Box createBox(float xMax, float yMax);


#endif // !BOX_H
