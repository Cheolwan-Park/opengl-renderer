#pragma once

#include <OBJ_Loader.h>
#include <stdint.h>
#include <string>

using std::string;

class Mesh {
private:
	objl::Mesh _mesh;
	uint32_t _vao;
	uint32_t _arrayBuffer;
	uint32_t _elementBuffer;

public:
	Mesh();

	explicit Mesh(const string& obj_file);

	~Mesh() = default;

	bool load(const string& obj_file);

	bool updateVao();

	void draw();

private:
	void allocateBuffers();
};