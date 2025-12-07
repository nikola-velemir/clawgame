#include "Toy.h"
#include "Util.h"
#include <glm/gtc/type_ptr.hpp>
Toy createToy(const float x, const float y, const float width, const float height,
    const float xMax, const float yMax, const char* image)
{
    Toy t = Toy(x, y, width, height);

    float scaledW = width * xMax;
    float scaledH = height * yMax;

    float vertices[] = {
        // Bottom-left
        -scaledW / 2, -scaledH / 2, 0.0f, 0.0f,
        // Bottom-right
         scaledW / 2, -scaledH / 2, 1.0f, 0.0f,
         // Top-right
          scaledW / 2,  scaledH / 2, 1.0f, 1.0f,
          // Top-left
          -scaledW / 2,  scaledH / 2, 0.0f, 1.0f
    };
    glGenVertexArrays(1, &t.VAO);
    glGenBuffers(1, &t.VBO);

    glBindVertexArray(t.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, t.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position = first 2 floats
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // UV = next 2 floats
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Load texture
    t.texture = loadImageToTexture(image);

    t.vertexCount = 4;

    return t;
}
void Toy::render(const GLuint shader, const glm::mat4& projection, float aspect)
{
    glUseProgram(shader);

    // Pass offset to shader
    GLint offsetLoc = glGetUniformLocation(shader, "uOffset");
    glUniform2f(offsetLoc, x + uX, y + uY);

    GLint projLoc = glGetUniformLocation(shader, "projection");
    if (projLoc >= 0) glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    GLint texLoc = glGetUniformLocation(shader, "uTexture");
    if (texLoc >= 0) glUniform1i(texLoc, 0);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, this->vertexCount);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Toy::Toy(const float _x, const float _y, const float _w, const float _h)
    : width(_w), height(_h), uX(_x), uY(_y)
{

}
