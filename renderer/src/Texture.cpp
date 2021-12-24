#include "Texture.hpp"
#include <cassert>
#include <GL/glew.h>

void Texture::bind_null(uint32_t target) {
	glBindTexture(target, 0);
}

Texture::Texture()
	:_id(0) {
}

Texture::~Texture() {
	if (_id > 0)
		glDeleteTextures(1, &_id);
}

Texture::Texture(const string& filename)
	:Texture() {
	auto result = load(filename);
	assert(result);
}

bool Texture::load(const string& filename, uint32_t flags) {
	GLuint using_id = SOIL_CREATE_NEW_ID;
	if (_id > 0)
		using_id = _id;
	_id = SOIL_load_OGL_texture(filename.c_str(),
		SOIL_LOAD_AUTO,
		using_id,
		flags);
	if (0 == _id) {
		fprintf(stderr, "texture loading failed(%s): %s\n", filename.c_str(), SOIL_last_result());
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, _id);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::bind(uint32_t target) const {
	glBindTexture(GL_TEXTURE_2D, _id);
}