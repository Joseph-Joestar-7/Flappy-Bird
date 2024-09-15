#include "FlappyBird.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>

FlappyBird::FlappyBird()
    : m_BirdPos(glm::vec3(-0.5f, 0.0f, 0.0f)),
    m_BirdVelocity(glm::vec3(0.0f, 0.0f, 0.0f)),
    m_Gravity(-0.5f),
    m_JumpVelocity(0.4f),
    m_GameOver(false),
    m_PipeSpawnTimer(0.0f),
    m_SpawnInterval(2.0f)
{
    Init();
}

FlappyBird::~FlappyBird()
{
    delete m_VBO;
    delete m_IBO;
    delete m_Shader;
    delete m_Renderer;
    for (Texture* texture : m_Textures) {
        delete texture;
    }
    m_Textures.clear();

    for (Pipe* pipe : m_Pipes)
    {
        delete pipe;
    }
    m_Pipes.clear();
}

void FlappyBird::Init()
{
    srand(static_cast<unsigned int>(time(0)));

    // Initialize bird vertices and the texture coords
    float birdVertices[] = {
    -0.05f, -0.05f, 0.0f,  0.165f, 0.165f,  // Bottom-left
     0.05f, -0.05f, 0.0f,  0.95f, 0.165f,  // Bottom-right
     0.05f,  0.05f, 0.0f,  0.95f, 0.95f,  // Top-right
    -0.05f,  0.05f, 0.0f,  0.165f, 0.95f   // Top-left
    };

    unsigned int birdIndices[] = {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };

    m_VBO = new VertexBuffer(birdVertices, sizeof(birdVertices));
    m_IBO = new IndexBuffer(birdIndices, 6);

    VertexBufferLayout layout;
    layout.push<float>(3);  // 3 floats per vertex (x, y, z)
    layout.push<float>(2); // for the texture co-ords
    m_VAO.addBuffer(*m_VBO, layout);

    m_Shader = new Shader("res/Shaders/Basic.shader");
    m_Shader->bind();

    m_Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    m_View = glm::mat4(1.0f);

    m_Shader->setUniformMat4f("u_Projection", m_Projection);
    m_Shader->setUniformMat4f("u_View", m_View);

    //The 4 textures
    m_Textures.push_back(new Texture("res/Textures/bluebird1.png"));
    m_Textures.push_back(new Texture("res/Textures/bluebird2.png"));
    m_Textures.push_back(new Texture("res/Textures/bluebird3.png"));
    m_Textures.push_back(new Texture("res/Textures/bluebird4.png"));

    //Bind the first one for now
    m_Textures[0]->bind(0);

    m_Shader->setUniform1i("u_Texture", 0);

    m_Renderer = new Renderer();
    
    m_Pipes.push_back(new Pipe(0.7f, 0.5f, 0.15f, 1.0f));  // First pipe
    m_Pipes.push_back(new Pipe(1.2f, 0.45f, 0.15f, 1.0f)); // Second pipe
    m_Pipes.push_back(new Pipe(1.7f, 0.55f, 0.15f, 1.0f)); // Third pipe

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

    // Update animation time
    m_AnimationTime += deltaTime;

    // Alternate between textures based on time
    float animationCycle = 0.25f;  // How long each frame lasts

    int currentFrame = static_cast<int>(fmod(m_AnimationTime, animationCycle * 4.0f) / animationCycle);
    m_Textures[currentFrame]->bind(0);

    if(!m_GameOver)
    {
    for (Pipe* pipe : m_Pipes)
    {
        pipe->Update(deltaTime); // Move pipes to the left
    }

    // Check if we need to spawn a new pipe
    m_PipeSpawnTimer += deltaTime;
    if (m_PipeSpawnTimer >= m_SpawnInterval) {
        m_PipeSpawnTimer = 0.0f;
        SpawnNewPipe();
    }

    // Remove pipes that are off the screen or recycle them
    for (auto it = m_Pipes.begin(); it != m_Pipes.end(); )
    {
        if ((*it)->getPosition().x < -1.1f)  // Pipe is off the left side of the screen
        {
            delete* it;  // Remove pipe from memory
            it = m_Pipes.erase(it);  // Remove pipe from the list
        }
        else {
            ++it;
        }
    }

    for (Pipe* pipe : m_Pipes)
    {
        pipe->Render();
    }
}

    CheckCollisions();

}

void FlappyBird::Render()
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    m_Shader->bind();

    // Render the bird
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_BirdPos);  
    m_Shader->setUniformMat4f("u_Model", model);
    m_VAO.bind();
    m_IBO->Bind(); 
    m_Renderer->draw(m_VAO, *m_IBO, *m_Shader, GL_TRIANGLES); 

    for (Pipe* pipe : m_Pipes)
    {
        pipe->Render();
    }

}

void FlappyBird::SpawnNewPipe()
{
    // Randomize the gap height once when the pipe is created
    float randomGapHeight = 0.4f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (0.6f - 0.4f)));

    // Add a new pipe to the right side of the screen with a constant gap height
    m_Pipes.push_back(new Pipe(1.2f, randomGapHeight, 0.15f, 1.0f));
}


void FlappyBird::CheckCollisions()
{
    float birdWidth = 0.1f;  // Width of the bird
    float birdHeight = 0.1f; // Height of the bird
    float birdLeft = m_BirdPos.x - birdWidth / 2;
    float birdRight = m_BirdPos.x + birdWidth / 2;
    float birdBottom = m_BirdPos.y - birdHeight / 2;
    float birdTop = m_BirdPos.y + birdHeight / 2;

    // Define ground and ceiling boundaries
    float groundLevel = -1.0f;
    float ceilingLevel = 1.0f;

    // Check for ground or ceiling collision
    if (birdBottom <= groundLevel) {
        m_BirdPos.y = groundLevel + birdHeight / 2;  // Keep the bird at the ground level
        m_BirdVelocity.y = 0;  // Stop downward velocity
    }

    if (birdTop >= ceilingLevel) {
        m_BirdPos.y = ceilingLevel - birdHeight / 2;  // Keep the bird at the ceiling level
        m_BirdVelocity.y = 0;  // Stop upward velocity
    }

    // Check for pipe collisions
    for (Pipe* pipe : m_Pipes) {
        // Get the pipe's position and size
        float pipeX = pipe->getPosition().x;
        float pipeWidth = pipe->getWidth();
        float pipeHeight = pipe->getHeight();
        float gapHeight = pipe->getHeight();  // The height of the gap between pipes
        float gapY = pipe->getPosition().y;   // y-position of the gap center

        // Calculate pipe bounding boxes
        // Bottom pipe
        float pipeBottomLeft = pipeX - pipeWidth / 2;
        float pipeBottomRight = pipeX + pipeWidth / 2;
        float pipeBottomTop = gapY - gapHeight / 2  ;  // Bottom pipe extends from the bottom to the gap

        // Top pipe
        float pipeTopBottom = gapY + gapHeight / 2;
        float pipeTopLeft = pipeX - pipeWidth / 2;
        float pipeTopRight = pipeX + pipeWidth / 2;

        // Check if bird collides with the bottom pipe
        if (birdRight > pipeBottomLeft && birdLeft < pipeBottomRight && birdBottom < pipeBottomTop) {
            m_GameOver = true;
            return;
        }
        // Check if bird collides with the top pipe
        if (birdRight > pipeTopLeft && birdLeft < pipeTopRight && birdTop > pipeTopBottom) {
            m_GameOver = true;
            return;
        }
    }

}
