#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Renderer.h"
#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "Pipe.h"
#include "GLFW/glfw3.h"
#include <iostream>

class FlappyBird
{
public:
	FlappyBird();
	~FlappyBird();

    void Init();                      // Initialize game components
    void Restart();                   // Restart the game
    void ProcessInput(float deltaTime); // Handle user input (jumping)
    void Update(float deltaTime);     // Update the game state (bird movement, pipe movement)
    void Render();                    // Render the game objects (bird, pipes, background)
    void CheckCollisions();           // Check for collisions between bird and pipes

private:
    glm::vec3 m_BirdPos;              // Bird's position
    glm::vec3 m_BirdVelocity;         // Bird's velocity (affected by gravity and jump)

    float m_Gravity;                  // Gravity affecting the bird
    float m_JumpVelocity;             // Upward velocity when jumping

    std::vector<Pipe> m_Pipes;        // List of pipes
    float m_PipeSpawnTime;            // Time between spawning new pipes

    bool m_GameOver;                  // Game over flag

    VertexArray m_VAO;
    VertexBuffer* m_VBO;
    IndexBuffer* m_IBO;
    Shader* m_Shader;
    Renderer* m_Renderer;

    // Projections and View matrices
    glm::mat4 m_Projection;
    glm::mat4 m_View;
};
