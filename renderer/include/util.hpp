#pragma once

#include <string>
#include <fstream>

inline bool readWholeFile(const std::string& path, std::string& out) {
	out = "";
	std::ifstream fin(path);
	if (!fin) return false;
	fin.seekg(0, std::ios::end);
	out.reserve(fin.tellg());
	fin.seekg(0, std::ios::beg);
	out.assign(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());
	return true;
}
