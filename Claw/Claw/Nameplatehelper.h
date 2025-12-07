
#include "Nameplate.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <string>
#include "Util.h"
// NameplateHelper.h
#pragma once
#ifndef NAME_HELPER_H
#define NAME_HELPER_H



// Singleton text shader
inline unsigned int nameplateShader = 0;

// Call this once after creating window and loading font
inline void InitNameplateShader() {
    if (nameplateShader == 0) {
        nameplateShader = createShader("nameplate.vert", "nameplate.frag");
        glUseProgram(nameplateShader);
        glUniform1i(glGetUniformLocation(nameplateShader, "text"), 0); // texture unit 0
        glUseProgram(0);
    }
}

// Draws a string in the top-left corner, 10 px margin, white color
inline void DrawNameplateWorld(const std::string& name, const glm::mat4& projection, float worldWidth, float worldHeight)
{
    if (nameplateShader == 0) return;

    glUseProgram(nameplateShader);
    glUniformMatrix4fv(glGetUniformLocation(nameplateShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Top-left corner in world coordinates
    float marginX = 0.025f * worldWidth;  // 5% of width
    float marginY = 0.07f * worldHeight; // 5% of height
    float textX = -worldWidth + marginX;
    float textY = worldHeight - marginY;

    // Choose a scale that looks reasonable in your world units
    float scale = 0.0015f; // tweak this

    RenderText(nameplateShader, name, textX, textY, scale, glm::vec3(1.0f, 1.0f, 1.0f));

    glUseProgram(0);
}


#endif NAME_HELPER_H