#pragma once
#include <iostream>
#include <vector>
#include "Camera.h"
#include "Shader.h"

namespace VoxelWaffle {
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void draw();
		void addQueue(VoxelWaffle::Shader shader);

		void Render();
		void RenderUI();
	private:
		std::unique_ptr<Camera> m_camera;
		std::vector<VoxelWaffle::Shader> m_queueToDraw;
	};
}

