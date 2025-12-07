#include "Finger.h"

#include <GL/glew.h>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include "Util.h"

Finger createFinger(float vertices[], float xMax, float yMax) {
	Finger f{};


	glGenVertexArrays(1, &f.VAO);
	glGenBuffers(1, &f.VBO);

	glBindVertexArray(f.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, f.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*5*2, vertices, GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * 5, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,                 
        2,                  
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float), 
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,                        
        3,                       
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),       
        (void*)(2 * sizeof(float))
    );
    glEnableVertexAttribArray(1);
	f.vertexCount = 5;
	return f;
}
void Finger::render(const GLuint shader,
    float rotationAngle,
    bool isLeftFinger,
    float circleX,
    float circleY,
    float circleUx,
    float circleUy,
    const glm::mat4& projection,
    const float aspect) {

    glUseProgram(shader);

    // Position the finger at the claw's current position
    GLuint offsetLoc = glGetUniformLocation(shader, "uOffset");
    glUniform2f(offsetLoc, circleUx, circleUy);

    GLuint scaleLoc = glGetUniformLocation(shader, "uScale");
    glUniform2f(scaleLoc, 1.0f, 1.0f);

    GLuint rotationLoc = glGetUniformLocation(shader, "uRotation");
    float finalRotation = isLeftFinger ? -rotationAngle : rotationAngle;
    glUniform1f(rotationLoc, finalRotation);

    GLuint pivotLoc = glGetUniformLocation(shader, "uPivot");
    
    float r = 0.07f;
    float pivotX = isLeftFinger ? (-(0.00f + r) + circleX) : ((0.00f + r) + circleX);
    float pivotY = 0.0f + circleY;

    glUniform2f(pivotLoc, pivotX, pivotY);

    GLuint projLoc = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, this->vertexCount);
}