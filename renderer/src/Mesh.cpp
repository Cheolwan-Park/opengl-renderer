#include "Mesh.hpp"
#include <cassert>
#include <GL/glew.h>
#include <cstdio>

Mesh::Mesh()
	: _mesh(), _vao(0), _arrayBuffer(0), _elementBuffer(0) {
	allocateBuffers();
}

Mesh::Mesh(const string& obj_file)
	: Mesh() {
	auto result = load(obj_file);
	assert(result);
}

#include <chrono>
bool Mesh::load(const string& obj_file) {
	objl::Loader loader;

	auto time_point = std::chrono::system_clock::now();

	bool result = loader.LoadFile(obj_file);
	if (!result) {
		fprintf(stderr, "failed to load %s\n", obj_file.c_str());
		return false;
	}

	if (loader.LoadedMeshes.empty()) {
		fprintf(stderr, "%s has no mesh\n", obj_file.c_str());
		return false;
	}

	auto deltatime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - time_point);
	printf("%f\n", float(deltatime.count()) / 1000.0f);

	_mesh = loader.LoadedMeshes[0];

	printf("Verticies count: %llu\n", _mesh.Vertices.size());

	return updateVao();
}

bool Mesh::updateVao() {
	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _arrayBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(objl::Vertex) * _mesh.Vertices.size(),
		_mesh.Vertices.data(),
		GL_STATIC_DRAW);

	glVertexAttribPointer(0,            // attribute index
		3,            // size
		GL_FLOAT,     // type
		GL_FALSE,     // normalized?
		sizeof(objl::Vertex), // stride
		(void*)offsetof(objl::Vertex, Position)); // start offset
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2,
		GL_FLOAT, GL_FALSE, sizeof(objl::Vertex),
		(void*)offsetof(objl::Vertex, TextureCoordinate));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3,
		GL_FLOAT, GL_FALSE, sizeof(objl::Vertex),
		(void*)offsetof(objl::Vertex, Normal));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int) * _mesh.Indices.size(),
		_mesh.Indices.data(),
		GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return true;
}

void Mesh::draw() {
	glBindVertexArray(_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer);
	glDrawElements(GL_TRIANGLES, _mesh.Indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::allocateBuffers() {
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_arrayBuffer);
	glGenBuffers(1, &_elementBuffer);
}