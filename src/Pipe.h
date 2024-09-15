#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include "Texture.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Renderer.h"
#include <glm/glm.hpp>

class Pipe
{
public:
    Pipe(float xPos, float gapHeight, float pipeWidth, float pipeHeight);
    ~Pipe();

    void Update(float deltaTime);
    void Render();

    glm::vec3 getPosition() const { return m_Pos; }
    float getWidth() const { return m_PipeWidth; }
    float getHeight() const { return m_PipeHeight; }
    float GetXPosition() const { return m_Pos.x; }

private:
    void Init(float pipeWidth, float pipeHeight);

    glm::vec3 m_Pos;           // Position of the pipe
    const float m_PipeWidth;         // Width of the pipe
    const float m_PipeHeight;        // Height of the pipe
    const float m_GapHeight;         // Gap between upper and lower pipes

    Texture* m_PipeTexture;
    VertexArray m_VAO;
    VertexBuffer* m_VBO;
    IndexBuffer* m_IBO;
    Shader* m_Shader;
    Renderer* m_Renderer;

    //Projections and View Matrices
    glm::mat4 m_Projection;
    glm::mat4 m_View;
};
