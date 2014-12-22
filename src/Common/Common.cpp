#include "Common.hpp"
#include <vector>
#include <cstddef> // std::size_t
#include <cstdint> // std::uintX_t

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

std::string getOnlyFile(const std::string &path)
{
	return getSuffix(getSuffix(path, '\\'), '/');
}

sf::Image textureToImage(const Texture::Base &from, Texture::size_type z)
{
	std::vector<std::uint8_t> pixels(from.getWidth() * from.getHeight() * 4);
	for (std::size_t y = 0; y < from.getHeight(); y++) {
		for (std::size_t x = 0; x < from.getWidth(); x++) {
			std::size_t offset = 4 * x + 4 * y * from.getWidth();
			Texture::Pixel8 pixel = from.getPixel8(x, y, z);
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

const char* getChannelName(Texture::size_type channels) {
	switch (channels) {
	case 0:  return "NONE";
	case 1:  return "GRAYSCALE"; 
	case 2:  return "RG"; 
	case 3:  return "RGB"; 
	case 4:  return "RGBA";
	default: return "INVALID";
	}
}

Texture::size_type findChannels(const sf::Image &image)
{
	Texture::size_type channels = 0;
	sf::Vector2u size = image.getSize();
	sf::Color last = image.getPixel(0, 0);
	for (unsigned y = 0; y < size.y; y++) {
		for (unsigned x = 0; x < size.x; x++) {
			sf::Color pixel = image.getPixel(x, y);
			if (channels == 0 && pixel.r != last.r) channels = 1;
			if (channels == 1 && pixel.r != pixel.g && pixel.g != last.g) channels = 2;
			if (channels == 2 && pixel.g != pixel.b && pixel.b != last.b) channels = 3;
			if (channels == 3 && pixel.b != pixel.a && pixel.a != last.a) return 4;
		}
	}
	return channels;
}
