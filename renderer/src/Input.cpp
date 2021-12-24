#include "Input.hpp"
#include "Application.hpp"
#include <cstring>

Input::Input()
	: m_data() {
	memset(&m_data, 0, sizeof(Data));
}

Input::~Input() {
	;
}

void Input::clear() {
	memset(m_data.key_pressed_buf, 0, sizeof(m_data.key_pressed_buf));
	memset(m_data.mouse_info.button_pressed_buf, 0,
		sizeof(m_data.mouse_info.button_pressed_buf));
}

GLint Input::isAnyKeyPressed() const {
	for (Uint32 i = 0; i < KEYBOARD_BUFFER_SIZE; ++i) {
		if (m_data.key_pressed_buf[i])
			return true;
	}
	return false;
}

GLint Input::isKeyDown(GLint key) const {
	return m_data.key_down_buf[key];
}

GLint Input::isKeyPressed(GLint key) const {
	return m_data.key_pressed_buf[key];
}

GLint Input::isButtonDown(GLuint button) const {
	return m_data.mouse_info.button_down_buf[button];
}

GLint Input::isButtonPressed(GLint button) const {
	return m_data.mouse_info.button_pressed_buf[button];
}

const Input::Mouse& Input::getMouseInfo() const {
	return m_data.mouse_info;
}

void Input::handleEvent(const SDL_Event& event) {
	switch (event.type) {
	case SDL_KEYDOWN: {
		m_data.key_down_buf[event.key.keysym.scancode] = SDL_TRUE;
		m_data.key_pressed_buf[event.key.keysym.scancode] = SDL_TRUE;
		break;
	}

	case SDL_KEYUP: {
		m_data.key_down_buf[event.key.keysym.scancode] = SDL_FALSE;
		break;
	}

	case SDL_MOUSEMOTION: {
		SDL_GetMouseState(&m_data.mouse_info.x, &m_data.mouse_info.y);
		break;
	}

	case SDL_MOUSEBUTTONDOWN: {
		SDL_GetMouseState(&m_data.mouse_info.x, &m_data.mouse_info.y);
		m_data.mouse_info.button_down_buf[event.button.button] = SDL_TRUE;
		m_data.mouse_info.button_pressed_buf[event.button.button] = SDL_TRUE;
		break;
	}

	case SDL_MOUSEBUTTONUP: {
		SDL_GetMouseState(&m_data.mouse_info.x, &m_data.mouse_info.y);
		m_data.mouse_info.button_down_buf[event.button.button] = SDL_FALSE;
		break;
	}

	default: break;
	}
}

Input& Input::Get() {
	static Input instance;
	return instance;
}