#include "Pipe.h"
#include <glm/gtx/transform.hpp>

Pipe::Pipe(float xPos, float gapHeight, float pipeWidth, float pipeHeight)
    : m_Pos(glm::vec3(xPos, 0.0f, 0.0f)), m_GapHeight(gapHeight), m_PipeWidth(pipeWidth), m_PipeHeight(pipeHeight)
{
    Init(pipeWidth, pipeHeight);
}

Pipe::~Pipe()
{
    delete m_VBO;
    delete m_IBO;
    delete m_PipeTexture;
}

void Pipe::Init(float pipeWidth, float pipeHeight)
{
    // Initialize vertices for both upper and lower pipes
    float vertices[] = {
        // Lower pipe (bottom part of the gap)
        -pipeWidth / 2, -1.0f, 0.0f,   0.0f, 0.0f,  // Bottom-left
         pipeWidth / 2, -1.0f, 0.0f,   1.0f, 0.0f,  // Bottom-right
         pipeWidth / 2, m_GapHeight, 0.0f,   1.0f, 1.0f,  // Top-right
        -pipeWidth / 2, m_GapHeight, 0.0f,   0.0f, 1.0f,  // Top-left

        // Upper pipe (top part of the gap)
        -pipeWidth / 2, m_GapHeight + pipeHeight, 0.0f,   0.0f, 0.0f,  // Bottom-left
         pipeWidth / 2, m_GapHeight + pipeHeight, 0.0f,   1.0f, 0.0f,  // Bottom-right
         pipeWidth / 2, 1.0f, 0.0f,   1.0f, 1.0f,  // Top-right
        -pipeWidth / 2, 1.0f, 0.0f,   0.0f, 1.0f   // Top-left
    };

    unsigned int indices[] = {
        0, 1, 2,  // Lower pipe triangle 1
        2, 3, 0,  // Lower pipe triangle 2
        4, 5, 6,  // Upper pipe triangle 1
        6, 7, 4   // Upper pipe triangle 2
    };

    m_VBO = new VertexBuffer(vertices, sizeof(vertices));
    m_IBO = new IndexBuffer(indices, 12);

    VertexBufferLayout layout;
    layout.push<float>(3);  // Positions
    layout.push<float>(2);  // Texture coordinates
    m_VAO.addBuffer(*m_VBO, layout);

    m_Shader = new Shader("res/Shaders/Basic.shader");
    m_Shader->bind();

    m_Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    m_View = glm::mat4(1.0f);

    m_Shader->setUniformMat4f("u_Projection", m_Projection);
    m_Shader->setUniformMat4f("u_View", m_View);

    m_PipeTexture = new Texture("res/Textures/greenpipe.png");
    
    m_PipeTexture->bind(1);

    m_Shader->setUniform1i("u_Texture", 1);
}

void Pipe::Update(float deltaTime)
{
    // Move pipe to the left (similar to scrolling the background)
    m_Pos.x -= 0.5f * deltaTime;  // Adjust speed as necessary
}

void Pipe::Render()
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Pos);
    m_Shader->setUniformMat4f("u_Model", model);
    m_VAO.bind();
    m_IBO->Bind();
    m_Renderer->draw(m_VAO, *m_IBO, *m_Shader, GL_TRIANGLES);
}
