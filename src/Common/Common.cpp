#include "Common.hpp"
#include <vector>
#include <fstream>
#include <algorithm> // std::count

std::string getSuffix(const std::string &str, char c) {
	std::string::size_type n = str.rfind(c);
	if (n == str.size() || n == std::string::npos) {
		return str;
	} else {
		return str.substr(n + 1, std::string::npos);
	}
}

std::string getPrefix(const std::string &str, char c)
{
	std::string::size_type n = str.rfind(c);
	if (n == 0 || n == std::string::npos) {
		return str;
	} else {
		return str.substr(0, n);
	}
}

std::string removePath(const std::string &path)
{
	std::string::size_type n = path.rfind('/');
	if (n == std::string::npos) n = path.rfind('\\');
	if (n == path.size() || n == std::string::npos) {
		return path;
	} else {
		return path.substr(n + 1, std::string::npos);
	}
}

std::size_t countFileLines(const std::string &file)
{
	std::ifstream stream;
	stream.open(file, std::ios::binary);
	if (!stream.is_open()) return 0;
	stream.seekg(0, std::ios_base::end);
	std::streamsize size = stream.tellg();
	stream.seekg(0, std::ios_base::beg);
	std::streamsize alloc = size > 209715200 ? 209715200 : size;
	std::vector<char> data(alloc);
	std::size_t lines = 0;
	while (size > 0) {
		stream.read(data.data(), alloc);
		lines += std::count(data.begin(), data.begin() + alloc, '\n');
		size -= alloc;
		if (size < alloc) alloc = size;
	}
	return lines;
}

const char* getChannelName(CFR::size_type channels) {
	switch (channels) {
	case 0:  return "NONE";
	case 1:  return "GRAYSCALE"; 
	case 2:  return "RG"; 
	case 3:  return "RGB"; 
	case 4:  return "RGBA";
	default: return "INVALID";
	}
}
