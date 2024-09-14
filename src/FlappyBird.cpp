#include "FlappyBird.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>

FlappyBird::FlappyBird()
    : m_BirdPos(glm::vec3(-0.5f, 0.0f, 0.0f)),
    m_BirdVelocity(glm::vec3(0.0f, 0.0f, 0.0f)),
    m_Gravity(-0.5f),
    m_JumpVelocity(0.4f),
    m_GameOver(false)
{
    Init();
}

FlappyBird::~FlappyBird()
{
    delete m_VBO;
    delete m_IBO;
    delete m_Shader;
    delete m_Renderer;
}

void FlappyBird::Init()
{
    // Initialize bird vertices as a simple square
    float birdVertices[] = {
        -0.05f, -0.05f, 0.0f,  // Bottom-left
         0.05f, -0.05f, 0.0f,  // Bottom-right
         0.05f,  0.05f, 0.0f,  // Top-right
        -0.05f,  0.05f, 0.0f   // Top-left
    };

    unsigned int birdIndices[] = {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };

    m_VBO = new VertexBuffer(birdVertices, sizeof(birdVertices));
    m_IBO = new IndexBuffer(birdIndices, 6);

    VertexBufferLayout layout;
    layout.push<float>(3);  // 3 floats per vertex (x, y, z)
    m_VAO.addBuffer(*m_VBO, layout);

    m_Shader = new Shader("res/Shaders/Basic.shader");
    m_Shader->bind();

    m_Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    m_View = glm::mat4(1.0f);

    m_Shader->setUniformMat4f("u_Projection", m_Projection);
    m_Shader->setUniformMat4f("u_View", m_View);

    m_Renderer = new Renderer();

}

void FlappyBird::Restart()
{
    m_BirdPos = glm::vec3(-0.5f, 0.0f, 0.0f);
    m_BirdVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    m_GameOver = false;
}

void FlappyBird::ProcessInput(float deltaTime)
{
    if (m_GameOver && glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_PRESS) {
        Restart();
    }

    if (!m_GameOver && glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS) {
        m_BirdVelocity.y = m_JumpVelocity;
    }
}

void FlappyBird::Update(float deltaTime)
{
    // Apply gravity to the bird
    m_BirdVelocity.y += m_Gravity * deltaTime;
    m_BirdPos += m_BirdVelocity * deltaTime;
}

void FlappyBird::Render()
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    m_Shader->bind();

    // Render the bird
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_BirdPos);  
    m_Shader->setUniformMat4f("u_Model", model);
    m_Shader->setUniform4f("uColor", 1.0f, 1.0f, 0.0f, 1.0f);  // Yellow color for the bird
    m_VAO.bind();
    m_IBO->Bind(); 
    m_Renderer->draw(m_VAO, *m_IBO, *m_Shader, GL_TRIANGLES); 

   
}

void FlappyBird::CheckCollisions()
{
    
}
