#include "Common.hpp"
#include <vector>
#include <cstdint> // std::uintX_t
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

sf::Image textureToImage(const CFR::BaseTexture &from, CFR::size_type z)
{
	std::vector<std::uint8_t> pixels(from.getWidth() * from.getHeight() * 4);
	for (std::size_t y = 0; y < from.getHeight(); y++) {
		for (std::size_t x = 0; x < from.getWidth(); x++) {
			std::size_t offset = 4 * x + 4 * y * from.getWidth();
			CFR::Pixel8 pixel = from.getPixel8(x, y, z);
			if (from.getChannels() == 1) {
				pixel.g = pixel.r;
				pixel.b = pixel.r;
			}
			pixels[offset + 0] = pixel.r;
			pixels[offset + 1] = pixel.g;
			pixels[offset + 2] = pixel.b;
			pixels[offset + 3] = pixel.a;
		}
	}
	const sf::Uint8 *data = reinterpret_cast<const sf::Uint8*>(pixels.data());
	sf::Image image;
	image.create(from.getWidth(), from.getHeight(), data);
	return image;
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

CFR::size_type findChannels(const sf::Image &image)
{
	CFR::size_type channels = 0;
	sf::Vector2u size = image.getSize();
	sf::Color last = image.getPixel(0, 0);
	for (unsigned y = 0; y < size.y; y++) {
		for (unsigned x = 0; x < size.x; x++) {
			sf::Color pixel = image.getPixel(x, y);
			if (channels <= 0 && pixel.r != last.r) channels = 1;
			if (channels <= 1 && pixel.r != pixel.g && pixel.g != last.g) channels = 2;
			if (channels <= 2 && pixel.g != pixel.b && pixel.b != last.b) channels = 3;
			if (channels <= 3 && pixel.b != pixel.a && pixel.a != last.a) return 4;
			last = pixel;
		}
	}
	return channels;
}
