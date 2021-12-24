#include "Shader.hpp"
#include "util.hpp"
#include <cstdio>
#include <vector>
#include <cassert>
#include <GL/glew.h>

GLuint Shader::_current_program = 0;

void Shader::use_null() {
	glUseProgram(0);
	_current_program = 0;
}

Shader::Shader()
	:_program(0) {
	;
}

Shader::Shader(const string& vert_file, const string& frag_file)
	: Shader() {
	load(vert_file, frag_file);
}

Shader::~Shader() {
	if (0 != _program)
		glDeleteProgram(_program);
}

bool Shader::load(const string& vert_file, const string& frag_file) {
	// read files
	string vert_text, frag_text;
	if (!readWholeFile(vert_file, vert_text)) {
		fprintf(stderr, "cannot read vertex shader, filename: %s\n", vert_file.c_str());
		return false;
	}
	if (!readWholeFile(frag_file, frag_text)) {
		fprintf(stderr, "cannot read fragment shader, filename: %s\n", frag_file.c_str());
		return false;
	}

	// create shader
	GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);

	GLint log_len = 0;

	// compile vertex shader
	auto vert_text_ptr = vert_text.c_str();
	glShaderSource(vert_id, 1, &vert_text_ptr, nullptr);
	glCompileShader(vert_id);

	glGetShaderiv(vert_id, GL_INFO_LOG_LENGTH, &log_len);
	if (log_len > 0) {
		std::vector<char> error_msg(log_len + 1);
		glGetShaderInfoLog(vert_id, log_len, nullptr, error_msg.data());
		fprintf(stderr, "failed to compile vertex shader\n%s\n", error_msg.data());
		return false;
	}

	// compile fragment shader
	auto frag_text_ptr = frag_text.c_str();
	glShaderSource(frag_id, 1, &frag_text_ptr, nullptr);
	glCompileShader(frag_id);

	glGetShaderiv(frag_id, GL_INFO_LOG_LENGTH, &log_len);
	if (log_len > 0) {
		std::vector<char> error_msg(log_len + 1);
		glGetShaderInfoLog(frag_id, log_len, nullptr, error_msg.data());
		fprintf(stderr, "failed to compile fragment shader\n%s\n", error_msg.data());
		return false;
	}

	// link
	_program = glCreateProgram();
	glAttachShader(_program, vert_id);
	glAttachShader(_program, frag_id);
	glLinkProgram(_program);

	glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &log_len);
	if (log_len > 0) {
		std::vector<char> error_msg(log_len + 1);
		glGetProgramInfoLog(_program, log_len, nullptr, error_msg.data());
		fprintf(stderr, "failed to link shaders\n%s\n", error_msg.data());
		return false;
	}

	glDetachShader(_program, vert_id);
	glDetachShader(_program, frag_id);
	glDeleteShader(vert_id);
	glDeleteShader(frag_id);

	return true;
}

void Shader::use() const {
	glUseProgram(_program);
	_current_program = _program;
}

void Shader::setUniform(const string& name, const glm::mat4x4& mat) const {
	assert(_current_program == _program);
	auto location = glGetUniformLocation(_program, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setUniform(const string& name, const glm::vec3& vec) const {
	assert(_current_program == _program);
	auto location = glGetUniformLocation(_program, name.c_str());
	glUniform3fv(location, 1, &vec.x);
}

void Shader::setUniform(const string& name, float v) const {
	assert(_current_program == _program);
	auto location = glGetUniformLocation(_program, name.c_str());
	glUniform1f(location, v);
}

void Shader::setTexture(const string& name, uint32_t target, const Texture& texture) {
	assert(_current_program == _program);
	auto location = glGetUniformLocation(_program, name.c_str());
	glUniform1i(location, target);
	glActiveTexture(target);
	texture.bind(GL_TEXTURE_2D);
}