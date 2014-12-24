#pragma once
#ifndef _CFR_COMMON_HPP_
#define _CFR_COMMON_HPP_

#include <cstddef> // std::size_t
#include <cstdint> // std::uintX_t
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
	
	/* Geometry vertex */
	struct Vertex {
		float position[3];
		float normal  [3];
		float texture [3];
		Vertex(
			float px = 0.f, float py = 0.f, float pz = 0.f,
			float nx = 0.f, float ny = 0.f, float nz = 0.f,
			float tx = 0.f, float ty = 0.f, float tz = 0.f
		);
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

#endif // _CFR_COMMON_HPP_
