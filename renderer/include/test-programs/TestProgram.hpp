#pragma once

#include "Application.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include <glm/glm.hpp>
#include <string>

class TestProgram : public Program {
private:
	Mesh _cube;
	Shader _shader;
	Texture _tex;
	Camera _cam;
	glm::vec2 _cam_rot;
	glm::vec2 _prev_mouse_pos;
	bool _mouse_moving;

public:
	TestProgram();

	~TestProgram() override = default;

	void update(const Application* app) override;

	void render(const Application* app) override;

	void render_gui(const Application* app) override;
};