#pragma once
#include <iostream>
#include <vector>
#include "Camera.h"
#include "Shader.h"
#include <SFML/Graphics.hpp>

namespace VoxelWaffle {
	class IDrawable {
	public: 
		virtual ~IDrawable() = default;
		virtual void draw(Shader& program) = 0;
	};

	class IRenderer {
	public:
		virtual ~IRenderer() = default;

		virtual void registerCamera(Camera* camera) = 0;
		virtual void unregisterCamera(Camera* camera) = 0;

		virtual void registerDrawable(IDrawable* renderable) = 0;
		virtual void unregisterDrawable(IDrawable* renderable) = 0;
	};

	class Renderer : public IRenderer
	{
	public:
		Renderer(sf::RenderWindow& window);

		void render();

		void registerCamera(Camera* camera) override;
		void unregisterCamera(Camera* camera) override;

		void registerDrawable(IDrawable* renderable) override;
		void unregisterDrawable(IDrawable* renderable) override;

	private:
		std::unique_ptr<VoxelWaffle::Shader> m_shader;

		Camera* m_camera{ nullptr };
		std::vector<IDrawable*> m_drawables;
	};
}

