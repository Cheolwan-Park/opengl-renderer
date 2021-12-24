#pragma once

#include <SDL.h>
#include <string>
#include <chrono>
#include <memory>

class Application;

class Program {
public:
	virtual ~Program() = default;

	virtual void update(const Application* app) = 0;

	virtual void render(const Application* app) = 0;

	virtual void render_gui(const Application* app) = 0;
};

class Application {
private:
	typedef std::chrono::duration<uint32_t, std::milli> milliseconds;
	typedef std::chrono::system_clock::time_point time_point;
	static constexpr milliseconds IDEAL_DELTATIME = milliseconds(16);

public:
	Application() = delete;
	explicit Application(const std::string& title, int width, int height);

	~Application();

	void run();

	void setProgram(Program* program);

	[[nodiscard]] float deltatime() const;

	[[nodiscard]] float now() const;

private:
	bool init(const std::string& title, int width, int height);

	void free();

	void render() const;

	void render_gui();

private:
	SDL_Window* _window;
	SDL_GLContext _context;
	std::shared_ptr<Program> _program;

	struct {
		time_point begin;
		time_point now;
		time_point start;
		milliseconds deltatime = milliseconds(0);
	} _time;
};
