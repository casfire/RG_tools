#include "Texture.hpp"
#include <fstream>

using std::uint32_t;
using std::uint16_t;
using std::uint8_t;



/* CFR::Texture */

CFR::Texture::Texture()
: CFR::BaseTexture()
{}

CFR::Texture::Texture(const CFR::BaseTexture &copy)
: CFR::BaseTexture(copy)
{}

CFR::Texture::Texture(
	size_type width, size_type height, size_type depth,
	size_type channels, size_type bytes)
: CFR::BaseTexture(width, height, depth, channels, bytes)
{}

void CFR::Texture::loadFromFile(const std::string &file)
{
	try {
		std::ifstream stream;
		stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		stream.open(file, std::ios::binary);
		stream >> *this;
		stream.close();
	} catch (std::ios::failure &fail) {
		throw CFR::Exception("IO error: " + std::string(fail.what()));
	}
}

void CFR::Texture::saveToFile(const std::string &file) const
{
	try {
		std::ofstream stream;
		stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		stream.open(file, std::ios::binary);
		stream << *this;
		stream.close();
	} catch (std::ios::failure &fail) {
		throw CFR::Exception("IO error: " + std::string(fail.what()));
	}
}



/* Stream insertion/extraction */

inline uint8_t read8(std::istream &in) {
	return static_cast<uint8_t>(in.get());
}

inline uint16_t read16(std::istream &in) {
	uint16_t t = 0;
	t |= static_cast<uint16_t>(in.get()) << 0;
	t |= static_cast<uint16_t>(in.get()) << 8;
	return t;
}

inline uint32_t read32(std::istream &in) {
	uint32_t t = 0;
	t |= static_cast<uint32_t>(in.get()) << 0;
	t |= static_cast<uint32_t>(in.get()) << 8;
	t |= static_cast<uint32_t>(in.get()) << 16;
	t |= static_cast<uint32_t>(in.get()) << 24;
	return t;
}

inline void write8(std::ostream &out, uint8_t v) {
	out.put(v);
}

inline void write16(std::ostream &out, uint16_t v) {
	write8(out, static_cast<uint8_t>((v >> 0) & 0xFF));
	write8(out, static_cast<uint8_t>((v >> 8) & 0xFF));
}

inline void write32(std::ostream &out, uint32_t v) {
	write8(out, static_cast<uint8_t>((v >> 0 ) & 0xFF));
	write8(out, static_cast<uint8_t>((v >> 8 ) & 0xFF));
	write8(out, static_cast<uint8_t>((v >> 16) & 0xFF));
	write8(out, static_cast<uint8_t>((v >> 24) & 0xFF));
}

std::istream& operator>>(std::istream& in, CFR::Texture& obj)
{
	if (read32(in) != 0x43465254) {
		throw CFR::Exception("Invalid magic number.");
	} else if (read8(in) != 1) {
		throw CFR::Exception("Invalid version.");
	}
	uint16_t width    = read16(in);
	uint16_t height   = read16(in);
	uint16_t depth    = read16(in);
	uint8_t  channels = read8 (in);
	uint8_t  bytes    = read8 (in);
	if (channels == 0 || channels > 4) {
		throw CFR::Exception("Invalid number of channels.");
	} else if (bytes == 0 || bytes == 3 || channels > 4) {
		throw CFR::Exception("Invalid number of bytes per color.");
	}
	obj.resize(width, height, depth, channels, bytes);
	in.read(reinterpret_cast<char*>(obj.getRawPixels()), obj.getRawSize());
	return in;
}

std::ostream& operator<<(std::ostream& out, const CFR::Texture& obj)
{
	if (obj.getWidth() > 0xFFFF || obj.getHeight() > 0xFFFF || obj.getDepth() > 0xFFFF) {
		throw CFR::Exception("Dimensions are too big.");
	} else if (obj.getChannels() > 4) {
		throw CFR::Exception("Invalid number of channels.");
	} else if (obj.getBytes() == 3 || obj.getBytes() > 4) {
		throw CFR::Exception("Invalid number of bytes per color.");
	}
	write32(out, 0x43465254);
	write8 (out, 1);
	write16(out, static_cast<uint16_t>(obj.getWidth()));
	write16(out, static_cast<uint16_t>(obj.getHeight()));
	write16(out, static_cast<uint16_t>(obj.getDepth()));
	write8 (out, static_cast<uint8_t >(obj.getChannels()));
	write8 (out, static_cast<uint16_t>(obj.getBytes()));
	out.write(static_cast<const char*>(obj.getRawPixels()), obj.getRawSize());
	return out;
}
