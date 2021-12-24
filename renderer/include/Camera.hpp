#pragma once

#include <glm/gtx/transform.hpp>

class Camera {
private:
	float _fov;
	float _near;
	float _far;
	glm::vec3 _position;
	glm::vec3 _front;
	glm::vec3 _up;

public:
	Camera() = default;

	void setup(float fov, float near, float far,
		const glm::vec3& position, const glm::vec3& front, const glm::vec3& up) {
		_fov = fov;
		_near = near;
		_far = far;
		_position = position;
		_front = glm::normalize(front);
		_up = glm::normalize(up);
	}

	glm::mat4x4 getViewMatrix() {
		return glm::lookAt(_position, _position + _front, _up);
	}

	glm::mat4x4 getProjectionMatrix(float aspect_ratio) {
		return glm::perspective(glm::radians(_fov), aspect_ratio, _near, _far);
	}

	float getFov() {
		return _fov;
	}

	float getNear() {
		return _near;
	}

	float getFar() {
		return _far;
	}

	const glm::vec3& getPosition() {
		return _position;
	}

	const glm::vec3& getFront() {
		return _front;
	}

	const glm::vec3& getUp() {
		return _up;
	}

	void setFov(float fov) {
		_fov = fov;
	}

	void setNear(float near) {
		_near = near;
	}

	void setFar(float far) {
		_far = far;
	}

	void move(const glm::vec3& delta) {
		_position += delta;
	}

	void setPosition(const glm::vec3& v) {
		_position = v;
	}

	void setFront(const glm::vec3& v) {
		_front = glm::normalize(v);
	}

	void setTarget(const glm::vec3& target) {
		_front = glm::normalize(target - _position);
	}

	void setUp(const glm::vec3& up) {
		_up = glm::normalize(up);
	}
};