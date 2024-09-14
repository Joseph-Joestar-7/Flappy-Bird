#pragma once
#include "Renderer.h"

class Pipe {
public:
    Pipe(float x, float gapY);        // Constructor with initial position and gap

    void Update(float deltaTime);     // Update the pipe's position
    void Render(Renderer& renderer);  // Render the pipe using the Renderer
    bool IsOffScreen();               // Check if the pipe has moved off-screen

    glm::vec3 GetPosition();          // Get the current position of the pipe

private:
    glm::vec3 m_PipePos;              // Position of the pipe
    float m_GapY;                     // Y position of the gap between pipes

    static const float PIPE_WIDTH;    // Width of the pipe
    static const float PIPE_HEIGHT;   // Height of the pipe
    static const float PIPE_SPEED;    // Speed at which the pipe moves (toward the bird)
};
