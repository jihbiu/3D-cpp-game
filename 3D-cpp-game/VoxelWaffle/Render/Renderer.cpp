#include "Renderer.h"
#include "../../Game.h"

VoxelWaffle::Renderer::Renderer(sf::RenderWindow& window)
{
    gladLoadGL();
    glViewport(0, 0, static_cast<GLsizei>(window.getSize().x), static_cast<GLsizei>(window.getSize().y));
    glEnable(GL_DEPTH_TEST);

    m_shader = std::make_unique<Shader>(
        "res/shaders/vertexShader.shader", "res/shaders/fragmentShader.shader"
    ); 
}

void VoxelWaffle::Renderer::render()
{
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->m_shader->use();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = m_camera->getView();
    glm::mat4 projection = m_camera->getProjection();

    m_shader->setMat4("Model", model);
    m_shader->setMat4("View", view);
    m_shader->setMat4("Projection", projection);


    for (VoxelWaffle::IDrawable* drawable : m_drawables) {
        if (drawable != nullptr) {
            drawable->draw(*m_shader);
        }
    }
}

void VoxelWaffle::Renderer::registerCamera(Camera* camera)
{
	m_camera = camera;
}

void VoxelWaffle::Renderer::unregisterCamera(Camera* camera)
{
    m_camera = nullptr;
}

void VoxelWaffle::Renderer::registerDrawable(IDrawable* renderable)
{
    m_drawables.push_back(renderable);
}

void VoxelWaffle::Renderer::unregisterDrawable(IDrawable* renderable)
{
    m_drawables.erase(
        std::remove(m_drawables.begin(), m_drawables.end(), renderable),
        m_drawables.end()
    );
}
