#pragma once

#include <stdint.h>
#include <string>
#include <glm/glm.hpp>
#include "Texture.hpp"

using std::string;

class Shader {
public:
	static void use_null();

private:
	static uint32_t _current_program;

private:
	uint32_t _program;

public:
	Shader();

	explicit Shader(const string& vert_file, const string& frag_file);

	~Shader();

	bool load(const string& vert_file, const string& frag_file);

	void use() const;

	void setUniform(const string& name, const glm::mat4x4& mat) const;

	void setUniform(const string& name, const glm::vec3& vec) const;

	void setUniform(const string& name, float v) const;

	void setTexture(const string& name, uint32_t target, const Texture& texture);
};