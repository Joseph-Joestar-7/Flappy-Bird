#pragma once
#include "Texture.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include <glm/glm.hpp>

class Pipe
{
public:
    Pipe(float xPos, float gapHeight, float pipeWidth, float pipeHeight);
    ~Pipe();

    void Update(float deltaTime);
    void Render(const Shader& shader, Renderer& renderer);

    glm::vec3 getPosition() const { return m_Pos; }
    float getWidth() const { return m_PipeWidth; }
    float getHeight() const { return m_PipeHeight; }

private:
    void Init(float pipeWidth, float pipeHeight);

    glm::vec3 m_Pos;           // Position of the pipe
    float m_PipeWidth;         // Width of the pipe
    float m_PipeHeight;        // Height of the pipe
    float m_GapHeight;         // Gap between upper and lower pipes

    Texture* m_PipeTexture;
    VertexArray m_VAO;
    VertexBuffer* m_VBO;
    IndexBuffer* m_IBO;
};
