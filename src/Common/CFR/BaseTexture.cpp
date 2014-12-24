#include "BaseTexture.hpp"

using CFR::size_type;
using CFR::Uint8;
using CFR::Uint16;
using CFR::Uint32;
using CFR::Pixel8;
using CFR::Pixel16;
using CFR::Pixel32;



/* Pixel access */

inline Uint16 get16(const Uint8 *pixels) {
	return
		  (static_cast<Uint16>(pixels[0]) << 0)
		| (static_cast<Uint16>(pixels[1]) << 8);
}

inline Uint32 get32(const Uint8 *pixels) {
	return
		  (static_cast<Uint32>(pixels[0]) << 0)
		| (static_cast<Uint32>(pixels[1]) << 8)
		| (static_cast<Uint32>(pixels[2]) << 16)
		| (static_cast<Uint32>(pixels[3]) << 24);
}

inline void set16(Uint16 value, Uint8 *pixels) {
	pixels[0] = static_cast<Uint8>(value >> 0);
	pixels[1] = static_cast<Uint8>(value >> 8);
}

inline void set32(Uint32 value, Uint8 *pixels) {
	pixels[0] = static_cast<Uint8>(value >> 0);
	pixels[1] = static_cast<Uint8>(value >> 8);
	pixels[2] = static_cast<Uint8>(value >> 16);
	pixels[3] = static_cast<Uint8>(value >> 24);
}

inline Pixel8 accessGet8(const Uint8 *pixels, size_type channels) {
	Pixel8 pixel(0, 0, 0, 0xFF);
	if (channels > 0) pixel.r = pixels[0];
	if (channels > 1) pixel.g = pixels[1];
	if (channels > 2) pixel.b = pixels[2];
	if (channels > 3) pixel.a = pixels[3];
	return pixel;
}

inline Pixel16 accessGet16(const Uint8 *pixels, size_type channels) {
	Pixel16 pixel(0, 0, 0, 0xFFFF);
	if (channels > 0) pixel.r = get16(pixels + 0);
	if (channels > 1) pixel.g = get16(pixels + 2);
	if (channels > 2) pixel.b = get16(pixels + 4);
	if (channels > 3) pixel.a = get16(pixels + 6);
	return pixel;
}

inline Pixel32 accessGet32(const Uint8 *pixels, size_type channels) {
	Pixel32 pixel(0, 0, 0, 0xFFFFFFFF);
	if (channels > 0) pixel.r = get32(pixels + 0 );
	if (channels > 1) pixel.g = get32(pixels + 4 );
	if (channels > 2) pixel.b = get32(pixels + 8 );
	if (channels > 3) pixel.a = get32(pixels + 12);
	return pixel;
}

inline void accessSet8(Pixel8 pixel, Uint8 *pixels, size_type channels) {
	if (channels > 0) pixels[0] = pixel.r;
	if (channels > 1) pixels[1] = pixel.g;
	if (channels > 2) pixels[2] = pixel.b;
	if (channels > 3) pixels[3] = pixel.a;
}

inline void accessSet16(Pixel16 pixel, Uint8 *pixels, size_type channels) {
	if (channels > 0) set16(pixel.r, pixels + 0);
	if (channels > 1) set16(pixel.g, pixels + 2);
	if (channels > 2) set16(pixel.b, pixels + 4);
	if (channels > 3) set16(pixel.a, pixels + 6);
}

inline void accessSet32(Pixel32 pixel, Uint8 *pixels, size_type channels) {
	if (channels > 0) set32(pixel.r, pixels + 0 );
	if (channels > 1) set32(pixel.g, pixels + 4 );
	if (channels > 2) set32(pixel.b, pixels + 8 );
	if (channels > 3) set32(pixel.a, pixels + 12);
}



/* CFR::BaseTexture */

CFR::BaseTexture::BaseTexture()
: width(0), height(0), depth(0), channels(3), bytes(1)
{}

CFR::BaseTexture::BaseTexture(const BaseTexture &copy)
: width(copy.width), height(copy.height), depth(copy.depth),
  channels(copy.channels), bytes(copy.bytes)
{}

CFR::BaseTexture::BaseTexture(
	size_type width, size_type height, size_type depth,
	size_type channels, size_type bytes)
: width(width), height(height), depth(depth),
  channels(channels), bytes(bytes),
  pixels(width * height * depth * channels * bytes)
{}

CFR::BaseTexture::~BaseTexture()
{}

void* CFR::BaseTexture::getRawPixels()
{
	return static_cast<void*>(pixels.data());
}

const void* CFR::BaseTexture::getRawPixels() const
{
	return static_cast<const void*>(pixels.data());
}

size_type CFR::BaseTexture::getRawSize() const
{
	return width * height * depth * channels * bytes;
}

size_type CFR::BaseTexture::getWidth() const
{
	return width;
}
size_type CFR::BaseTexture::getHeight() const
{
	return height;
}

size_type CFR::BaseTexture::getDepth() const
{
	return depth;
}

size_type CFR::BaseTexture::getChannels() const
{
	return channels;
}

size_type CFR::BaseTexture::getBytes() const
{
	return bytes;
}

void CFR::BaseTexture::resize(
	size_type width, size_type height, size_type depth ,
	size_type channels, size_type bytes)
{
	if (channels > 4) channels = 4;
	if (bytes > 2) bytes = 4; else if (bytes == 0) bytes = 1;
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->channels = channels;
	this->bytes = bytes;
	pixels.resize(width * height * depth * channels * bytes);
}

Uint32 CFR::BaseTexture::getPixel(size_type x, size_type y, size_type z) const
{
	size_type offset = (z * (width * height) + y * width + x) * channels * bytes;
	switch (bytes) {
	case 1:  return accessGet8 (pixels.data() + offset, channels).pixel();
	case 2:  return accessGet16(pixels.data() + offset, channels).pixel();
	case 4:  return accessGet32(pixels.data() + offset, channels).pixel();
	default: return 0;
	}
}

Pixel8 CFR::BaseTexture::getPixel8(size_type x, size_type y, size_type z) const
{
	size_type offset = (z * (width * height) + y * width + x) * channels * bytes;
	switch (bytes) {
	case 1:  return accessGet8 (pixels.data() + offset, channels);
	case 2:  return accessGet16(pixels.data() + offset, channels).pixel8();
	case 4:  return accessGet32(pixels.data() + offset, channels).pixel8();
	default: return 0;
	}
}

Pixel16 CFR::BaseTexture::getPixel16(size_type x, size_type y, size_type z) const
{
	size_type offset = (z * (width * height) + y * width + x) * channels * bytes;
	switch (bytes) {
	case 1:  return accessGet8 (pixels.data() + offset, channels).pixel16();
	case 2:  return accessGet16(pixels.data() + offset, channels);
	case 4:  return accessGet32(pixels.data() + offset, channels).pixel16();
	default: return 0;
	}
}

Pixel32 CFR::BaseTexture::getPixel32(size_type x, size_type y, size_type z) const
{
	size_type offset = (z * (width * height) + y * width + x) * channels * bytes;
	switch (bytes) {
	case 1:  return accessGet8 (pixels.data() + offset, channels).pixel32();
	case 2:  return accessGet16(pixels.data() + offset, channels).pixel32();
	case 4:  return accessGet32(pixels.data() + offset, channels);
	default: return 0;
	}
}

void CFR::BaseTexture::setPixel(Uint32 p, size_type x, size_type y, size_type z)
{
	size_type offset = (z * (width * height) + y * width + x) * channels * bytes;
	switch (bytes) {
	case 1: accessSet8 (Pixel8 (p), pixels.data() + offset, channels); break;
	case 2: accessSet16(Pixel16(p), pixels.data() + offset, channels); break;
	case 4: accessSet32(Pixel32(p), pixels.data() + offset, channels); break;
	}
}

void CFR::BaseTexture::setPixel8(Pixel8 p, size_type x, size_type y, size_type z)
{
	size_type offset = (z * (width * height) + y * width + x) * channels * bytes;
	switch (bytes) {
	case 1: accessSet8 (p,           pixels.data() + offset, channels); break;
	case 2: accessSet16(p.pixel16(), pixels.data() + offset, channels); break;
	case 4: accessSet32(p.pixel32(), pixels.data() + offset, channels); break;
	}
}

void CFR::BaseTexture::setPixel16(Pixel16 p, size_type x, size_type y, size_type z)
{
	size_type offset = (z * (width * height) + y * width + x) * channels * bytes;
	switch (bytes) {
	case 1: accessSet8 (p.pixel8(),  pixels.data() + offset, channels); break;
	case 2: accessSet16(p,           pixels.data() + offset, channels); break;
	case 4: accessSet32(p.pixel32(), pixels.data() + offset, channels); break;
	}
}

void CFR::BaseTexture::setPixel32(Pixel32 p, size_type x, size_type y, size_type z)
{
	size_type offset = (z * (width * height) + y * width + x) * channels * bytes;
	switch (bytes) {
	case 1: accessSet8 (p.pixel8(),  pixels.data() + offset, channels); break;
	case 2: accessSet16(p.pixel16(), pixels.data() + offset, channels); break;
	case 4: accessSet32(p,           pixels.data() + offset, channels); break;
	}
}
