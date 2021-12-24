#pragma once

#include <SDL.h>
#include <GL/glew.h>

class Application;

class Input {
public:
	static constexpr GLuint NUM_MOUSE_BUTTONS = 8;
	struct Mouse {
		GLint x;
		GLint y;
		GLchar button_down_buf[NUM_MOUSE_BUTTONS];
		GLchar button_pressed_buf[NUM_MOUSE_BUTTONS];
	};

private:
	static constexpr GLuint KEYBOARD_BUFFER_SIZE = SDL_NUM_SCANCODES;
	struct Data {
		GLchar key_down_buf[KEYBOARD_BUFFER_SIZE];
		GLchar key_pressed_buf[KEYBOARD_BUFFER_SIZE];
		Mouse mouse_info;
		SDL_Window* window;
	};

	Input();

public:
	Input(const Input& other) = delete;

	~Input();

	Input& operator=(const Input& other) = delete;

	void clear();

	GLint isAnyKeyPressed() const;

	GLint isKeyDown(GLint key) const;

	GLint isKeyPressed(GLint key) const;

	GLint isButtonDown(GLuint button) const;

	GLint isButtonPressed(GLint button) const;

	const Mouse& getMouseInfo() const;

	void handleEvent(const SDL_Event& event);

public:
	static Input& Get();

private:
	Data m_data;
};