#pragma once
#ifndef _CFR_COMMON_HPP_
#define _CFR_COMMON_HPP_

#include <cstddef> // std::size_t
#include <cstdint> // std::uintX_t
#include <functional> // std::hash
#include <exception>
#include <string>

namespace CFR {
	
	
	
	typedef std::size_t   size_type;
	typedef std::uint8_t  Uint8;
	typedef std::uint16_t Uint16;
	typedef std::uint32_t Uint32;
	
	struct Pixel8;
	struct Pixel16;
	struct Pixel32;
	struct Vec2;
	struct Vec3;
	struct Vertex;
	class  Exception;
	class  BaseTexture;
	class  Texture;
	class  BaseGeometry;
	class  Geometry;
	
	
	
	/* 8-bit pixel */
	struct Pixel8 {
		Uint8 r, g, b, a;
		Pixel8(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xFF);
		Pixel8(Uint32 pixel);
		Uint32  pixel()   const;
		Pixel16 pixel16() const;
		Pixel32 pixel32() const;
	};
	
	/* 16-bit pixel */
	struct Pixel16 {
		Uint16 r, g, b, a;
		Pixel16(Uint16 r, Uint16 g, Uint16 b, Uint16 a = 0xFFFF);
		Pixel16(Uint32 pixel);
		Uint32  pixel()   const;
		Pixel8  pixel8()  const;
		Pixel32 pixel32() const;
	};
	
	/* 32-bit pixel */
	struct Pixel32 {
		Uint32 r, g, b, a;
		Pixel32(Uint32 r, Uint32 g, Uint32 b, Uint32 a = 0xFFFFFFFF);
		Pixel32(Uint32 pixel);
		Uint32  pixel()   const;
		Pixel8  pixel8()  const;
		Pixel16 pixel16() const;
	};
	
	struct Vec2 {
		float x, y;
		Vec2(float x = 0.f, float y = 0.f);
	};
	
	struct Vec3 {
		float x, y, z;
		Vec3(float x = 0.f, float y = 0.f, float z = 0.f);
	};
	
	/* Geometry vertex */
	struct Vertex {
		Vec3 position;
		Vec2 texcoord;
		Vec3 normal;
		Vec3 tangent;
		Vec3 binormal;
		bool operator <(const CFR::Vertex&) const;
		bool operator==(const CFR::Vertex&) const;
	};
	
	
	
	/* CFR Base exception */
	class Exception : public std::exception {
	public:
		
		Exception(const std::string &info);
		const char* what() const throw();
		
	private:
		
		const std::string info;
		
	};
	
	
	
} // namespace CFR

namespace std {
	template<> struct hash<CFR::Vertex> {
		typedef CFR::Vertex argument_type;
		typedef std::hash<float>::result_type result_type;
		result_type operator()(const CFR::Vertex&) const;
	};
}

#endif // _CFR_COMMON_HPP_
