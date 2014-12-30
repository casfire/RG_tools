#include "Common.hpp"

using CFR::size_type;
using CFR::Uint8;
using CFR::Uint16;
using CFR::Uint32;
using CFR::Pixel8;
using CFR::Pixel16;
using CFR::Pixel32;
using CFR::Vec2;
using CFR::Vec3;
using CFR::Vertex;



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



/* CFR::Pixel8 */
 
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



/* CFR::Pixel16 */

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



/* CFR::Pixel32 */

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



/* CFR::Vertex */

Vec2::Vec2(float x, float y)
: x(x), y(y)
{}

Vec3::Vec3(float x, float y, float z)
: x(x), y(y), z(z)
{}

bool Vertex::operator <(const CFR::Vertex &obj) const
{
	std::hash<Vertex>::result_type hashA = std::hash<Vertex>()(*this);
	std::hash<Vertex>::result_type hashB = std::hash<Vertex>()(obj);
	if (hashA == hashB) {
		return std::equal_to<CFR::Vertex>()(*this, obj);
	} else {
		return hashA < hashB;
	}
}

bool Vertex::operator==(const CFR::Vertex &b) const
{
	const Vertex& a = *this;
	return
		   a.position.x == b.position.x
		&& a.position.y == b.position.y
		&& a.position.z == b.position.z
		&& a.texcoord.x == b.texcoord.x
		&& a.texcoord.y == b.texcoord.y
		&& a.normal.x   == b.normal.x
		&& a.normal.y   == b.normal.y
		&& a.normal.z   == b.normal.z
		&& a.tangent.x  == b.tangent.x
		&& a.tangent.y  == b.tangent.y
		&& a.tangent.z  == b.tangent.z
		&& a.binormal.x == b.binormal.x
		&& a.binormal.y == b.binormal.y
		&& a.binormal.z == b.binormal.z;
}

std::hash<Vertex>::result_type std::hash<Vertex>::operator()(Vertex const& v) const
{
	std::hash<float> hash;
	return 
		  (hash(v.position.x) * 0x8AF785E7)
		^ (hash(v.position.y) * 0x200CD333)
		^ (hash(v.position.z) * 0x0AE9ED1B)
		^ (hash(v.texcoord.x) * 0x20ADB3A9)
		^ (hash(v.texcoord.y) * 0x6E63CBD1)
		^ (hash(v.normal.x)   * 0xE4492C33)
		^ (hash(v.normal.y)   * 0xF201058D)
		^ (hash(v.normal.z)   * 0xFD0B4D09)
		^ (hash(v.tangent.x)  * 0xEF7A6E89)
		^ (hash(v.tangent.y)  * 0xAE028975)
		^ (hash(v.tangent.z)  * 0x07EECD6F)
		^ (hash(v.binormal.x) * 0xF380DC15)
		^ (hash(v.binormal.x) * 0xE17BBD23)
		^ (hash(v.binormal.z) * 0xBC154D09);
}



/* CFR::Exception */

CFR::Exception::Exception(const std::string &info)
: info(info)
{}

const char* CFR::Exception::what() const throw()
{
	return info.c_str();
}
