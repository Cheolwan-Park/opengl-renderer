#pragma once

#include <stdint.h>
#include <string>
#include "common/common.hpp"
#include "SOIL2/SOIL2.h"

using std::string;

class Texture {
public:
	static void bind_null(uint32_t target);

private:
	constexpr static unsigned int default_flags =
		SOIL_FLAG_POWER_OF_TWO |
		SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_DDS_LOAD_DIRECT |
		SOIL_FLAG_PVR_LOAD_DIRECT |
		SOIL_FLAG_ETC1_LOAD_DIRECT |
		SOIL_FLAG_COMPRESS_TO_DXT;

private:
	uint32_t _id;

public:
	Texture();

	explicit Texture(const string& filename);

	~Texture();

	bool load(const string& filename, uint32_t flags = default_flags);

	void bind(uint32_t target) const;
};