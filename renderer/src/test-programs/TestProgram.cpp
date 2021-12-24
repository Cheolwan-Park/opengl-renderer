#include "test-programs/TestProgram.hpp"
#include "Input.hpp"
#include <glm/gtx/transform.hpp>

TestProgram::TestProgram()
	:_cube("models/ninjaHead.obj"),
	_shader("shaders/vertex.vert", "shaders/frag.frag"),
	_tex("textures/earth.jpg"),
	_cam(), _cam_rot(-90.0f, 0.0f), _prev_mouse_pos(), _mouse_moving(false) {
	_cam.setup(75.0f, 0.1f, 1000.0f,
		glm::vec3(0, 0, 3), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
	_cam.setTarget(glm::vec3(0, 0, 0));
}

void TestProgram::update(const Application* app) {
	auto& input = Input::Get();

	glm::vec2 moveVec = glm::vec3();
	if (input.isKeyDown(SDL_SCANCODE_W)) {
		moveVec.y += 1;
	}
	else if (input.isKeyDown(SDL_SCANCODE_S)) {
		moveVec.y -= 1;
	}
	if (input.isKeyDown(SDL_SCANCODE_A)) {
		moveVec.x -= 1;
	}
	else if (input.isKeyDown(SDL_SCANCODE_D)) {
		moveVec.x += 1;
	}

	glm::vec2 rot_vec(0, 0);
	auto& mouse_state = input.getMouseInfo();
	if (mouse_state.button_pressed_buf[SDL_BUTTON_RIGHT] && !_mouse_moving) {
		_mouse_moving = true;
		_prev_mouse_pos.x = mouse_state.x;
		_prev_mouse_pos.y = mouse_state.y;
	}
	if (mouse_state.button_down_buf[SDL_BUTTON_RIGHT]) {
		glm::vec2 cur_mouse_pos(mouse_state.x, mouse_state.y);
		auto mouse_delta = cur_mouse_pos - _prev_mouse_pos;
		rot_vec = mouse_delta * 0.1f;
		rot_vec.y = -rot_vec.y;
		_prev_mouse_pos = cur_mouse_pos;
	}
	else {
		_mouse_moving = false;
	}

	_cam_rot += rot_vec;
	_cam_rot.y = glm::clamp(_cam_rot.y, -89.0f, 89.0f);
	glm::vec3 rotated_vector;
	rotated_vector.x = cos(glm::radians(_cam_rot.y)) * cos(glm::radians(_cam_rot.x));
	rotated_vector.y = sin(glm::radians(_cam_rot.y));
	rotated_vector.z = cos(glm::radians(_cam_rot.y)) * sin(glm::radians(_cam_rot.x));
	_cam.setFront(rotated_vector);

	auto front = _cam.getFront();
	auto right = glm::cross(front, _cam.getUp());
	auto delta = front * moveVec.y + right * moveVec.x;
	_cam.move(delta);
}

void TestProgram::render(const Application* app) {
	_shader.use();

	// model matrix
	auto model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)) *
		/*glm::rotate(app->now(), glm::vec3(0, 1, 0)) *
		glm::rotate(app->now(), glm::vec3(1, 0, 1)) **/
		glm::scale(glm::vec3(1, 1, 1));

	// view matrix
	auto view_matrix = _cam.getViewMatrix();

	// projection matrix
	auto projection_matrix = _cam.getProjectionMatrix(960.0f / 540.0f);

	// vp matrix
	auto vp_matrix = projection_matrix * view_matrix;

	// light
	glm::vec3 light_pos(0, 0, 300);

	_shader.setUniform("model_matrix", model_matrix);
	_shader.setUniform("vp_matrix", vp_matrix);
	_shader.setUniform("world_camera_position", _cam.getPosition());
	_shader.setUniform("world_light_position", light_pos);

	_shader.setUniform("light_color", glm::vec3(1, 1, 1));
	_shader.setTexture("tex_object", GL_TEXTURE0, _tex);

	_cube.draw();

	Shader::use_null();
}

void TestProgram::render_gui(const Application* app) {
	;
}