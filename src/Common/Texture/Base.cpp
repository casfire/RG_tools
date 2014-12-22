#include "Base.hpp"

using Texture::size_type;
using Texture::Uint8;
using Texture::Uint16;
using Texture::Uint32;
using Texture::Pixel8;
using Texture::Pixel16;
using Texture::Pixel32;



/* Pixel access */

inline Uint16 get16(const Uint8 *pixels) {
	return
		  (static_cast<Uint16>(pixels[0]) << 8)
		| (static_cast<Uint16>(pixels[1]) << 0);
}

inline Uint32 get32(const Uint8 *pixels) {
	return
		  (static_cast<Uint32>(pixels[0]) << 24)
		| (static_cast<Uint32>(pixels[1]) << 16)
		| (static_cast<Uint32>(pixels[2]) << 8 )
		| (static_cast<Uint32>(pixels[3]) << 0 );
}

inline void set16(Uint16 value, Uint8 *pixels) {
	pixels[0] = static_cast<Uint8>(value >> 8);
	pixels[1] = static_cast<Uint8>(value >> 0);
}

inline void set32(Uint32 value, Uint8 *pixels) {
	pixels[0] = static_cast<Uint8>(value >> 24);
	pixels[1] = static_cast<Uint8>(value >> 16);
	pixels[2] = static_cast<Uint8>(value >> 8 );
	pixels[3] = static_cast<Uint8>(value >> 0 );
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



/* Texture::Base */

Texture::Base::Base()
: width(0), height(0), depth(0), channels(3), bytes(1)
{}

Texture::Base::Base(const Base &copy)
: width(copy.width), height(copy.height), depth(copy.depth),
  channels(copy.channels), bytes(copy.bytes)
{}

Texture::Base::Base(
	size_type width, size_type height, size_type depth,
	size_type channels, size_type bytes)
: width(width), height(height), depth(depth),
  channels(channels), bytes(bytes),
  pixels(width * height * depth * channels * bytes)
{}

Texture::Base::~Base()
{}

void* Texture::Base::getRawPixels()
{
	return static_cast<void*>(pixels.data());
}

const void* Texture::Base::getRawPixels() const
{
	return static_cast<const void*>(pixels.data());
}

size_type Texture::Base::getRawSize() const
{
	return width * height * depth * channels * bytes;
}

size_type Texture::Base::getWidth() const
{
	return width;
}
size_type Texture::Base::getHeight() const
{
	return height;
}

size_type Texture::Base::getDepth() const
{
	return depth;
}

size_type Texture::Base::getChannels() const
{
	return channels;
}

size_type Texture::Base::getBytes() const
{
	return bytes;
}

void Texture::Base::resize(
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

Uint32 Texture::Base::getPixel(size_type x, size_type y, size_type z) const
{
	size_type offset = (z * (width * height) + y * width + x) * channels * bytes;
	switch (bytes) {
	case 1:  return accessGet8 (pixels.data() + offset, channels).pixel();
	case 2:  return accessGet16(pixels.data() + offset, channels).pixel();
	case 4:  return accessGet32(pixels.data() + offset, channels).pixel();
	default: return 0;
	}
}

Pixel8 Texture::Base::getPixel8(size_type x, size_type y, size_type z) const
{
	size_type offset = (z * (width * height) + y * width + x) * channels * bytes;
	switch (bytes) {
	case 1:  return accessGet8 (pixels.data() + offset, channels);
	case 2:  return accessGet16(pixels.data() + offset, channels).pixel8();
	case 4:  return accessGet32(pixels.data() + offset, channels).pixel8();
	default: return 0;
	}
}

Pixel16 Texture::Base::getPixel16(size_type x, size_type y, size_type z) const
{
	size_type offset = (z * (width * height) + y * width + x) * channels * bytes;
	switch (bytes) {
	case 1:  return accessGet8 (pixels.data() + offset, channels).pixel16();
	case 2:  return accessGet16(pixels.data() + offset, channels);
	case 4:  return accessGet32(pixels.data() + offset, channels).pixel16();
	default: return 0;
	}
}

Pixel32 Texture::Base::getPixel32(size_type x, size_type y, size_type z) const
{
	size_type offset = (z * (width * height) + y * width + x) * channels * bytes;
	switch (bytes) {
	case 1:  return accessGet8 (pixels.data() + offset, channels).pixel32();
	case 2:  return accessGet16(pixels.data() + offset, channels).pixel32();
	case 4:  return accessGet32(pixels.data() + offset, channels);
	default: return 0;
	}
}

void Texture::Base::setPixel(Uint32 p, size_type x, size_type y, size_type z)
{
	size_type offset = (z * (width * height) + y * width + x) * channels * bytes;
	switch (bytes) {
	case 1: accessSet8 (Pixel8 (p), pixels.data() + offset, channels);
	case 2: accessSet16(Pixel16(p), pixels.data() + offset, channels);
	case 4: accessSet32(Pixel32(p), pixels.data() + offset, channels);
	}
}

void Texture::Base::setPixel8(Pixel8 p, size_type x, size_type y, size_type z)
{
	size_type offset = (z * (width * height) + y * width + x) * channels * bytes;
	switch (bytes) {
	case 1: accessSet8 (p,           pixels.data() + offset, channels);
	case 2: accessSet16(p.pixel16(), pixels.data() + offset, channels);
	case 4: accessSet32(p.pixel32(), pixels.data() + offset, channels);
	}
}

void Texture::Base::setPixel16(Pixel16 p, size_type x, size_type y, size_type z)
{
	size_type offset = (z * (width * height) + y * width + x) * channels * bytes;
	switch (bytes) {
	case 1: accessSet8 (p.pixel8(),  pixels.data() + offset, channels);
	case 2: accessSet16(p,           pixels.data() + offset, channels);
	case 4: accessSet32(p.pixel32(), pixels.data() + offset, channels);
	}
}

void Texture::Base::setPixel32(Pixel32 p, size_type x, size_type y, size_type z)
{
	size_type offset = (z * (width * height) + y * width + x) * channels * bytes;
	switch (bytes) {
	case 1: accessSet8 (p.pixel8(),  pixels.data() + offset, channels);
	case 2: accessSet16(p.pixel16(), pixels.data() + offset, channels);
	case 4: accessSet32(p,           pixels.data() + offset, channels);
	}
}



/* Conversions */

inline Uint32 to32From8(Uint8 value) {
	Uint32 v = static_cast<Uint32>(value);
	return (v << 0) | (v << 8) | (v << 16) | (v << 24);
}

inline Uint32 to32From16(Uint16 value) {
	Uint32 v = static_cast<Uint32>(value);
	return (v << 0) | (v << 16);
}

inline Uint16 to16From8(Uint8 value) {
	Uint16 v = static_cast<Uint16>(value);
	return (v << 0) | (v << 8);
}

inline Uint16 to16From32(Uint32 value) {
	return static_cast<Uint16>((value >> 16) & 0xFFFF);
}

inline Uint8 to8From16(Uint16 value) {
	return static_cast<Uint8>((value >> 8) & 0xFF);
}

inline Uint8 to8From32(Uint32 value) {
	return static_cast<Uint8>((value >> 24) & 0xFF);
}

inline Uint8  toRed   (Uint32 p) { return static_cast<Uint8>((p >> 24) & 0xFF); }
inline Uint8  toGreen (Uint32 p) { return static_cast<Uint8>((p >> 16) & 0xFF); }
inline Uint8  toBlue  (Uint32 p) { return static_cast<Uint8>((p >> 8 ) & 0xFF); }
inline Uint8  toAlpha (Uint32 p) { return static_cast<Uint8>((p >> 0 ) & 0xFF); }
inline Uint32 toPixel (Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	return
		  (static_cast<Uint32>(r) << 24)
		| (static_cast<Uint32>(g) << 16)
		| (static_cast<Uint32>(b) << 8)
		| (static_cast<Uint32>(a) << 0);
}



/* Texture::Pixel8 */
 
Pixel8::Pixel8(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
: r(r), g(g), b(b), a(a)
{}

Pixel8::Pixel8(Uint32 p)
: r(toRed(p)), g(toGreen(p)), b(toBlue(p)), a(toAlpha(p))
{}

Uint32 Pixel8::pixel() const
{
	return toPixel(r, g, b, a);
}

Pixel16 Pixel8::pixel16() const
{
	return Pixel16(to16From8(r), to16From8(g), to16From8(b), to16From8(a));
}

Pixel32 Pixel8::pixel32() const
{
	return Pixel32(to32From8(r), to32From8(g), to32From8(b), to32From8(a));
}



/* Texture::Pixel16 */

Pixel16::Pixel16(Uint16 r, Uint16 g, Uint16 b, Uint16 a)
: r(r), g(g), b(b), a(a)
{}

Pixel16::Pixel16(Uint32 p)
: r(to16From8(toRed(p))), g(to16From8(toGreen(p))), b(to16From8(toBlue(p))), a(to16From8(toAlpha(p)))
{}

Uint32 Pixel16::pixel() const
{
	return toPixel(to8From16(r), to8From16(g), to8From16(b), to8From16(a));
}

Pixel8 Pixel16::pixel8() const
{
	return Pixel8(to8From16(r), to8From16(g), to8From16(b), to8From16(a));
}

Pixel32 Pixel16::pixel32() const
{
	return Pixel32(to32From16(r), to32From16(g), to32From16(b), to32From16(a));
}



/* Texture::Pixel32 */

Pixel32::Pixel32(Uint32 r, Uint32 g, Uint32 b, Uint32 a)
: r(r), g(g), b(b), a(a)
{}

Pixel32::Pixel32(Uint32 p)
: r(to32From8(toRed(p))), g(to32From8(toGreen(p))), b(to32From8(toBlue(p))), a(to32From8(toAlpha(p)))
{}

Uint32 Pixel32::pixel() const
{
	return toPixel(to8From32(r), to8From32(g), to8From32(b), to8From32(a));
}

Pixel8 Pixel32::pixel8() const
{
	return Pixel8(to8From32(r), to8From32(g), to8From32(b), to8From32(a));
}

Pixel16 Pixel32::pixel16() const
{
	return Pixel16(to16From32(r), to16From32(g), to16From32(b), to16From32(a));
}
