#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Renderer.h"
#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
#include <iostream>
#include "Texture.h"
#include "Pipe.h"
class FlappyBird
{
private:
    //Vertex Arrays, Vertex Buffers,Index Buffers
    VertexArray m_VAO;
    VertexBuffer* m_VBO;
    IndexBuffer* m_IBO;
    
    Shader* m_Shader;
    Renderer* m_Renderer;
    std::vector<Texture*> m_Textures;

    bool m_GameOver;

    //Properties of the Bird
    glm::vec3 m_BirdPos;
    glm::vec3 m_BirdVelocity;
    float m_Gravity;
    float m_JumpVelocity;
    float m_AnimationTime;

    std::vector<Pipe*> m_Pipes;

    //Projections and View Matrices
    glm::mat4 m_Projection;
    glm::mat4 m_View;

public:
    FlappyBird();
    ~FlappyBird();   

    void Init();
    void Restart();
    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
    void Render();
    void CheckCollisions();
};
