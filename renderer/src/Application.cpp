#include "Application.hpp"
#include "Input.hpp"
#include <GL/glew.h>
#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_sdl.h>
#include <cstdio>
#include <vector>
#include <thread>
#include <glm/gtx/transform.hpp>

Application::Application(const std::string& title, int width, int height)
	: _window(nullptr), _context(), _program(nullptr), _time() {
	init(title, width, height);
}

Application::~Application() {
	_program.reset();

	free();
}

void Application::run() {
	using std::chrono::duration_cast;
	using std::this_thread::sleep_for;

	SDL_Event event;
	int quit = false;

	auto& input = Input::Get();
	input.clear();
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			input.handleEvent(event);
			if (SDL_QUIT == event.type)
				quit = true;
		}

		// calculate deltatime and sleep
		time_point last = _time.now;
		_time.now = std::chrono::system_clock::now();
		_time.deltatime = duration_cast<milliseconds>(_time.now - last);
		if (_time.deltatime < IDEAL_DELTATIME) {
			sleep_for(IDEAL_DELTATIME - _time.deltatime);
			_time.now = std::chrono::system_clock::now();
			_time.deltatime = duration_cast<milliseconds>(_time.now - last);
		}

		if (_program)
			_program->update(this);
		input.clear();

		render();

		render_gui();

		SDL_GL_SwapWindow(_window);
	}
}

void Application::setProgram(Program* program) {
	_program = std::shared_ptr<Program>(program);
}

float Application::deltatime() const {
	return float(_time.deltatime.count()) / 1000.0f;
}

float Application::now() const {
	auto dur_ms = std::chrono::duration_cast<milliseconds>(_time.now - _time.begin);
	return float(dur_ms.count()) / 1000.0f;
}

bool Application::init(const std::string& title, int width, int height) {
	// begin time
	_time.begin = std::chrono::system_clock::now();

	// setup opengl
	const char* glsl_version = "#version 410";
#ifdef __APPLE__
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	_window = SDL_CreateWindow(title.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL);
	if (nullptr == _window) {
		fprintf(stderr, "failed to create window\n");
		return false;
	}

	_context = SDL_GL_CreateContext(_window);
	if (GLEW_OK != glewInit()) {
		fprintf(stderr, "failed initialize glew(opengl loader)\n");
		return false;
	}

	// setup opengl
	printf("OpenGL Version : %s\n", glGetString(GL_VERSION));
	printf("GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// setup imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(_window, _context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);

	return true;
}

void Application::free() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_window);

	SDL_Quit();
}

void Application::render() const {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_program->render(this);
}

void Application::render_gui() {
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(_window);
	ImGui::NewFrame();

	ImGui::Begin("Program Info");
	ImGui::Text("Program for studying OpenGL");
	ImGui::Text("deltatime : %.4f", deltatime());
	ImGui::Text("fps       : %.2f", 1.0f / deltatime());
	ImGui::Text("time      : %.2f", now());
	ImGui::End();

	_program->render_gui(this);

	ImGui::Render();
	SDL_GL_MakeCurrent(_window, _context);
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}